#ifndef DO_NODE_H
#define DO_NODE_H

#include "ASTNode.h"
#include <string>
#include <vector>
#include <memory>

class DoNode:public ASTNode{
    public:
        int target_label;
        std::string loop_variable;
        std::unique_ptr<ASTNode> start_expr;
        std::unique_ptr<ASTNode> end_expr;
        std::unique_ptr<ASTNode> step_expr;
        std::vector<std::unique_ptr<ASTNode>> body;
        DoNode(int label, const std::string& var, std::unique_ptr<ASTNode> start, std::unique_ptr<ASTNode> end, std::unique_ptr<ASTNode> step, std::vector<std::unique_ptr<ASTNode>> do_body)
            :target_label(label), loop_variable(var), start_expr(std::move(start)), end_expr(std::move(end)), step_expr(std::move(step)), body(std::move(do_body)) {}
        void accept(ASTVisitor& visitor) override{
            visitor.visit(*this);
        }
};

#endif
