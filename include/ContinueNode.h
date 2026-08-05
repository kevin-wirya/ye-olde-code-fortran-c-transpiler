#ifndef CONTINUE_NODE_H
#define CONTINUE_NODE_H

#include "ASTNode.h"

class ContinueNode : public ASTNode {
public:
    int label;
    ContinueNode(int l=0): label(l) {}
    void accept(ASTVisitor& visitor) override{visitor.visit(*this);}
};
#endif