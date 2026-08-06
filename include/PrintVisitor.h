#ifndef PRINT_VISITOR_H
#define PRINT_VISITOR_H

#include "ASTVisitor.h"
#include "ProgramNode.h"
#include "SubroutineNode.h"
#include "FunctionNode.h"
#include "TypeDeclNode.h"
#include "ArrayDeclNode.h"
#include "CommonBlockNode.h"
#include "ImplicitNoneNode.h"
#include "IfNode.h"
#include "DoNode.h"
#include "AssignNode.h"
#include "PrintNode.h"
#include "ReadNode.h"
#include "CallNode.h"
#include "GotoNode.h"
#include "ContinueNode.h"
#include "ReturnNode.h"
#include "BinaryOpNode.h"
#include "UnaryOpNode.h"
#include "IdentifierNode.h"
#include "NumberLiteralNode.h"
#include "StringLiteralNode.h"
#include "ArrayAccessNode.h"

#include <iostream>
#include <string>

class PrintVisitor : public ASTVisitor {
private:
    int indent=0;
    std::ostream& os;
    void printIndent() const {
        for(int i=0;i<indent;++i)os<<"  ";
    }

public:
    explicit PrintVisitor(std::ostream& outStream = std::cout):os(outStream){}
    void visit(ProgramNode& node) override {
        printIndent();
        os<<"[ProgramNode] "<<node.name<<"\n";
        indent++;
        for(auto& stmt:node.body){
            if(stmt)stmt->accept(*this);
        }
        indent--;
    }

    void visit(SubroutineNode& node) override {
        printIndent();
        os<<"[SubroutineNode] "<<node.name<<"(";
        for(size_t i=0;i<node.parameters.size();++i){
            os<<node.parameters[i]<<(i+1<node.parameters.size()?", ":"");
        }
        os<<")\n";
        indent++;
        for(auto& stmt:node.body){
            if(stmt)stmt->accept(*this);
        }
        indent--;
    }

    void visit(FunctionNode& node) override {
        printIndent();
        os<<"[FunctionNode] "<<node.name<<" -> "<<node.returnType<<"(";
        for(size_t i=0;i<node.parameters.size();++i){
            os<<node.parameters[i]<<(i+1<node.parameters.size()?", ":"");
        }
        os<<")\n";
        indent++;
        for(auto& stmt:node.body){
            if(stmt)stmt->accept(*this);
        }
        indent--;
    }

    void visit(ImplicitNoneNode& node) override {
        printIndent();
        os<<"[ImplicitNoneNode]\n";
    }

    void visit(TypeDeclNode& node) override {
        printIndent();
        os<<"[TypeDeclNode] "<<node.type_name<<": ";
        for(size_t i=0;i<node.variable_names.size();++i){
            os<<node.variable_names[i]<<(i+1<node.variable_names.size()?", ":"");
        }
        os<<"\n";
    }

    void visit(ArrayDeclNode& node) override {
        printIndent();
        os<<"[ArrayDeclNode] ";
        for(size_t i=0;i<node.dimensions.size();++i){
            os<<node.dimensions[i].upper_bound<<(i+1<node.dimensions.size()?", ":"");
        }
        os<<")\n";
    }

    void visit(CommonBlockNode& node) override {
        printIndent();
        os<<"[CommonBlockNode] /"<<node.block_name<<"/: ";
        for(size_t i=0;i<node.variable_names.size();++i){
            os<<node.variable_names[i]<<(i+1<node.variable_names.size()?", ":"");
        }
        os<<"\n";
    }

    void visit(IfNode& node) override {
        printIndent();
        os<<"[IfNode]\n";
        indent++;
        printIndent();
        os<<"Condition:\n";
        indent++;
        if(node.condition)node.condition->accept(*this);
        indent--;
        printIndent();
        os<<"Then:\n";
        indent++;
        for(auto& stmt:node.then_body){
            if(stmt)stmt->accept(*this);
        }
        indent--;
        if(!node.else_body.empty()){
            printIndent();
            os<<"Else:\n";
            indent++;
            for(auto& stmt:node.else_body){
                if(stmt)stmt->accept(*this);
            }
            indent--;
        }
        indent--;
    }

    void visit(DoNode& node) override {
        printIndent();
        os<<"[DoNode] Label:"<< node.target_label <<", LoopVar:"<< node.loop_variable <<"\n";
        indent++;
        printIndent();
        os<<"Start:\n";
        indent++;
        if(node.start_expr)node.start_expr->accept(*this);
        indent--;
        printIndent();
        os<<"End:\n";
        indent++;
        if(node.end_expr)node.end_expr->accept(*this);
        indent--;
        printIndent();
        os<<"Body:\n";
        indent++;
        for(auto& stmt:node.body){
            if(stmt)stmt->accept(*this);
        }
        indent--;
        indent--;
    }

    void visit(AssignNode& node) override {
        printIndent();
        os<<"[AssignNode] "<<node.target_variable<<" =\n";
        indent++;
        if(node.expression)node.expression->accept(*this);
        indent--;
    }

    void visit(PrintNode& node) override {
        printIndent();
        os<<"[PrintNode] Format: "<<node.format<<"\n";
        indent++;
        for(auto& expr:node.expressions){
            if(expr)expr->accept(*this);
        }
        indent--;
    }

    void visit(ReadNode& node) override {
        printIndent();
        os<<"[ReadNode] Format: "<<node.format<<" Vars: ";
        for(size_t i=0;i<node.variables.size();++i){
            os<<node.variables[i]<<(i+1<node.variables.size()?", ":"");
        }
        os<<"\n";
    }

    void visit(CallNode& node) override {
        printIndent();
        os<<"[CallNode] Subroutine: "<<node.subroutine_name<<"\n";
        indent++;
        for(auto& arg:node.arguments){
            if(arg)arg->accept(*this);
        }
        indent--;
    }

    void visit(GotoNode& node) override {
        printIndent();
        os<<"[GotoNode] Target: "<<node.target_label<<"\n";
    }

    void visit(ReturnNode& node) override {
        printIndent();
        os<<"[ReturnNode]\n";
    }

    void visit(ContinueNode& node) override {
        printIndent();
        os<<"[ContinueNode] Label: "<< node.label <<"\n";
    }

    void visit(BinaryOpNode& node) override {
        printIndent();
        os<<"[BinaryOpNode] "<< node.op <<"\n";
        indent++;
        if(node.left)node.left->accept(*this);
        if(node.right)node.right->accept(*this);
        indent--;
    }

    void visit(UnaryOpNode& node) override {
        printIndent();
        os<<"[UnaryOpNode] "<< node.op <<"\n";
        indent++;
        if(node.operand)node.operand->accept(*this);
        indent--;
    }

    void visit(IdentifierNode& node) override {
        printIndent();
        os<<"[IdentifierNode] "<< node.name <<"\n";
    }

    void visit(NumberLiteralNode& node) override {
        printIndent();
        os<<"[NumberLiteralNode] "<< node.value <<"\n";
    }

    void visit(StringLiteralNode& node) override {
        printIndent();
        os<<"[StringLiteralNode] "<<node.value<<"\n";
    }

    void visit(ArrayAccessNode& node) override {
        printIndent();
        os<<"[ArrayAccessNode] "<<node.array_name<<"\n";
        indent++;
        for(auto& idx:node.indices){
            if(idx)idx->accept(*this);
        }
        indent--;
    }
};

#endif
