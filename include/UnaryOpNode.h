#ifndef UNARY_OP_NODE_H
#define UNARY_OP_NODE_H

#include "ExpressionNode.h"
#include <string>
#include <memory>

class UnaryOpNode : public ExpressionNode {
    public:
        std::string op;
        std::unique_ptr<ASTNode> operand;
        UnaryOpNode(const std::string& o, std::unique_ptr<ASTNode> operand) 
            : op(o), operand(std::move(operand)) {}
        void accept(ASTVisitor& visitor) override{
            visitor.visit(*this);
        }
};

#endif
