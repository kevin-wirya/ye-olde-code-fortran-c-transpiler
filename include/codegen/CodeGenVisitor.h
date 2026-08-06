#ifndef CODE_GEN_VISITOR_H
#define CODE_GEN_VISITOR_H

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

class CodeGenVisitor: public ASTVisitor{
    private:
        std::ostream& os;
    public:
        explicit CodeGenVisitor(std::ostream& outStream = std::cout):os(outStream){
            os << "#include <stdio.h>\n";
            os << "#include <math.h>\n";
            os << "#include <stdlib.h>\n";
            os << "#include <stdbool.h>\n";
            os << "#include <string.h>\n\n";
        }

        void visit(ProgramNode& node) override;
        void visit(SubroutineNode& node) override;
        void visit(FunctionNode& node) override;
        void visit(ImplicitNoneNode& node) override;
        void visit(TypeDeclNode& node) override;
        void visit(ArrayDeclNode& node) override;
        void visit(CommonBlockNode& node) override;
        void visit(IfNode& node) override;
        void visit(DoNode& node) override;
        void visit(AssignNode& node) override;
        void visit(PrintNode& node) override;
        void visit(ReadNode& node) override;
        void visit(CallNode& node) override;
        void visit(GotoNode& node) override;
        void visit(ContinueNode& node) override;
        void visit(ReturnNode& node) override;
        void visit(BinaryOpNode& node) override;
        void visit(UnaryOpNode& node) override;
        void visit(IdentifierNode& node) override;
        void visit(NumberLiteralNode& node) override;
        void visit(StringLiteralNode& node) override;
        void visit(ArrayAccessNode& node) override;
};

#endif 