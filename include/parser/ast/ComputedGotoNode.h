#ifndef COMPUTED_GOTO_NODE_H
#define COMPUTED_GOTO_NODE_H

#include "../ASTNode.h"
#include <vector>
#include <memory>

class ComputedGotoNode:public ASTNode{
    public:
        std::vector<int>labels;
        std::unique_ptr<ASTNode>selector_expr;
        ComputedGotoNode(std::vector<int>lbls,std::unique_ptr<ASTNode>expr)
            :labels(std::move(lbls)),selector_expr(std::move(expr)){}
        void accept(ASTVisitor& visitor) override {
            visitor.visit(*this);
        }
};

#endif
