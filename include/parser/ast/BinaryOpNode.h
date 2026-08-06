#ifndef BINARY_OP_NODE_H
#define BINARY_OP_NODE_H

#include "ExpressionNode.h"
#include <string>
#include <memory>

class BinaryOpNode : public ExpressionNode {
    public:
        std::unique_ptr<ASTNode> left;
        std::string op;
        std::unique_ptr<ASTNode> right;
        BinaryOpNode(std::unique_ptr<ASTNode> l, const std::string& o, std::unique_ptr<ASTNode> r)
            : left(std::move(l)), op(o), right(std::move(r)) {}
        void accept(ASTVisitor& visitor) override{
            visitor.visit(*this);
        }
};

#endif