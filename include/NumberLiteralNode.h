#ifndef NUMBER_LITERAL_NODE_H
#define NUMBER_LITERAL_NODE_H

#include "ExpressionNode.h"
#include <string>

class NumberLiteralNode : public ExpressionNode {
    public:
        std::string value;
        bool is_real;
        NumberLiteralNode(const std::string& v, bool real=false): value(v), is_real(real) {}
        void accept(ASTVisitor& visitor) override{
            visitor.visit(*this);
        }
};

#endif