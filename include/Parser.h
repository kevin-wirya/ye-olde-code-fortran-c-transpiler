#ifndef PARSER_H
#define PARSER_H

#include "TokenType.h"
#include "ASTNode.h"
#include "ProgramNode.h"
#include "SubroutineNode.h"
#include "FunctionNode.h"
#include "TypeDeclNode.h"
#include "ArrayDeclNode.h"
#include "CommonBlockNode.h"
#include "ImplicitNoneNode.h"
#include "IfNode.h"
#include "DoNode.h"
#include "AssignNode.h"
#include "PrintNode.h"
#include "ReadNode.h"
#include "CallNode.h"
#include "GotoNode.h"
#include "ContinueNode.h"
#include "ReturnNode.h"
#include "BinaryOpNode.h"
#include "UnaryOpNode.h"
#include "IdentifierNode.h"
#include "NumberLiteralNode.h"
#include "StringLiteralNode.h"
#include "ArrayAccessNode.h"

#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

class Parser{
    private:
        std::vector<Token> tokens;
        size_t current=0;
    public:
        explicit Parser(std::vector<Token> toks):tokens(std::move(toks)){}
        // helper utils
        Token peek() const;
        Token previous() const;
        bool isAtEnd() const;
        bool check(TokenType type) const;
        bool match(TokenType type);
        Token advance();
        Token consume(TokenType type, const std::string& error_msg);
        // main parsing
        std::unique_ptr<ASTNode> parse();
};

#endif 
