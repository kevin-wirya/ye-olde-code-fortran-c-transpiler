#include "codegen/CodeGenVisitor.h"

void CodeGenVisitor::collectReferencedLabels(ASTNode* node){
    if(!node)return;
    if(auto g=dynamic_cast<GotoNode*>(node)){
        referenced_labels.insert(g->target_label);
    }else if(auto cg=dynamic_cast<ComputedGotoNode*>(node)){
        for(int lbl:cg->labels){
            referenced_labels.insert(lbl);
        }
    }else if(auto p=dynamic_cast<ProgramNode*>(node)){
        for(auto& s:p->body)collectReferencedLabels(s.get());
    }else if(auto sub=dynamic_cast<SubroutineNode*>(node)){
        for(auto& s:sub->body)collectReferencedLabels(s.get());
    }else if(auto fn=dynamic_cast<FunctionNode*>(node)){
        for(auto& s:fn->body)collectReferencedLabels(s.get());
    }else if(auto ifn=dynamic_cast<IfNode*>(node)){
        for(auto& s:ifn->then_body)collectReferencedLabels(s.get());
        for(auto& s:ifn->else_body)collectReferencedLabels(s.get());
    }else if(auto don=dynamic_cast<DoNode*>(node)){
        for(auto& s:don->body)collectReferencedLabels(s.get());
    }
}

bool CodeGenVisitor::hasStringVars(ASTNode* node){
    if(!node) return false;
    if(auto tnode=dynamic_cast<TypeDeclNode*>(node)){
        if(tnode->type_name.rfind("CHARACTER",0)==0) return true;
    }else if(auto anode=dynamic_cast<ArrayDeclNode*>(node)){
        if(anode->type_name.rfind("CHARACTER",0)==0) return true;
    }else if(auto p=dynamic_cast<ProgramNode*>(node)){
        for(auto& s:p->body) if(hasStringVars(s.get())) return true;
    }else if(auto sub=dynamic_cast<SubroutineNode*>(node)){
        for(auto& s:sub->body) if(hasStringVars(s.get())) return true;
    }else if(auto fn=dynamic_cast<FunctionNode*>(node)){
        for(auto& s:fn->body) if(hasStringVars(s.get())) return true;
    }else if(auto ifn=dynamic_cast<IfNode*>(node)){
        for(auto& s:ifn->then_body) if(hasStringVars(s.get())) return true;
        for(auto& s:ifn->else_body) if(hasStringVars(s.get())) return true;
    }else if(auto don=dynamic_cast<DoNode*>(node)){
        for(auto& s:don->body) if(hasStringVars(s.get())) return true;
    }
    return false;
}

