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
    if(check(TokenType::PROGRAM))return parseProgram();
    else if(check(TokenType::SUBROUTINE))return parseSubroutine();
    else if(check(TokenType::FUNCTION))return parseFunction();
    else throw std::runtime_error("Syntax Error: Expected PROGRAM, SUBROUTINE, or FUNCTION at top level");
}

std::unique_ptr<ASTNode> Parser::parseProgram(){
    consume(TokenType::PROGRAM,"Expected PROGRAM keyword");
    Token name_token=consume(TokenType::IDENTIFIER,"Expected program name identifier");
    std::vector<std::unique_ptr<ASTNode>> body;
    while(!isAtEnd()&&!check(TokenType::END)){
        body.push_back(parseStatement());
    }
    consume(TokenType::END, "Expected END statement at program closure");
    if(match(TokenType::PROGRAM))match(TokenType::IDENTIFIER);
    return std::make_unique<ProgramNode>(name_token.lexeme,std::move(body));
}

std::unique_ptr<ASTNode> Parser::parseSubroutine() {
    consume(TokenType::SUBROUTINE,"Expected SUBROUTINE keyword");
    Token name_token=consume(TokenType::IDENTIFIER,"Expected subroutine name identifier");
    std::vector<std::string> params;
    if(match(TokenType::LPAREN)){
        if(!check(TokenType::RPAREN)){
            do{
                Token p=consume(TokenType::IDENTIFIER,"Expected parameter identifier");
                params.push_back(p.lexeme);
            }while(match(TokenType::COMMA));
        }
        consume(TokenType::RPAREN,"Expected ')' after subroutine parameter list");
    }
    std::vector<std::unique_ptr<ASTNode>> body;
    while(!isAtEnd()&&!check(TokenType::END)){
        body.push_back(parseStatement());
    }
    consume(TokenType::END,"Expected END statement at subroutine closure");
    if(match(TokenType::SUBROUTINE))match(TokenType::IDENTIFIER);
    return std::make_unique<SubroutineNode>(name_token.lexeme,std::move(params),std::move(body));
}

std::unique_ptr<ASTNode> Parser::parseFunction() {
    std::string ret_type="REAL"; // default
    if(match(TokenType::INTEGER))ret_type="INTEGER";
    else if(match(TokenType::REAL))ret_type="REAL";
    else if(match(TokenType::LOGICAL))ret_type="LOGICAL";
    consume(TokenType::FUNCTION,"Expected FUNCTION keyword");
    Token name_token=consume(TokenType::IDENTIFIER,"Expected function name identifier");
    std::vector<std::string> params;
    if(match(TokenType::LPAREN)){
        if(!check(TokenType::RPAREN)){
            do{
                Token p=consume(TokenType::IDENTIFIER,"Expected parameter identifier");
                params.push_back(p.lexeme);
            }while(match(TokenType::COMMA));
        }
        consume(TokenType::RPAREN,"Expected ')' after function parameter list");
    }
    std::vector<std::unique_ptr<ASTNode>> body;
    while(!isAtEnd()&&!check(TokenType::END)){
        body.push_back(parseStatement());
    }
    consume(TokenType::END,"Expected END statement at function closure");
    if(match(TokenType::FUNCTION)){
        match(TokenType::IDENTIFIER);
    }
    return std::make_unique<FunctionNode>(name_token.lexeme,ret_type,std::move(params),std::move(body));
}
std::unique_ptr<ASTNode> Parser::parseImplicitNone(){
    consume(TokenType::IMPLICIT,"Expected IMPLICIT keyword");
    consume(TokenType::NONE,"Expected NONE after IMPLICIT");
    return std::make_unique<ImplicitNoneNode>();
}

std::unique_ptr<ASTNode> Parser::parseCommonBlock() {
    consume(TokenType::COMMON,"Expected COMMON keyword");
    std::string blockName="";
    if (match(TokenType::SLASH)){
        if (!check(TokenType::SLASH)){
            Token bName=consume(TokenType::IDENTIFIER,"Expected common block name");
            blockName=bName.lexeme;
        }
        consume(TokenType::SLASH,"Expected '/' after common block name");
    }
    std::vector<std::string> vars;
    do{
        Token v=consume(TokenType::IDENTIFIER,"Expected variable name in COMMON block");
        vars.push_back(v.lexeme);
    }while(match(TokenType::COMMA));
    return std::make_unique<CommonBlockNode>(blockName, std::move(vars));
}

std::unique_ptr<ASTNode> Parser::parseDeclaration() {
    std::string typeName="";
    if(match(TokenType::INTEGER))typeName="INTEGER";
    else if(match(TokenType::REAL))typeName="REAL";
    else if(match(TokenType::LOGICAL))typeName="LOGICAL";
    else throw std::runtime_error("Expected type specification in declaration");
    std::vector<std::string> scalarVars;
    do{
        Token varToken=consume(TokenType::IDENTIFIER,"Expected variable name in declaration");
        if(check(TokenType::LPAREN)){
            consume(TokenType::LPAREN,"Expected '(' for array dimensions");
            std::vector<ArrayDimension> dims;
            do{
                Token upperToken=consume(TokenType::INT_LITERAL,"Expected integer dimension size");
                int upper=std::stoi(upperToken.lexeme);
                dims.push_back(ArrayDimension(upper));
            }while(match(TokenType::COMMA));
            consume(TokenType::RPAREN,"Expected ')' after array dimensions");
            return std::make_unique<ArrayDeclNode>(varToken.lexeme,typeName,std::move(dims));
        } else {
            scalarVars.push_back(varToken.lexeme);
        }
    }while(match(TokenType::COMMA));
    return std::make_unique<TypeDeclNode>(typeName, std::move(scalarVars));
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    if(check(TokenType::IMPLICIT))return parseImplicitNone();
    if(check(TokenType::INTEGER)||check(TokenType::REAL)||check(TokenType::LOGICAL))return parseDeclaration();
    if(check(TokenType::COMMON))return parseCommonBlock();
    advance();
    return nullptr;
}