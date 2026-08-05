#include "Parser.h"
#include <iostream>

Token Parser::peek()const{
    if(current>=tokens.size())return Token(TokenType::TOKEN_EOF,"",0,0);
    return tokens[current];
}

Token Parser::previous()const{
    if(current==0)return Token(TokenType::TOKEN_EOF,"",0,0);
    return tokens[current-1];
}

bool Parser::isAtEnd()const{
    return peek().type==TokenType::TOKEN_EOF||current>=tokens.size();
}

bool Parser::check(TokenType type)const{
    if(isAtEnd())return false;
    return peek().type==type;
}

bool Parser::match(TokenType type){
    if(check(type)){
        advance();
        return true;
    }
    return false;
}

Token Parser::advance(){
    if(!isAtEnd())current++;
    return previous();
}

Token Parser::consume(TokenType type, const std::string& error_msg){
    if(check(type))return advance();
    Token err_token=peek();
    std::string full_error = "Syntax Error [Line " + std::to_string(err_token.line) + 
                            ", Column " + std::to_string(err_token.column) + "]: " + 
                            error_msg + " (Found '" + err_token.lexeme + "')";
    throw std::runtime_error(full_error);
}

std::unique_ptr<ASTNode> Parser::parse(){
    // will be implemented later
    return nullptr;
}