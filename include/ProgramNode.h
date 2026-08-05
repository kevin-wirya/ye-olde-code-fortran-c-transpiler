#ifndef PROGRAM_NODE_H
#define PROGRAM_NODE_H

#include "ASTNode.h"
#include <string>
#include <vector>
#include <memory>

class ProgramNode:public ASTNode{
    public:
        std::string name;
        std::vector<std::unique_ptr<ASTNode>> body;
        ProgramNode(const std::string& prog_name, std::vector<std::unique_ptr<ASTNode>> prog_body)
            : name(prog_name), body(std::move(prog_body)){}   
        void accept(ASTVisitor& visitor) override{
            visitor.visit(*this);
        }
};

#endif