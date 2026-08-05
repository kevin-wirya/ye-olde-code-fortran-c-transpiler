#ifndef GOTO_NODE_H
#define GOTO_NODE_H

#include "ASTNode.h"
#include <string>
#include <vector>

class GotoNode: public ASTNode{
    public:
        int target_label;
        GotoNode(int target_label): target_label(target_label) {}
        void accept(ASTVisitor& visitor) override{
            visitor.visit(*this);
        }
};

#endif