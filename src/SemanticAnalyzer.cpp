#include "SemanticAnalyzer.h"
#include "ProgramNode.h"
#include "SubRoutineNode.h"
#include "FunctionNode.h"
#include "ImplicitNoneNode.h"
#include "TypeDeclNode.h"
#include "ArrayDeclNode.h"
#include "CommonBlockNode.h"
#include "AssignNode.h"
#include "IfNode.h"
#include "DoNode.h"
#include "GotoNode.h"
#include "ContinueNode.h"
#include "NumberLiteralNode.h"
#include "StringLiteralNode.h"
#include "IdentifierNode.h"
#include "ArrayAccessNode.h"
#include "BinaryOpNode.h"
#include "UnaryOpNode.h"
#include "PrintNode.h"
#include "ReadNode.h"
#include "CallNode.h"
#include "ReturnNode.h"
#include <iostream>

SemanticAnalyzer::SemanticAnalyzer():has_implicit_none(false){
    initPredefinedSymbolTables();
}

void SemanticAnalyzer::initPredefinedSymbolTables(){
    tab.clear();
    btab.clear();
    atab.clear();
    current_level=0;
    current_btab_idx=0;
    current_offset=0;
    tab.push_back({0,"INTEGER","type","INTEGER",0,1,0,0,0});
    tab.push_back({1,"REAL","type","REAL",0,1,0,0,0});
    tab.push_back({2,"LOGICAL","type","LOGICAL",0,1,0,0,0});
    tab.push_back({3,"CHARACTER","type","CHARACTER",0,1,0,0,0});
}

void SemanticAnalyzer::reportError(const std::string& msg){
    errors.push_back(msg);
}

std::string SemanticAnalyzer::getInferredType(ASTNode* node){
    if(!node)return "";
    auto expr=dynamic_cast<ExpressionNode*>(node);
    if(expr)return expr->inferred_type;
    return "";
}

bool SemanticAnalyzer::analyze(ASTNode& root){
    errors.clear();
    initPredefinedSymbolTables();
    root.accept(*this);
    return errors.empty();
}

void SemanticAnalyzer::printSymbolTables(std::ostream& os)const{
    os<<"\n====== SYMBOL TABLES (tab, btab, atab) ======\n\n";
    os<<"[tab - Identifier Table]\n";
    os<<"idx   id           obj          type      ref   nrm   lev   adr   link\n";
    os<<"-----------------------------------------------------------------------\n";
    for(const auto& entry:tab){
        char buf[256];
        snprintf(buf,sizeof(buf),"%-5d %-12s %-12s %-9s %-5d %-5d %-5d %-5d %-5d\n",
                 entry.idx, entry.id.c_str(), entry.obj.c_str(), entry.type.c_str(),
                 entry.ref, entry.nrm, entry.lev, entry.adr, entry.link);
        os<<buf;
    }
    os<<"\n";
    os<<"[btab - Block Table]\n";
    os<<"idx   name         last  lpar  psze  vsze\n";
    os<<"-----------------------------------------\n";
    for(const auto& entry:btab){
        char buf[256];
        snprintf(buf,sizeof(buf),"%-5d %-12s %-5d %-5d %-5d %-5d\n",
                 entry.idx, entry.name.c_str(), entry.last, entry.lpar, entry.psze, entry.vsze);
        os<<buf;
    }
    os<<"\n";
    os<<"[atab - Array Table]\n";
    os<<"idx   name         xtyp      etyp      eref  low   high  elsz  size\n";
    os<<"-------------------------------------------------------------------\n";
    if(atab.empty()){
        os<<"(empty)\n";
    }else{
        for(const auto& entry:atab){
            char buf[256];
            snprintf(buf,sizeof(buf),"%-5d %-12s %-9s %-9s %-5d %-5d %-5d %-5d %-5d\n",
                     entry.idx, entry.name.c_str(), entry.xtyp.c_str(), entry.etyp.c_str(),
                     entry.eref, entry.low, entry.high, entry.elsz, entry.size);
            os<<buf;
        }
    }
    os<<"===============================================\n";
}

