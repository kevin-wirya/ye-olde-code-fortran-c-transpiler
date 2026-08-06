#ifndef EXPRESSION_NODE_H
#define EXPRESSION_NODE_H

#include "ASTNode.h"
class ExpressionNode : public ASTNode {
    public:
        virtual ~ExpressionNode() = default;
};

#endif