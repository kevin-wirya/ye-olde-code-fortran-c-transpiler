#ifndef AST_VISITOR_H
#define AST_VISITOR_H

class ProgramNode;
class SubroutineNode;
class FunctionNode;
class TypeDeclNode;
class ArrayDeclNode;
class CommonBlockNode;
class IfNode;
class DoNode;
class GotoNode;
class CallNode;
class AssignNode;
class PrintNode;
class ReadNode;
class ReturnNode;
class ContinueNode;
class BinaryOpNode;
class UnaryOpNode;
class IdentifierNode;
class NumberLiteralNode;
class StringLiteralNode;

class ASTVisitor{
    public:
        virtual ~ASTVisitor() = default;
        virtual void visit(ProgramNode& node)=0;
        virtual void visit(SubroutineNode& node)=0;
        virtual void visit(FunctionNode& node)=0;
        virtual void visit(TypeDeclNode& node)=0;
        virtual void visit(ArrayDeclNode& node)=0;
        virtual void visit(CommonBlockNode& node)=0;
        virtual void visit(AssignmentNode& node)=0;
        virtual void visit(PrintNode& node)=0;
        virtual void visit(IfNode& node)=0;
        virtual void visit(DoNode& node)=0;
        virtual void visit(GotoNode& node)=0;
        virtual void visit(CallNode& node)=0;
        virtual void visit(AssignNode& node)=0;
        virtual void visit(PrintNode& node)=0;
        virtual void visit(ReadNode& node)=0;
        virtual void visit(ReturnNode& node)=0;
        virtual void visit(ContinueNode& node)=0;
        virtual void visit(BinaryOpNode& node)=0;
        virtual void visit(UnaryOpNode& node)=0;
        virtual void visit(IdentifierNode& node)=0;
        virtual void visit(NumberLiteralNode& node)=0;
        virtual void visit(StringLiteralNode& node)=0;
};
#endif