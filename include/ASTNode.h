#ifndef AST_NODE_H
#define AST_NODE_H

#include "ASTVisitor.h"

class ASTNode{
    public:
        virtual ~ASTNode()=default;
        virtual void accept(ASTVisitor& visitor)=0;
};

#endif