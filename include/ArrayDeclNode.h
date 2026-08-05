#ifndef ARRAY_DECL_NODE_H
#define ARRAY_DECL_NODE_H

#include "ASTNode.h"
#include <string>
#include <vector>
#include <utility>


struct ArrayDimension{
    int lower_bound;
    int upper_bound;
    ArrayDimension(int upper): lower_bound(1), upper_bound(upper){}
    ArrayDimension(int lower, int upper): lower_bound(lower), upper_bound(upper){}
};

class ArrayDeclNode:public ASTNode{
    public:
        std::string array_name;
        std::string type_name;
        std::vector<ArrayDimension> dimensions;
        ArrayDeclNode(const std::string& name, const std::string& type, std::vector<ArrayDimension> dims)
            : array_name(name), type_name(type), dimensions(std::move(dims)) {}
        void accept(ASTVisitor& visitor) override{
            visitor.visit(*this);
        }
};

#endif