void CodeGenVisitor::visit(ProgramNode& node){
    referenced_labels.clear();
    collectReferencedLabels(&node);
    if(hasStringVars(&node)){
        os<<"#define F77_STR_ASSIGN(dest, src, len) \\\n";
        os<<"    do { \\\n";
        os<<"        strncpy(dest, src, len); \\\n";
        os<<"        int _l = strlen(src); \\\n";
        os<<"        for(int _i = _l; _i < len; _i++) dest[_i] = ' '; \\\n";
        os<<"        dest[len] = '\\0'; \\\n";
        os<<"    } while(0)\n\n";
    }
    std::vector<ASTNode*> main_stmts;
    std::vector<SubroutineNode*> subroutines;
    std::vector<FunctionNode*> functions;
    for(auto& stmt:node.body){
        if(!stmt)continue;
        if(auto sub=dynamic_cast<SubroutineNode*>(stmt.get())){
            subroutines.push_back(sub);
        }else if(auto func=dynamic_cast<FunctionNode*>(stmt.get())){
            functions.push_back(func);
        }else{
            main_stmts.push_back(stmt.get());
        }
    }
    global_subprogram_names.clear();
    for(auto sub:subroutines) global_subprogram_names.insert(toLower(sub->name));
    for(auto func:functions) global_subprogram_names.insert(toLower(func->name));
    for(auto func:functions){
        std::string func_ret=func->returnType;
        if(func_ret.empty()){
            char fc=toupper(func->name[0]);
            func_ret=(fc>='I'&&fc<='N')?"INTEGER":"REAL";
        }
        std::string ret="int";
        if(func_ret=="REAL")ret="float";
        else if(func_ret=="LOGICAL")ret="bool";
        else if(func_ret.find("CHARACTER")==0)ret="char*";
        os<<ret<<" "<<toLower(func->name)<<"(";
        for(size_t i=0;i<func->parameters.size();++i){
            os<<getCType(func->parameters[i])<<"* "<<toLower(func->parameters[i])<<(i+1<func->parameters.size()?", ":"");
        }
        os<<");\n";
    }
    for(auto sub:subroutines){
        os<<"void "<<toLower(sub->name)<<"(";
        for(size_t i=0;i<sub->parameters.size();++i){
            os <<getCType(sub->parameters[i])<<"* "<<toLower(sub->parameters[i])<<(i+1<sub->parameters.size()?", ":"");
        }
        os<<");\n";
    }
    if(!functions.empty()||!subroutines.empty()) os<<"\n";
    scanCommonBlocks(node.body);
    os<<"int main(void){\n";
    if(tab){
        for(const auto& entry:*tab){
            if(entry.obj=="variable"&&entry.lev==0){
                std::string var=entry.id;
                if(!isCommonVar(var)){
                    bool explicitly_declared=false;
                    for(auto s:main_stmts){
                        if(auto tnode=dynamic_cast<TypeDeclNode*>(s)){
                            for(const auto& vn:tnode->variable_names){
                                if(toLower(vn)==toLower(var)){explicitly_declared=true;break;}
                            }
                        }
                        if(explicitly_declared)break;
                    }
                    if(!explicitly_declared){
                        os<<"    "<<(entry.type=="REAL"?"float":(entry.type=="LOGICAL"?"bool":"int"))<<" "<<toLower(var)<<";\n";
                    }
                }
            }
        }
    }
    for(auto stmt:main_stmts){
        stmt->accept(*this);
    }
    os<<"    return 0;\n";
    os<<"}\n\n";
    for(auto func:functions){
        func->accept(*this);
    }
    for(auto sub:subroutines){
        sub->accept(*this);
    }
}

void CodeGenVisitor::visit(SubroutineNode& node){
    in_subprogram=true;
    scanCommonBlocks(node.body);
    current_params.clear();
    for(const auto& p:node.parameters){
        current_params.insert(toLower(p));
    }
    current_func_name="";
    current_func_ret_var="";
    os<<"void "<<toLower(node.name)<<"(";
    for(size_t i=0;i<node.parameters.size();++i){
        os <<getCType(node.parameters[i])<<"* "<<toLower(node.parameters[i])<<(i+1<node.parameters.size()?", ":"");
    }
    os<<"){\n";
    if(tab){
        for(const auto& entry:*tab){
            if(entry.obj=="variable"&&entry.lev>0){
                std::string var=entry.id;
                if(current_params.find(toLower(var))==current_params.end()&&!isCommonVar(var)){
                    bool explicitly_declared=false;
                    for(auto& s:node.body){
                        if(auto tnode=dynamic_cast<TypeDeclNode*>(s.get())){
                            for(const auto& vn:tnode->variable_names){
                                if(toLower(vn)==toLower(var)){explicitly_declared=true;break;}
                            }
                        }
                        if(explicitly_declared)break;
                    }
                    if(!explicitly_declared){
                        os<<"    "<<(entry.type=="REAL"?"float":(entry.type=="LOGICAL"?"bool":"int"))<<" "<<toLower(var)<<";\n";
                    }
                }
            }
        }
    }
    for(auto& stmt:node.body){
        if(stmt) stmt->accept(*this);
    }
    os<<"}\n\n";
    in_subprogram=false;
    current_params.clear();
}

