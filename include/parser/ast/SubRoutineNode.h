#ifndef SUBROUTINE_NODE_H
#define SUBROUTINE_NODE_H

#include "ASTNode.h"
#include <string>
#include <vector>
#include <memory>

class SubroutineNode:public ASTNode{
    public:
        std::string name;
        std::vector<std::string> parameters;
        std::vector<std::unique_ptr<ASTNode>> body;
        SubroutineNode(const std::string& sub_name, std::vector<std::string> params, std::vector<std::unique_ptr<ASTNode>> sub_body)
            : name(sub_name), parameters(std::move(params)), body(std::move(sub_body)) {}
        void accept(ASTVisitor& visitor) override{
            visitor.visit(*this);
        }
};

#endif