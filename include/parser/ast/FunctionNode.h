#ifndef FUNCTION_NODE_H
#define FUNCTION_NODE_H

#include "ASTNode.h"
#include <string>
#include <vector>
#include <memory>

class FunctionNode:public ASTNode{
    public:
        std::string name;
        std::string returnType;
        std::vector<std::string> parameters;
        std::vector<std::unique_ptr<ASTNode>> body;
        FunctionNode(const std::string& func_name, const std::string& ret_type, std::vector<std::string> params, std::vector<std::unique_ptr<ASTNode>> func_body)
            : name(func_name), returnType(ret_type), parameters(std::move(params)), body(std::move(func_body)) {}
        void accept(ASTVisitor& visitor) override{
            visitor.visit(*this);
        }
};

#endif