void CodeGenVisitor::visit(FunctionNode& node){
    in_subprogram=true;
    scanCommonBlocks(node.body);
    current_params.clear();
    for(const auto& p:node.parameters){
        current_params.insert(toLower(p));
    }
    current_func_name=toLower(node.name);
    current_func_ret_var=toLower(node.name)+"_val";
    std::string func_ret=node.returnType;
    if(func_ret.empty()){
        char fc=toupper(node.name[0]);
        func_ret=(fc>='I'&&fc<='N')?"INTEGER":"REAL";
    }
    std::string ret="int";
    if(func_ret=="REAL") ret="float";
    else if(func_ret=="LOGICAL") ret="bool";
    os<<ret<<" "<<toLower(node.name)<<"(";
    for(size_t i=0;i<node.parameters.size();++i){
        os<<getCType(node.parameters[i])<<"* "<<toLower(node.parameters[i])<<(i+1<node.parameters.size()? ", " : "");
    }
    os<<"){\n";
    os<<"    "<<ret<<" "<<current_func_ret_var<<";\n";
    if(tab){
        for(const auto& entry:*tab){
            if(entry.obj=="variable"&&entry.lev>0){
                std::string var=entry.id;
                if(current_params.find(toLower(var))==current_params.end()&&!isCommonVar(var)&&toLower(var)!=toLower(node.name)){
                    bool explicitly_declared=false;
                    for(auto& s:node.body){
                        if(auto tnode = dynamic_cast<TypeDeclNode*>(s.get())){
                            for(const auto& vn:tnode->variable_names){
                                if(toLower(vn)==toLower(var)) { explicitly_declared = true; break; }
                            }
                        }
                        if(explicitly_declared) break;
                    }
                    if(!explicitly_declared){
                        os<< "    " << (entry.type == "REAL" ? "float" : (entry.type == "LOGICAL" ? "bool" : "int")) << " " << toLower(var) << ";\n";
                    }
                }
            }
        }
    }
    bool ends_with_return = false;
    if(!node.body.empty()){
        if(dynamic_cast<ReturnNode*>(node.body.back().get())) {
            ends_with_return = true;
        }
    }
    for(auto& stmt:node.body){
        if(stmt) stmt->accept(*this);
    }
    if(!ends_with_return){
        os<<"    return "<<current_func_ret_var<<";\n";
    }
    os<<"}\n\n";
    in_subprogram=false;
    current_params.clear();
    current_func_name="";
    current_func_ret_var="";
}

void CodeGenVisitor::visit(ImplicitNoneNode& node){
    // tidak menghasilkan kode
}

void CodeGenVisitor::visit(TypeDeclNode& node){
    std::vector<std::string> local_vars;
    for(const auto& v:node.variable_names){
        if(current_params.find(toLower(v))==current_params.end()&&global_subprogram_names.find(toLower(v))==global_subprogram_names.end()){
            bool is_common=isCommonVar(v);
            if(!is_common){
                local_vars.push_back(v);
            }
        }
    }
    if(local_vars.empty()) return;

    if(node.line>0)os<<"    // line "<<node.line<<"\n";
    std::string ctype="int";
    std::string char_suffix="";
    if(node.type_name=="REAL")ctype="float";
    else if(node.type_name=="LOGICAL")ctype="bool";
    else if(node.type_name.rfind("CHARACTER",0)==0){
        ctype="char";
        int len=1;
        if(node.type_name.size()>9&&node.type_name[9]=='*'){
            len=std::stoi(node.type_name.substr(10));
        }
        char_suffix="["+std::to_string(len+1)+"]";
        for(const auto& v:local_vars){
            string_lengths[v]=len;
        }
    }
    os<<"    "<<ctype<<" ";
    for(size_t i=0;i<local_vars.size();++i){
        os<<getVarName(local_vars[i])<<char_suffix;
        os<<(i+1<local_vars.size()?", ":"");
    }
    os<<";\n";
    if(ctype=="char"){
        for(const auto& v:local_vars){
            os<<"    "<<getVarName(v)<<"[0] = '\\0';\n";
        }
    }
}

void CodeGenVisitor::visit(ArrayDeclNode& node){
    std::vector<std::string> dims;
    int total_size=1;
    for(const auto& dim:node.dimensions){
        try{
            total_size*=std::stoi(dim.upper_bound);
        }catch(...){
            total_size*=10;
        }
        dims.push_back(dim.upper_bound);
    }
    array_dims[node.array_name]=dims;
    array_dims[toLower(node.array_name)]=dims;

    if(in_subprogram && current_params.find(toLower(node.array_name)) != current_params.end()){
        return;
    }

    if(node.line>0)os<<"    // line "<<node.line<<"\n";
    std::string ctype="int";
    int char_len=-1;
    if(node.type_name=="REAL")ctype="float";
    else if(node.type_name=="LOGICAL")ctype="bool";
    else if(node.type_name.rfind("CHARACTER",0)==0){
        ctype="char";
        char_len=1;
        if(node.type_name.size()>9&&node.type_name[9]=='*'){
            char_len=std::stoi(node.type_name.substr(10));
        }
        string_lengths[node.array_name]=char_len;
        string_lengths[toLower(node.array_name)]=char_len;
    }
    if(char_len>0){
        os<<"    "<<ctype<<" "<<getVarName(node.array_name)<<"["<<total_size<<"]["<<(char_len+1)<<"];\n";
    }else{
        os<<"    "<<ctype<<" "<<getVarName(node.array_name)<<"["<<total_size<<"];\n";
    }
}