void SemanticAnalyzer::visit(ProgramNode& node){
    has_implicit_none=false;
    symbol_table.enterScope();
    int b_idx=static_cast<int>(btab.size());
    btab.push_back({b_idx, node.name, 0, 0, 0, 0});
    int t_idx=static_cast<int>(tab.size());
    tab.push_back({t_idx, node.name, "program", "VOID", b_idx, 1, current_level, 0, 0});
    current_btab_idx=b_idx;
    for(auto& stmt:node.body){
        if(stmt)stmt->accept(*this);
    }
    if(!has_implicit_none)reportError("Semantic Error: IMPLICIT NONE is required in PROGRAM '" + node.name + "'");
    symbol_table.exitScope();
}

void SemanticAnalyzer::visit(SubroutineNode& node){
    has_implicit_none=false;
    const Symbol* existing = symbol_table.lookup(node.name);
    if(existing && existing->arity > 0 && existing->arity != static_cast<int>(node.parameters.size())){
        reportError("Arity Error: Subroutine '" + node.name + "' was called with " +
                    std::to_string(existing->arity) + " argument(s), but defined with " +
                    std::to_string(node.parameters.size()) + " parameter(s)");
    }
    Symbol sub_sym(node.name,"VOID",SymbolKind::SUBROUTINE);
    sub_sym.arity=static_cast<int>(node.parameters.size());
    symbol_table.declare(sub_sym);
    symbol_table.enterScope();
    int b_idx=static_cast<int>(btab.size());
    btab.push_back({b_idx, node.name, 0, 0, 0, 0});
    int t_idx=static_cast<int>(tab.size());
    tab.push_back({t_idx, node.name, "subroutine", "VOID", b_idx, 1, current_level, 0, 0});
    int old_btab=current_btab_idx;
    current_btab_idx=b_idx;
    current_level++;
    int old_offset=current_offset;
    current_offset=0;
    for(const auto& param:node.parameters){
        Symbol sym(param,"UNKNOWN",SymbolKind::VARIABLE);
        symbol_table.declare(sym);
    }
    for(auto& stmt:node.body){
        if(stmt)stmt->accept(*this);
    }
    if(!has_implicit_none){
        reportError("Semantic Error: IMPLICIT NONE is required in SUBROUTINE '" + node.name + "'");
    }
    current_level--;
    current_btab_idx=old_btab;
    current_offset=old_offset;
    symbol_table.exitScope();
}

void SemanticAnalyzer::visit(FunctionNode& node){
    has_implicit_none=false;
    Symbol fn_sym(node.name,node.returnType.empty()?"UNKNOWN":node.returnType,SymbolKind::FUNCTION);
    fn_sym.arity=static_cast<int>(node.parameters.size());
    symbol_table.declare(fn_sym);
    symbol_table.enterScope();
    int b_idx=static_cast<int>(btab.size());
    btab.push_back({b_idx, node.name, 0, 0, 0, 0});
    int t_idx=static_cast<int>(tab.size());
    tab.push_back({t_idx, node.name, "function", node.returnType.empty()?"UNKNOWN":node.returnType, b_idx, 1, current_level, 0, 0});
    int old_btab=current_btab_idx;
    current_btab_idx=b_idx;
    current_level++;
    int old_offset=current_offset;
    current_offset=0;
    for(const auto& param:node.parameters){
        Symbol sym(param,"UNKNOWN",SymbolKind::VARIABLE);
        symbol_table.declare(sym);
    }
    for(auto& stmt:node.body){
        if(stmt)stmt->accept(*this);
    }
    if(!has_implicit_none)reportError("Semantic Error: IMPLICIT NONE is required in FUNCTION '" + node.name + "'");
    current_level--;
    current_btab_idx=old_btab;
    current_offset=old_offset;
    symbol_table.exitScope();
}

void SemanticAnalyzer::visit(ImplicitNoneNode& node){
    (void)node;
    has_implicit_none=true;
}

