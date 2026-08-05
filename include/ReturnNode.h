#ifndef RETURN_NODE_H
#define RETURN_NODE_H
#include "ASTNode.h"

class ReturnNode : public ASTNode {
public:
    ReturnNode()=default;
    void accept(ASTVisitor& visitor) override{visitor.visit(*this);}
};
#endif