void CodeGenVisitor::visit(CommonBlockNode& node){
    // handled in CodeGenVisitor constructor
}

void CodeGenVisitor::visit(IfNode& node){
    if(node.line>0)os<<"    // line "<<node.line<<"\n";
    os<<"    if(";
    if(node.condition)node.condition->accept(*this);
    os<<"){\n";
    for(auto& stmt:node.then_body){
        if(stmt)stmt->accept(*this);
    }
    os<<"    }";
    if(!node.else_body.empty()){
        os<<" else {\n";
        for(auto& stmt:node.else_body){
            if(stmt)stmt->accept(*this);
        }
        os<<"    }";
    }
    os<<"\n";
}

void CodeGenVisitor::visit(DoNode& node){
    if(node.line>0)os<<"    // line "<<node.line<<"\n";
    std::string loop_var = getVarName(node.loop_variable);
    os<<"    for("<<loop_var<<"=";
    if(node.start_expr)node.start_expr->accept(*this);
    os<<"; "<<loop_var<<"<=";
    if(node.end_expr)node.end_expr->accept(*this);
    os<<"; "<<loop_var<<"+=";
    if(node.step_expr)node.step_expr->accept(*this);
    else os<<"1";
    os<<"){\n";
    for(auto& stmt:node.body){
        if(stmt)stmt->accept(*this);
    }
    os<<"    }\n";
}

void CodeGenVisitor::visit(AssignNode& node){
    if(node.line>0)os<<"    // line "<<node.line<<"\n";
    std::string target = toLower(node.target_variable);
    if(!current_func_name.empty()&&target==current_func_name){
        target = current_func_ret_var;
    } else if(in_subprogram && current_params.find(target) != current_params.end()){
        if(!node.index_expressions.empty() || array_dims.find(node.target_variable) != array_dims.end()){
            target = getVarName(target);
        } else {
            target = "(*" + target + ")";
        }
    } else {
        target = getVarName(target);
    }

    if(string_lengths.find(node.target_variable)!=string_lengths.end()){
        int len=string_lengths[node.target_variable];
        os<<"    F77_STR_ASSIGN("<<target;
        if(!node.index_expressions.empty()){
            os<<"[";
            printFlattenedIndex(node.target_variable, node.index_expressions);
            os<<"]";
        }
        os<<", ";
        if(node.expression)node.expression->accept(*this);
        os<<", "<<len<<");\n";
    } else {
        os<<"    "<<target;
        if(!node.index_expressions.empty()){
            os<<"[";
            printFlattenedIndex(node.target_variable, node.index_expressions);
            os<<"]";
        }
        os<<" = ";
        if(node.expression)node.expression->accept(*this);
        os<<";\n";
    }
}

void CodeGenVisitor::visit(PrintNode& node){
    if(node.line>0)os<<"    // line "<<node.line<<"\n";
    os<<"    printf(\"";
    std::string fmt="";
    for(size_t i=0;i<node.expressions.size();++i){
        if(dynamic_cast<StringLiteralNode*>(node.expressions[i].get())){
            fmt+="%s";
        }else if(auto ident = dynamic_cast<IdentifierNode*>(node.expressions[i].get())){
            std::string type = getCType(ident->name);
            if(type=="float") fmt += "%f";
            else if(type=="char") fmt += "%s";
            else fmt += "%d";
        }else{
            fmt += "%d";
        }
        if(i+1<node.expressions.size()) fmt += " ";
    }
    os<<fmt<<"\\n\"";
    for(size_t i=0;i<node.expressions.size();++i){
        os<<", ";
        node.expressions[i]->accept(*this);
    }
    os<<");\n";
}

