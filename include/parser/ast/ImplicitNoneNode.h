#ifndef IMPLICIT_NONE_NODE_H
#define IMPLICIT_NONE_NODE_H

#include "ASTNode.h"

class ImplicitNoneNode : public ASTNode {
public:
    ImplicitNoneNode() = default;
    void accept(ASTVisitor& visitor) override{
        visitor.visit(*this);
    }
};

#endif
