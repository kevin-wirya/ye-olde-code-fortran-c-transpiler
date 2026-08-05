#ifndef AST_VISITOR_H
#define AST_VISITOR_H

class ProgramNode;
class VarDeclNode;
class AssignmentNode;
class PrintNode;
class IfNode;
class DoLoopNode;
class BinaryOpNode;
class IdentifierNode;
class NumberLiteralNode;
class StringLiteralNode;

class ASTVisitor{
    public:
        virtual ~ASTVisitor() = default;
        virtual void visit(ProgramNode& node)=0;
        virtual void visit(VarDeclNode& node)=0;
        virtual void visit(AssignmentNode& node)=0;
        virtual void visit(PrintNode& node)=0;
        virtual void visit(IfNode& node)=0;
        virtual void visit(DoLoopNode& node)=0;
        virtual void visit(BinaryOpNode& node)=0;
        virtual void visit(IdentifierNode& node)=0;
        virtual void visit(NumberLiteralNode& node)=0;
        virtual void visit(StringLiteralNode& node)=0;
};
#endif