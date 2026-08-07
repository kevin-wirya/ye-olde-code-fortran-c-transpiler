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
#include "ComputedGotoNode.h"
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

struct ParseError:public std::runtime_error{using std::runtime_error::runtime_error;};
class Parser{
    private:
        std::vector<Token> tokens;
        size_t current=0;
        std::vector<std::string> syntax_errors;
    public:
        explicit Parser(std::vector<Token> toks):tokens(std::move(toks)){}
        const std::vector<std::string>& getErrors() const{return syntax_errors;}
        bool hasErrors() const{return !syntax_errors.empty();}
        void synchronize();
        // helper utils
        Token peek() const;
        TokenType peekNextType() const;
        Token previous() const;
        bool isAtEnd() const;
        bool check(TokenType type) const;
        bool match(TokenType type);
        Token advance();
        Token consume(TokenType type, const std::string& error_msg);
        // main parsing
        std::unique_ptr<ASTNode> parse();
        std::unique_ptr<ASTNode> parseProgram();
        std::unique_ptr<ASTNode> parseSubroutine();
        std::unique_ptr<ASTNode> parseFunction();
        // declaration parsing
        std::unique_ptr<ASTNode> parseImplicitNone();
        std::unique_ptr<ASTNode> parseDeclaration();
        std::unique_ptr<ASTNode> parseCommonBlock();     
        // control flow parsing
        std::unique_ptr<ASTNode> parseIf();
        std::unique_ptr<ASTNode> parseDo();
        std::unique_ptr<ASTNode> parseGoto();
        std::unique_ptr<ASTNode> parseContinue();
        // expression parsing
        std::unique_ptr<ASTNode> parseExpression();
        std::unique_ptr<ASTNode> parseLogicalOr();
        std::unique_ptr<ASTNode> parseLogicalAnd();
        std::unique_ptr<ASTNode> parseLogicalNot();
        std::unique_ptr<ASTNode> parseRelational();
        std::unique_ptr<ASTNode> parseAdditive();
        std::unique_ptr<ASTNode> parseMultiplicative();
        std::unique_ptr<ASTNode> parsePower();
        std::unique_ptr<ASTNode> parseUnary();
        std::unique_ptr<ASTNode> parsePrimary();
        // statement and i/o parsing
        std::unique_ptr<ASTNode> parsePrint();
        std::unique_ptr<ASTNode> parseRead();
        std::unique_ptr<ASTNode> parseAssign();
        std::unique_ptr<ASTNode> parseCall();
        std::unique_ptr<ASTNode> parseReturn();
        std::unique_ptr<ASTNode> parseStatement();
};

#endif
