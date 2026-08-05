#ifndef ARRAY_ACCESS_NODE_H
#define ARRAY_ACCESS_NODE_H

#include "ExpressionNode.h"
#include <string>
#include <vector>
#include <memory>

class ArrayAccessNode : public ExpressionNode {
    public:
        std::string array_name;
        std::vector<std::unique_ptr<ASTNode>> indices;
        ArrayAccessNode(const std::string& name, std::vector<std::unique_ptr<ASTNode>> idx) 
            : array_name(name), indices(std::move(idx)) {}
        void accept(ASTVisitor& visitor) override{
            visitor.visit(*this);
        }
};

#endif