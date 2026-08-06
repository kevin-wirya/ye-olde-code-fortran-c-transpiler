#ifndef PRINT_NODE_H
#define PRINT_NODE_H

#include "ASTNode.h"
#include <string>
#include <vector>
#include <memory>

class PrintNode: public ASTNode{
    public:
        std::string format;
        std::vector<std::unique_ptr<ASTNode>> expressions;
        PrintNode(const std::string& fmt, std::vector<std::unique_ptr<ASTNode>> exprs) 
            : format(fmt), expressions(std::move(exprs)) {}
        void accept(ASTVisitor& visitor) override{
            visitor.visit(*this);
        }
};

#endif