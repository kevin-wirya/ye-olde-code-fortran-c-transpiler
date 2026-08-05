#ifndef IDENTIFIER_NODE_H
#define IDENTIFIER_NODE_H

#include "ExpressionNode.h"
#include <string>

class IdentifierNode : public ExpressionNode {
    public:
        std::string name;
        IdentifierNode(const std::string& n): name(n) {}
        void accept(ASTVisitor& visitor) override{
            visitor.visit(*this);
        }
};

#endif