#include "codegen/CodeGenVisitor.h"

void CodeGenVisitor::visit(ProgramNode& node){
    os<<"int main(void){\n";
    for(auto& stmt:node.body){
        if(stmt)stmt->accept(*this);
    }
    os<<"    return 0;\n";
    os<<"}\n";
}

void CodeGenVisitor::visit(SubroutineNode& node){
    os<<"void "<<node.name<<"(";
    for(size_t i=0;i<node.parameters.size();++i){
        os<<"int* "<<node.parameters[i]<<(i+1<node.parameters.size()?", ":"");
    }
    os<<"){\n";
    for(auto& stmt:node.body){
        if(stmt)stmt->accept(*this);
    }
    os<<"}\n\n";
}

void CodeGenVisitor::visit(FunctionNode& node){
    std::string ret="int";
    if(node.returnType=="REAL")ret="float";
    else if(node.returnType=="LOGICAL")ret="bool";
    os<<ret<<" "<<node.name<<"(";
    for(size_t i=0;i<node.parameters.size();++i){
        os<<"int* "<<node.parameters[i]<<(i+1<node.parameters.size()?", ":"");
    }
    os<<"){\n";
    for(auto& stmt:node.body){
        if(stmt)stmt->accept(*this);
    }
    os<<"}\n\n";
}

void CodeGenVisitor::visit(ImplicitNoneNode& node){
    // tidak menghasilkan kode
}

void CodeGenVisitor::visit(TypeDeclNode& node){
    std::string ctype="int";
    if(node.type_name=="REAL")ctype="float";
    else if(node.type_name=="LOGICAL")ctype="bool";
    os<<"    "<<ctype<<" ";
    for(size_t i=0;i<node.variable_names.size();++i){
        os<<node.variable_names[i]<<(i+1<node.variable_names.size()?", ":"");
    }
    os<<";\n";
}

void CodeGenVisitor::visit(ArrayDeclNode& node){
    std::string ctype="int";
    if(node.type_name=="REAL")ctype="float";
    else if(node.type_name=="LOGICAL")ctype="bool";
    int total_size=1;
    for(const auto& dim:node.dimensions){
        try{
            total_size*=std::stoi(dim.upper_bound);
        }catch(...){
            total_size*=10;
        }
    }
    os<<"    "<<ctype<<" "<<node.array_name<<"["<<total_size<<"];\n";
}

void CodeGenVisitor::visit(CommonBlockNode& node){
    // will be handled next
}

void CodeGenVisitor::visit(IfNode& node){
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
    os<<"    for("<<node.loop_variable<<"=";
    if(node.start_expr)node.start_expr->accept(*this);
    os<<"; "<<node.loop_variable<<"<=";
    if(node.end_expr)node.end_expr->accept(*this);
    os<<"; ++"<<node.loop_variable<<"){\n";
    for(auto& stmt:node.body){
        if(stmt)stmt->accept(*this);
    }
    os<<"    }\n";
}

void CodeGenVisitor::visit(AssignNode& node){
    os<<"    "<<node.target_variable;
    if(!node.index_expressions.empty()){
        os<<"[";
        for(size_t i=0;i<node.index_expressions.size();++i){
            node.index_expressions[i]->accept(*this);
            os<<" - 1";
        }
        os<<"]";
    }
    os<<" = ";
    if(node.expression)node.expression->accept(*this);
    os<<";\n";
}

void CodeGenVisitor::visit(PrintNode& node){
    os<<"    printf(";
    for(size_t i=0;i<node.expressions.size();++i){
        if(i>0)os<<", ";
        os<<"\"%d\\n\", ";
        node.expressions[i]->accept(*this);
    }
    os<<");\n";
}

void CodeGenVisitor::visit(ReadNode& node){
    os<<"    scanf(";
    for(size_t i=0;i<node.variables.size();++i){
        os<<"\"%d\", &"<<node.variables[i];
    }
    os<<");\n";
}

void CodeGenVisitor::visit(CallNode& node){
    os<<"    "<<node.subroutine_name<<"(";
    for(size_t i=0;i<node.arguments.size();++i){
        os<<"&";
        node.arguments[i]->accept(*this);
        os<<(i+1<node.arguments.size()?", ":"");
    }
    os<<");\n";
}

void CodeGenVisitor::visit(GotoNode& node){
    os<<"    goto label_"<<node.target_label<<";\n";
}

void CodeGenVisitor::visit(ReturnNode& node){
    os<<"    return;\n";
}

void CodeGenVisitor::visit(ContinueNode& node){
    os<<"label_"<<node.label<<":;\n";
}

void CodeGenVisitor::visit(BinaryOpNode& node){
    os<<"(";
    if(node.left)node.left->accept(*this);
    std::string cop=node.op;
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
    os<<node.name;
}

void CodeGenVisitor::visit(NumberLiteralNode& node){
    os<<node.value;
}

void CodeGenVisitor::visit(StringLiteralNode& node){
    os<<node.value;
}

void CodeGenVisitor::visit(ArrayAccessNode& node){
    os<<node.array_name<<"[";
    for(size_t i=0;i<node.indices.size();++i){
        node.indices[i]->accept(*this);
        os<<" - 1";
    }
    os<<"]";
}