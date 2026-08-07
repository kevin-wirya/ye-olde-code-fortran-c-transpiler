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
#include <vector>

class PrintVisitor : public ASTVisitor {
private:
    std::ostream& os;
    bool show_decorations;
    std::string current_prefix="";
    void printType(const std::string& type) const {
        if(show_decorations&&!type.empty())os<<" (type: "<<type<<")";
    }
    std::string getChildBasePrefix() const {
        if(current_prefix.empty())return "";
        std::string base=current_prefix;
        const std::string branch_mid="├── ";
        const std::string branch_last="└── ";
        if(base.size()>=branch_mid.size()&&base.substr(base.size()-branch_mid.size())==branch_mid){
            base=base.substr(0,base.size()-branch_mid.size())+"│   ";
        }else if(base.size()>=branch_last.size()&&base.substr(base.size()-branch_last.size())==branch_last){
            base=base.substr(0,base.size()-branch_last.size())+"    ";
        }
        return base;
    }

    template <typename T>
    void visitChildren(const std::vector<T>& children, const std::string& base_prefix){
        size_t n=children.size();
        for(size_t i=0;i<n;++i){
            bool is_last=(i==n-1);
            current_prefix=base_prefix+(is_last?"└── ":"├── ");
            if(children[i])children[i]->accept(*this);
        }
    }

public:
    explicit PrintVisitor(std::ostream& outStream = std::cout, bool showDecorations = false)
        :os(outStream),show_decorations(showDecorations){}
    void visit(ProgramNode& node) override {
        os<<current_prefix<<"[ProgramNode] "<<node.name<<"\n";
        std::string base=getChildBasePrefix();
        visitChildren(node.body,base);
    }

    void visit(SubroutineNode& node) override {
        os<<current_prefix<<"[SubroutineNode] "<<node.name<<"(";
        for(size_t i=0;i<node.parameters.size();++i){
            os<<node.parameters[i]<<(i+1<node.parameters.size()?", ":"");
        }
        os<<")\n";
        std::string base=getChildBasePrefix();
        visitChildren(node.body,base);
    }

    void visit(FunctionNode& node) override {
        os<<current_prefix<<"[FunctionNode] "<<node.name<<" -> "<<node.returnType<<"(";
        for(size_t i=0;i<node.parameters.size();++i){
            os<<node.parameters[i]<<(i+1<node.parameters.size()?", ":"");
        }
        os<<")\n";
        std::string base=getChildBasePrefix();
        visitChildren(node.body,base);
    }

    void visit(ImplicitNoneNode& node) override {
        os<<current_prefix<<"[ImplicitNoneNode]\n";
    }

    void visit(TypeDeclNode& node) override {
        os<<current_prefix<<"[TypeDeclNode] "<<node.type_name<<": ";
        for(size_t i=0;i<node.variable_names.size();++i){
            os<<node.variable_names[i]<<(i+1<node.variable_names.size()?", ":"");
        }
        os<<"\n";
    }

    void visit(ArrayDeclNode& node) override {
        os<<current_prefix<<"[ArrayDeclNode] "<<node.type_name<<" "<<node.array_name<<"(";
        for(size_t i=0;i<node.dimensions.size();++i){
            os<<node.dimensions[i].upper_bound<<(i+1<node.dimensions.size()?", ":"");
        }
        os<<")\n";
    }

    void visit(CommonBlockNode& node) override {
        os<<current_prefix<<"[CommonBlockNode] /"<<node.block_name<<"/: ";
        for(size_t i=0;i<node.variable_names.size();++i){
            os<<node.variable_names[i]<<(i+1<node.variable_names.size()?", ":"");
        }
        os<<"\n";
    }

    void visit(IfNode& node) override {
        os<<current_prefix<<"[IfNode]\n";
        std::string base=getChildBasePrefix();
        bool has_else=!node.else_body.empty();
        
        os<<base<<"├── Condition:\n";
        std::string cond_base=base+"│   ";
        if(node.condition){
            current_prefix=cond_base+"└── ";
            node.condition->accept(*this);
        }

        os<<base<<(has_else?"├── Then:\n":"└── Then:\n");
        std::string then_base=base+(has_else?"│   ":"    ");
        visitChildren(node.then_body,then_base);

        if(has_else){
            os<<base<<"└── Else:\n";
            std::string else_base=base+"    ";
            visitChildren(node.else_body,else_base);
        }
    }

