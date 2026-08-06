#ifndef COMMON_BLOCK_NODE_H
#define COMMON_BLOCK_NODE_H

#include "ASTNode.h"
#include <string>
#include <vector>

class CommonBlockNode : public ASTNode{
    public:
        std::string block_name;
        std::vector<std::string> variable_names;
        CommonBlockNode(const std::string& b_name, std::vector<std::string> vars)
            : block_name(b_name), variable_names(std::move(vars)) {}
        void accept(ASTVisitor& visitor) override{
            visitor.visit(*this);
        }
};
#endif