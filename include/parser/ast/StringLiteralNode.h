#ifndef STRING_LITERAL_NODE_H
#define STRING_LITERAL_NODE_H

#include "ExpressionNode.h"
#include <string>

class StringLiteralNode : public ExpressionNode {
    public:
        std::string value;
        StringLiteralNode(const std::string& v) : value(v) {}
        void accept(ASTVisitor& visitor) override{
            visitor.visit(*this);
        }
};

#endif