    void visit(DoNode& node) override {
        os<<current_prefix<<"[DoNode] Label:"<<node.target_label<<", LoopVar:"<<node.loop_variable<<"\n";
        std::string base=getChildBasePrefix();

        os<<base<<"├── Start:\n";
        std::string start_base=base+"│   ";
        if(node.start_expr){
            current_prefix=start_base+"└── ";
            node.start_expr->accept(*this);
        }

        os<<base<<"├── End:\n";
        std::string end_base=base+"│   ";
        if(node.end_expr){
            current_prefix=end_base+"└── ";
            node.end_expr->accept(*this);
        }

        os<<base<<"└── Body:\n";
        std::string body_base=base+"    ";
        visitChildren(node.body,body_base);
    }

    void visit(AssignNode& node) override {
        os<<current_prefix<<"[AssignNode] "<<node.target_variable;
        if(!node.index_expressions.empty()){
            os<<"(";
            for(size_t i=0;i<node.index_expressions.size();++i){
                if(i>0)os<<", ";
                os<<"<index>";
            }
            os<<")";
        }
        os<<" =\n";
        std::string base=getChildBasePrefix();

        bool has_indices=!node.index_expressions.empty();

        if(has_indices){
            os<<base<<"├── Indices:\n";
            std::string idx_base=base+"│   ";
            visitChildren(node.index_expressions,idx_base);
        }

        os<<base<<"└── Value:\n";
        std::string val_base=base+"    ";
        if(node.expression){
            current_prefix=val_base+"└── ";
            node.expression->accept(*this);
        }
    }

    void visit(PrintNode& node) override {
        os<<current_prefix<<"[PrintNode] Format: "<<node.format<<"\n";
        std::string base=getChildBasePrefix();
        visitChildren(node.expressions,base);
    }

    void visit(ReadNode& node) override {
        os<<current_prefix<<"[ReadNode] Format: "<<node.format<<" Vars: ";
        for(size_t i=0;i<node.variables.size();++i){
            os<<node.variables[i]<<(i+1<node.variables.size()?", ":"");
        }
        os<<"\n";
    }

    void visit(CallNode& node) override {
        os<<current_prefix<<"[CallNode] Subroutine: "<<node.subroutine_name<<"\n";
        std::string base=getChildBasePrefix();
        visitChildren(node.arguments,base);
    }

    void visit(GotoNode& node) override {
        os<<current_prefix<<"[GotoNode] Target: "<<node.target_label<<"\n";
    }

    void visit(ComputedGotoNode& node)override{
        os<<current_prefix<<"[ComputedGotoNode] Labels: ";
        for(size_t i=0;i<node.labels.size();++i){
            os<<node.labels[i]<<(i+1<node.labels.size()?", ":"");
        }
        os<<"\n";
        std::string base=getChildBasePrefix();
        if(node.selector_expr){
            current_prefix=base+"└── Selector:\n";
            std::string sel_base=base+"    ";
            current_prefix=sel_base+"└── ";
            node.selector_expr->accept(*this);
        }
    }

    void visit(ReturnNode& node) override {
        os<<current_prefix<<"[ReturnNode]\n";
    }

    void visit(ContinueNode& node) override {
        os<<current_prefix<<"[ContinueNode] Label: "<<node.label<<"\n";
    }

    void visit(BinaryOpNode& node) override {
        os<<current_prefix<<"[BinaryOpNode] "<<node.op;
        printType(node.inferred_type);
        os<<"\n";
        std::string base=getChildBasePrefix();
        
        std::vector<ASTNode*> operands;
        if(node.left)operands.push_back(node.left.get());
        if(node.right)operands.push_back(node.right.get());
        
        size_t n=operands.size();
        for(size_t i=0;i<n;++i){
            current_prefix=base+(i==n-1?"└── ":"├── ");
            operands[i]->accept(*this);
        }
    }

    void visit(UnaryOpNode& node) override {
        os<<current_prefix<<"[UnaryOpNode] "<<node.op;
        printType(node.inferred_type);
        os<<"\n";
        std::string base=getChildBasePrefix();
        if(node.operand){
            current_prefix=base+"└── ";
            node.operand->accept(*this);
        }
    }

    void visit(IdentifierNode& node) override {
        os<<current_prefix<<"[IdentifierNode] "<<node.name;
        printType(node.inferred_type);
        os<<"\n";
    }

    void visit(NumberLiteralNode& node) override {
        os<<current_prefix<<"[NumberLiteralNode] "<<node.value;
        printType(node.inferred_type);
        os<<"\n";
    }

    void visit(StringLiteralNode& node) override {
        os<<current_prefix<<"[StringLiteralNode] "<<node.value;
        printType(node.inferred_type);
        os<<"\n";
    }

    void visit(ArrayAccessNode& node) override {
        os<<current_prefix<<"[ArrayAccessNode] "<<node.array_name;
        printType(node.inferred_type);
        os<<"\n";
        std::string base=getChildBasePrefix();
        visitChildren(node.indices,base);
    }
};

#endif
