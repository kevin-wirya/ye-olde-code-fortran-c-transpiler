#ifndef READ_NODE_H
#define READ_NODE_H

#include "ASTNode.h"
#include <string>
#include <vector>
#include <memory>

class ReadNode: public ASTNode{
    public:
        std::string format;
        std::vector<std::string> variables;
        ReadNode(const std::string& fmt, std::vector<std::string> vars) 
            : format(fmt), variables(std::move(vars)) {}
        void accept(ASTVisitor& visitor) override{
            visitor.visit(*this);
        }
};

#endif