void SemanticAnalyzer::visit(TypeDeclNode& node){
    for(const auto& var_name:node.variable_names){
        const Symbol* existing=symbol_table.lookupLocal(var_name);
        if(existing){
            if(existing->type!="UNKNOWN")reportError("Redeclaration Error: Variable '" + var_name + "' is already declared in this scope");
            else symbol_table.updateType(var_name,node.type_name);
        }else{
            Symbol sym(var_name,node.type_name,SymbolKind::VARIABLE);
            symbol_table.declare(sym);
        }
        int t_idx=static_cast<int>(tab.size());
        int prev_link=btab.empty()?0:btab[current_btab_idx].last;
        tab.push_back({t_idx, var_name, "variable", node.type_name, 0, 1, current_level, current_offset, prev_link});
        if(!btab.empty()){
            btab[current_btab_idx].last=t_idx;
            btab[current_btab_idx].vsze+=4;
        }
        current_offset+=4;
    }
}

void SemanticAnalyzer::visit(ArrayDeclNode& node){
    Symbol sym(node.array_name,node.type_name,SymbolKind::ARRAY);
    sym.dimensions=node.dimensions;
    if(!symbol_table.declare(sym))reportError("Redeclaration Error: Array '" + node.array_name + "' is already declared in this scope");
    int total_elems=1;
    int high_val=1;
    for(const auto& dim:node.dimensions){
        int ub=10;
        try{ ub=std::stoi(dim.upper_bound); }catch(...){ ub=10; }
        total_elems*=ub;
        high_val=ub;
    }
    int a_idx=static_cast<int>(atab.size());
    atab.push_back({a_idx, node.array_name, "INTEGER", node.type_name, 0, 1, high_val, 4, 4*total_elems});
    int t_idx=static_cast<int>(tab.size());
    int prev_link=btab.empty()?0:btab[current_btab_idx].last;
    tab.push_back({t_idx, node.array_name, "array", node.type_name, a_idx, 1, current_level, current_offset, prev_link});
    if(!btab.empty()){
        btab[current_btab_idx].last=t_idx;
        btab[current_btab_idx].vsze+=4*total_elems;
    }
    current_offset+=4*total_elems;
}

void SemanticAnalyzer::visit(CommonBlockNode& node){
    for(const auto& var_name:node.variable_names){
        Symbol sym(var_name,"UNKNOWN",SymbolKind::VARIABLE);
        sym.is_common=true;
        sym.common_block_name=node.block_name;
        symbol_table.declare(sym);
    }
    auto it=global_common_blocks.find(node.block_name);
    if(it==global_common_blocks.end()){
        global_common_blocks[node.block_name]={node.block_name,node.variable_names};
    }else{
        if(it->second.variable_names.size()!=node.variable_names.size()){
            reportError("Semantic Error: COMMON block '/" + node.block_name + "/' variable count mismatch across subprogram units");
        }
    }
}

void SemanticAnalyzer::visit(AssignNode& node){
    const Symbol* sym=symbol_table.lookup(node.target_variable);
    if(!sym)reportError("Semantic Error: Target variable '" + node.target_variable + "' used without declaration");
    for(auto& idx:node.index_expressions){
        if(idx)idx->accept(*this);
    }
    if(node.expression){
        node.expression->accept(*this);
    }
}

void SemanticAnalyzer::visit(IfNode& node){
    if(node.condition)node.condition->accept(*this);
    for(auto& stmt:node.then_body){
        if(stmt)stmt->accept(*this);
    }
    for(auto& stmt:node.else_body){
        if(stmt)stmt->accept(*this);
    }
}

void SemanticAnalyzer::visit(DoNode& node){
    const Symbol* sym=symbol_table.lookup(node.loop_variable);
    if(!sym)reportError("Semantic Error: DO loop variable '" + node.loop_variable + "' is not declared");
    if(node.start_expr)node.start_expr->accept(*this);
    if(node.end_expr)node.end_expr->accept(*this);
    if(node.step_expr)node.step_expr->accept(*this);
    symbol_table.enterScope();
    for(auto& stmt:node.body){
        if(stmt)stmt->accept(*this);
    }
    symbol_table.exitScope();
}