void CodeGenVisitor::visit(ReadNode& node){
    if(node.line>0)os<<"    // line "<<node.line<<"\n";
    os<<"    scanf(\"";
    std::string fmt="";
    for(size_t i=0;i<node.variables.size();++i){
        std::string type = getCType(node.variables[i]);
        if(type=="float") fmt += "%f";
        else if(type=="char") fmt += "%s";
        else fmt += "%d";
        if(i+1<node.variables.size()) fmt += " ";
    }
    os<<fmt<<"\"";
    for(size_t i=0;i<node.variables.size();++i){
        os<<", ";
        if(getCType(node.variables[i]) != "char") os<<"&";
        os<<getVarName(node.variables[i]);
    }
    os<<");\n";
}

void CodeGenVisitor::visit(CallNode& node){
    if(node.line>0)os<<"    // line "<<node.line<<"\n";
    os<<"    "<<toLower(node.subroutine_name)<<"(";
    for(size_t i=0;i<node.arguments.size();++i){
        if(auto ident = dynamic_cast<IdentifierNode*>(node.arguments[i].get())){
            if(array_dims.find(ident->name) != array_dims.end()){
                os<<getVarName(ident->name);
            } else if(in_subprogram && current_params.find(toLower(ident->name)) != current_params.end()){
                os<<toLower(ident->name);
            } else {
                os<<"&";
                node.arguments[i]->accept(*this);
            }
        } else {
            os<<"&";
            node.arguments[i]->accept(*this);
        }
        os<<(i+1<node.arguments.size()?", ":"");
    }
    os<<");\n";
}

void CodeGenVisitor::visit(GotoNode& node){
    if(node.line>0)os<<"    // line "<<node.line<<"\n";
    os<<"    goto label_"<<node.target_label<<";\n";
}

void CodeGenVisitor::visit(ComputedGotoNode& node){
    if(node.line>0)os<<"    // line "<<node.line<<"\n";
    os<<"    switch(";
    if(node.selector_expr)node.selector_expr->accept(*this);
    os<<"){\n";
    for(size_t i=0;i<node.labels.size();++i){
        os<<"        case "<<(i+1)<<": goto label_"<<node.labels[i]<<"; break;\n";
    }
    os<<"    }\n";
}

void CodeGenVisitor::visit(ReturnNode& node){
    if(node.line>0)os<<"    // line "<<node.line<<"\n";
    if(!current_func_ret_var.empty()){
        os<<"    return "<<current_func_ret_var<<";\n";
    } else if(in_subprogram) {
        os<<"    return;\n";
    } else {
        os<<"    return 0;\n";
    }
}

void CodeGenVisitor::visit(ContinueNode& node){
    if(referenced_labels.count(node.label)>0&&emitted_labels.find(node.label)==emitted_labels.end()){
        if(node.line>0)os<<"    // line "<<node.line<<"\n";
        os<<"label_"<<node.label<<":;\n";
        emitted_labels.insert(node.label);
    }
}

void CodeGenVisitor::visit(BinaryOpNode& node){
    std::string cop=node.op;
    if(cop=="**"){
        os<<"pow(";
        if(node.left)node.left->accept(*this);
        os<<", ";
        if(node.right)node.right->accept(*this);
        os<<")";
        return;
    }
    os<<"(";
    if(node.left)node.left->accept(*this);
    if(cop==".AND.")cop="&&";
    else if(cop==".OR.")cop="||";
    else if(cop==".EQ.")cop="==";
    else if(cop==".NE.")cop="!=";
    else if(cop==".GT.")cop=">";
    else if(cop==".GE.")cop=">=";
    else if(cop==".LT.")cop="<";
    else if(cop==".LE.")cop="<=";
    os<<" "<<cop<<" ";
    if(node.right)node.right->accept(*this);
    os<<")";
}

void CodeGenVisitor::visit(UnaryOpNode& node){
    std::string cop=node.op;
    if(cop==".NOT.")cop="!";
    os<<cop;
    if(node.operand)node.operand->accept(*this);
}

void CodeGenVisitor::visit(IdentifierNode& node){
    std::string lname = toLower(node.name);
    if(!current_func_name.empty()&&lname==current_func_name){
        os<<current_func_ret_var;
    }else if(in_subprogram&&current_params.find(lname)!=current_params.end()){
        if(array_dims.find(node.name)!=array_dims.end()){
            os<<getVarName(node.name);
        }else{
            os<<"(*"<<lname<<")";
        }
    }else{
        os<<getVarName(node.name);
    }
}

