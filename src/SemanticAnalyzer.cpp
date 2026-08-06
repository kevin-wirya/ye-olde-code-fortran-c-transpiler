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

SemanticAnalyzer::SemanticAnalyzer():has_implicit_none(false){}

void SemanticAnalyzer::reportError(const std::string& msg){
    errors.push_back(msg);
}

bool SemanticAnalyzer::analyze(ASTNode& root){
    errors.clear();
    root.accept(*this);
    return errors.empty();
}

void SemanticAnalyzer::visit(ProgramNode& node){
    has_implicit_none=false;
    symbol_table.enterScope();
    for(auto& stmt:node.body){
        if(stmt)stmt->accept(*this);
    }
    if(!has_implicit_none)reportError("Semantic Error: IMPLICIT NONE is required in PROGRAM '" + node.name + "'");
    symbol_table.exitScope();
}

void SemanticAnalyzer::visit(SubroutineNode& node){
    has_implicit_none=false;
    symbol_table.enterScope();
    for(const auto& param:node.parameters){
        Symbol sym(param,"UNKNOWN",SymbolKind::VARIABLE);
        symbol_table.declare(sym);
    }
    for(auto& stmt:node.body){
        if(stmt)stmt->accept(*this);
    }
    if(!has_implicit_none)reportError("Semantic Error: IMPLICIT NONE is required in SUBROUTINE '" + node.name + "'");
    symbol_table.exitScope();
}

void SemanticAnalyzer::visit(FunctionNode& node){
    has_implicit_none=false;
    symbol_table.enterScope();
    if(!node.returnType.empty()){
        Symbol fn_sym(node.name,node.returnType,SymbolKind::FUNCTION);
        symbol_table.declare(fn_sym);
    }
    for(const auto& param:node.parameters){
        Symbol sym(param,"UNKNOWN",SymbolKind::VARIABLE);
        symbol_table.declare(sym);
    }
    for(auto& stmt:node.body){
        if(stmt)stmt->accept(*this);
    }
    if(!has_implicit_none)reportError("Semantic Error: IMPLICIT NONE is required in FUNCTION '" + node.name + "'");
    symbol_table.exitScope();
}

void SemanticAnalyzer::visit(ImplicitNoneNode& node){
    (void)node;
    has_implicit_none=true;
}

void SemanticAnalyzer::visit(TypeDeclNode& node){
    for(const auto& var_name:node.variable_names){
        const Symbol* existing=symbol_table.lookupLocal(var_name);
        if(existing&&existing->type!="UNKNOWN")reportError("Redeclaration Error: Variable '" + var_name + "' is already declared in this scope");
        else{
            Symbol sym(var_name,node.type_name,SymbolKind::VARIABLE);
            symbol_table.declare(sym);
        }
    }
}

void SemanticAnalyzer::visit(ArrayDeclNode& node){
    Symbol sym(node.array_name,node.type_name,SymbolKind::ARRAY);
    sym.dimensions=node.dimensions;
    if(!symbol_table.declare(sym))reportError("Redeclaration Error: Array '" + node.array_name + "' is already declared in this scope");
}

void SemanticAnalyzer::visit(CommonBlockNode& node){
    for(const auto& var_name:node.variable_names){
        Symbol sym(var_name,"UNKNOWN",SymbolKind::VARIABLE);
        sym.is_common=true;
        sym.common_block_name=node.block_name;
        symbol_table.declare(sym);
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
    (void)node;
}

void SemanticAnalyzer::visit(StringLiteralNode& node){
    (void)node;
}

void SemanticAnalyzer::visit(IdentifierNode& node){
    const Symbol* sym=symbol_table.lookup(node.name);
    if(!sym)reportError("Semantic Error: Variable '" + node.name + "' used without declaration");
}

void SemanticAnalyzer::visit(ArrayAccessNode& node){
    const Symbol* sym=symbol_table.lookup(node.array_name);
    if(!sym)reportError("Semantic Error: Array or Function '" + node.array_name + "' used without declaration");
    for(auto& idx:node.indices){
        if (idx) idx->accept(*this);
    }
}

void SemanticAnalyzer::visit(BinaryOpNode& node){
    if(node.left)node.left->accept(*this);
    if(node.right)node.right->accept(*this);
}

void SemanticAnalyzer::visit(UnaryOpNode& node){
    if(node.operand)node.operand->accept(*this);
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

void SemanticAnalyzer::visit(CallNode& node){
    for(auto& arg:node.arguments){
        if(arg)arg->accept(*this);
    }
}

void SemanticAnalyzer::visit(ReturnNode& node){
    (void)node;
}