void SemanticAnalyzer::visit(GotoNode& node){
    (void)node;
}

void SemanticAnalyzer::visit(ContinueNode& node){
    (void)node;
}

void SemanticAnalyzer::visit(NumberLiteralNode& node){
    node.inferred_type=node.is_real?"REAL":"INTEGER";
}

void SemanticAnalyzer::visit(StringLiteralNode& node){
    (void)node;
    node.inferred_type="STRING";
}

void SemanticAnalyzer::visit(IdentifierNode& node){
    const Symbol* sym=symbol_table.lookup(node.name);
    if(!sym){
        reportError("Semantic Error: Variable '" + node.name + "' used without declaration");
        node.inferred_type="UNKNOWN";
    }else{
        node.inferred_type=sym->type;
    }
}

void SemanticAnalyzer::visit(ArrayAccessNode& node) {
    const Symbol* sym=symbol_table.lookup(node.array_name);
    if(!sym){
        reportError("Semantic Error: Array or Function '" + node.array_name + "' used without declaration");
        node.inferred_type="UNKNOWN";
    }else{
        node.inferred_type=sym->type;
    }
    for(auto& idx:node.indices){
        if(idx)idx->accept(*this);
    }
}

void SemanticAnalyzer::visit(BinaryOpNode& node){
    if(node.left)node.left->accept(*this);
    if(node.right)node.right->accept(*this);
    std::string left_type=getInferredType(node.left.get());
    std::string right_type=getInferredType(node.right.get());
    if(node.op==".AND."||node.op==".OR."){
        if((!left_type.empty()&&left_type!="LOGICAL")||(!right_type.empty()&&right_type!="LOGICAL")){
            reportError("Type Error: Logical operator '" + node.op + "' requires LOGICAL operands");
        }
        node.inferred_type="LOGICAL";
    }else if(node.op==".EQ."||node.op==".NE."||node.op==".LT."|| 
            node.op==".LE."||node.op==".GT."||node.op==".GE."){
        node.inferred_type="LOGICAL";
    }else{
        if(left_type=="LOGICAL"||right_type=="LOGICAL"||
            left_type=="STRING"||right_type=="STRING"){
            reportError("Type Error: Invalid operands for arithmetic operation '" + node.op + "'");
            node.inferred_type="UNKNOWN";
        }else if(left_type=="REAL"||right_type=="REAL"){
            node.inferred_type="REAL";
        }else{
            node.inferred_type="INTEGER";
        }
    }
}


void SemanticAnalyzer::visit(UnaryOpNode& node){
    if(node.operand)node.operand->accept(*this);
    if(node.op==".NOT."){
        node.inferred_type="LOGICAL";
    }else{
        node.inferred_type=getInferredType(node.operand.get());
    }
}

void SemanticAnalyzer::visit(PrintNode& node){
    for(auto& expr:node.expressions){
        if(expr)expr->accept(*this);
    }
}

void SemanticAnalyzer::visit(ReadNode& node){
    for(const auto& var_name:node.variables){
        const Symbol* sym=symbol_table.lookup(var_name);
        if(!sym)reportError("Semantic Error: Variable '" + var_name + "' in READ statement used without declaration");
    }
}

void SemanticAnalyzer::visit(CallNode& node) {
    for(auto& arg:node.arguments){
        if(arg)arg->accept(*this);
    }
    const Symbol* sym=symbol_table.lookup(node.subroutine_name);
    if(!sym){
        Symbol sub_sym(node.subroutine_name,"VOID",SymbolKind::SUBROUTINE);
        sub_sym.arity=static_cast<int>(node.arguments.size());
        symbol_table.declare(sub_sym);
    }else{
        if(sym->arity>0&&node.arguments.size()!=static_cast<size_t>(sym->arity)){
            reportError("Arity Error: Subroutine '" + node.subroutine_name + "' expects " + 
                        std::to_string(sym->arity) + " arguments, but got " + 
                        std::to_string(node.arguments.size()));
        }
    }
}

void SemanticAnalyzer::visit(ReturnNode& node){
    (void)node;
}
