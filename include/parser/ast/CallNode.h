#ifndef CALL_NODE_H
#define CALL_NODE_H

#include "ASTNode.h"
#include <string>
#include <vector>
#include <memory>

class CallNode: public ASTNode{
    public:
        std::string subroutine_name;
        std::vector<std::unique_ptr<ASTNode>> arguments;
        CallNode(const std::string& sub_name, std::vector<std::unique_ptr<ASTNode>> args)
            : subroutine_name(sub_name), arguments(std::move(args)) {}
        void accept(ASTVisitor& visitor) override {
            visitor.visit(*this);
        }
};

#endif