void CodeGenVisitor::visit(NumberLiteralNode& node){
    os<<node.value;
}

void CodeGenVisitor::visit(StringLiteralNode& node){
    std::string val=node.value;
    if(val==".TRUE.")os<<"true";
    else if(val==".FALSE.")os<<"false";
    else if(val.size()>=2&&val.front()=='\''&&val.back()=='\''){
        os<<"\""<<val.substr(1,val.size()-2)<<"\"";
    }else{
        os<<val;
    }
}

void CodeGenVisitor::visit(ArrayAccessNode& node){
    std::string upper_name=node.array_name;
    for(auto& c:upper_name)c=toupper(c);

    if(upper_name=="MAX"){
        if(node.indices.size()>=2){
            os<<"((";
            node.indices[0]->accept(*this);
            os<<" > ";
            node.indices[1]->accept(*this);
            os<<") ? (";
            node.indices[0]->accept(*this);
            os<<") : (";
            node.indices[1]->accept(*this);
            os<<"))";
        }
    }else if(upper_name=="MIN"){
        if(node.indices.size()>=2){
            os<<"((";
            node.indices[0]->accept(*this);
            os<<" < ";
            node.indices[1]->accept(*this);
            os<<") ? (";
            node.indices[0]->accept(*this);
            os<<") : (";
            node.indices[1]->accept(*this);
            os<<"))";
        }
    }else if(upper_name=="ABS"||upper_name=="IABS"){
        os<<"abs(";
        if(!node.indices.empty())node.indices[0]->accept(*this);
        os<<")";
    }else if(upper_name=="FABS"){
        os<<"fabs(";
        if(!node.indices.empty())node.indices[0]->accept(*this);
        os<<")";
    }else if(upper_name=="SQRT"){
        os<<"sqrt(";
        if(!node.indices.empty())node.indices[0]->accept(*this);
        os<<")";
    }else if(upper_name=="SIN"){
        os<<"sin(";
        if(!node.indices.empty())node.indices[0]->accept(*this);
        os<<")";
    }else if(upper_name=="COS"){
        os<<"cos(";
        if(!node.indices.empty())node.indices[0]->accept(*this);
        os<<")";
    }else if(upper_name=="TAN"){
        os<<"tan(";
        if(!node.indices.empty())node.indices[0]->accept(*this);
        os<<")";
    }else if(upper_name=="EXP"){
        os<<"exp(";
        if(!node.indices.empty())node.indices[0]->accept(*this);
        os<<")";
    }else if(upper_name=="LOG"||upper_name=="ALOG"){
        os<<"log(";
        if(!node.indices.empty())node.indices[0]->accept(*this);
        os<<")";
    }else if(upper_name=="MOD"){
        if(node.indices.size()>=2){
            os<<"(";
            node.indices[0]->accept(*this);
            os<<" % ";
            node.indices[1]->accept(*this);
            os<<")";
        }
    }else if(array_dims.find(node.array_name)!=array_dims.end()||(in_subprogram&&current_params.find(toLower(node.array_name))!=current_params.end())){
        os<<getVarName(node.array_name)<<"[";
        printFlattenedIndex(node.array_name, node.indices);
        os<<"]";
    }else{
        os<<toLower(node.array_name)<<"(";
        for(size_t i=0; i<node.indices.size(); ++i){
            os<<"&";
            node.indices[i]->accept(*this);
            os<<(i+1 < node.indices.size() ? ", " : "");
        }
        os<<")";
    }
}

void CodeGenVisitor::printFlattenedIndex(const std::string& array_name, const std::vector<std::unique_ptr<ASTNode>>& indices){
    if(array_dims.find(array_name)!=array_dims.end()&&indices.size()>1){
        const auto& dims=array_dims[array_name];
        os<<"(";
        for(size_t i=0;i<indices.size();++i){
            if(i>0) os<<"+";
            os<<"((";
            indices[i]->accept(*this);
            os<<")-1)";
            for(size_t j=0;j<i;++j){
                if(j<dims.size()){
                    os<<"*"<<dims[j];
                }
            }
        }
        os<<")";
    }else{
        os<<"(";
        if(!indices.empty())indices[0]->accept(*this);
        os<<" - 1)";
    }
}