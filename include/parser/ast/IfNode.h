#ifndef IF_NODE_H
#define IF_NODE_H

#include "ASTNode.h"
#include <memory>
#include <vector>

class IfNode: public ASTNode{
    public:
        std::unique_ptr<ASTNode> condition;
        std::vector<std::unique_ptr<ASTNode>> then_body;
        std::vector<std::unique_ptr<ASTNode>> else_body;
        IfNode(std::unique_ptr<ASTNode> cond, std::vector<std::unique_ptr<ASTNode>> then_b, std::vector<std::unique_ptr<ASTNode>> else_b = {})
            : condition(std::move(cond)), then_body(std::move(then_b)), else_body(std::move(else_b)) {}
        void accept(ASTVisitor& visitor) override{
            visitor.visit(*this);
        }
};

#endif