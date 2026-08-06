#ifndef TYPE_DECL_NODE_H
#define TYPE_DECL_NODE_H

#include "ASTNode.h"
#include <string>
#include <vector>

class TypeDeclNode : public ASTNode{
    public:
        std::string type_name;
        std::vector<std::string> variable_names;
        TypeDeclNode(const std::string& type, std::vector<std::string> vars) 
            : type_name(type), variable_names(std::move(vars)) {}
        void accept(ASTVisitor& visitor) override{
            visitor.visit(*this);
        }
};

#endif