#ifndef ASSIGN_NODE_H
#define ASSIGN_NODE_H

#include "ASTNode.h"
#include <string>
#include <vector>
#include <memory>

class AssignNode: public ASTNode{
    public:
        std::string target_variable;
        std::unique_ptr<ASTNode> expression;
        std::vector<std::unique_ptr<ASTNode>> index_expressions;
        AssignNode(const std::string& target, std::unique_ptr<ASTNode> expr)
            : target_variable(target), expression(std::move(expr)) {}
        AssignNode(const std::string& target, std::vector<std::unique_ptr<ASTNode>> indices, std::unique_ptr<ASTNode> expr)
            : target_variable(target), expression(std::move(expr)), index_expressions(std::move(indices)) {}
        void accept(ASTVisitor& visitor) override{
            visitor.visit(*this);
        }
};

#endif