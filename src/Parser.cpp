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

// main parsing
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

// declaration parsing
std::unique_ptr<ASTNode> Parser::parseImplicitNone(){
    consume(TokenType::IMPLICIT,"Expected IMPLICIT keyword");
    consume(TokenType::NONE,"Expected NONE after IMPLICIT");
    return std::make_unique<ImplicitNoneNode>();
}

std::unique_ptr<ASTNode> Parser::parseCommonBlock() {
    consume(TokenType::COMMON,"Expected COMMON keyword");
    std::string block_name="";
    if (match(TokenType::SLASH)){
        if (!check(TokenType::SLASH)){
            Token bName=consume(TokenType::IDENTIFIER,"Expected common block name");
            block_name=bName.lexeme;
        }
        consume(TokenType::SLASH,"Expected '/' after common block name");
    }
    std::vector<std::string> vars;
    do{
        Token v=consume(TokenType::IDENTIFIER,"Expected variable name in COMMON block");
        vars.push_back(v.lexeme);
    }while(match(TokenType::COMMA));
    return std::make_unique<CommonBlockNode>(block_name, std::move(vars));
}

std::unique_ptr<ASTNode> Parser::parseDeclaration() {
    std::string typeName="";
    if(match(TokenType::INTEGER))typeName="INTEGER";
    else if(match(TokenType::REAL))typeName="REAL";
    else if(match(TokenType::LOGICAL))typeName="LOGICAL";
    else throw std::runtime_error("Expected type specification in declaration");
    std::vector<std::string> scalar_vars;
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
            scalar_vars.push_back(varToken.lexeme);
        }
    }while(match(TokenType::COMMA));
    return std::make_unique<TypeDeclNode>(typeName, std::move(scalar_vars));
}

// expression parsing
std::unique_ptr<ASTNode> Parser::parseIf(){
    consume(TokenType::IF,"Expected IF keyword");
    consume(TokenType::LPAREN,"Expected '(' after IF");
    auto condition=parseExpression();
    consume(TokenType::RPAREN,"Expected ')' after IF condition");
    consume(TokenType::THEN,"Expected THEN after IF condition");
    std::vector<std::unique_ptr<ASTNode>> then_body;
    std::vector<std::unique_ptr<ASTNode>> else_body;
    while(!isAtEnd()&&!check(TokenType::ELSE)&&!check(TokenType::ENDIF)){
        then_body.push_back(parseStatement());
    }
    if(match(TokenType::ELSE)){
        while(!isAtEnd()&&!check(TokenType::ENDIF)){
            else_body.push_back(parseStatement());
        }
    }
    consume(TokenType::ENDIF,"Expected ENDIF statement");
    return std::make_unique<IfNode>(std::move(condition),std::move(then_body),std::move(else_body));
}

std::unique_ptr<ASTNode> Parser::parseDo(){
    consume(TokenType::DO,"Expected DO keyword");
    Token label_token=consume(TokenType::INT_LITERAL,"Expected target label number for DO loop");
    int target_label=std::stoi(label_token.lexeme);
    Token var_token=consume(TokenType::IDENTIFIER,"Expected loop variable identifier");
    consume(TokenType::ASSIGN,"Expected '=' in DO loop statement");
    auto start_expr=parseExpression();
    consume(TokenType::COMMA,"Expected ',' after start expression");
    auto end_expr=parseExpression();
    std::unique_ptr<ASTNode> step_expr=nullptr;
    if(match(TokenType::COMMA)){
        step_expr=parseExpression();
    }
    std::vector<std::unique_ptr<ASTNode>> body;
    while(!isAtEnd()){
        if(check(TokenType::INT_LITERAL)||check(TokenType::LABEL)){
            if(std::stoi(peek().lexeme)==target_label){
                advance();
                if(match(TokenType::CONTINUE)){
                    body.push_back(std::make_unique<ContinueNode>(target_label));
                }
                break;
            }
        }
        body.push_back(parseStatement());
    }
    return std::make_unique<DoNode>(target_label,var_token.lexeme,std::move(start_expr),std::move(end_expr),std::move(step_expr),std::move(body));
}

std::unique_ptr<ASTNode> Parser::parseGoto() {
    consume(TokenType::GOTO, "Expected GOTO keyword");
    Token label_token=consume(TokenType::INT_LITERAL, "Expected label number after GOTO");
    int label=std::stoi(label_token.lexeme);
    return std::make_unique<GotoNode>(label);
}

std::unique_ptr<ASTNode> Parser::parseContinue() {
    int label=0;
    if(check(TokenType::INT_LITERAL)||check(TokenType::LABEL)){
        label=std::stoi(advance().lexeme);
    }
    consume(TokenType::CONTINUE,"Expected CONTINUE keyword");
    return std::make_unique<ContinueNode>(label);
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
    if(match(TokenType::INT_LITERAL))return std::make_unique<NumberLiteralNode>(previous().lexeme, false);
    if(match(TokenType::REAL_LITERAL))return std::make_unique<NumberLiteralNode>(previous().lexeme, true);
    if(match(TokenType::STRING_LITERAL))return std::make_unique<StringLiteralNode>(previous().lexeme);
    if(match(TokenType::IDENTIFIER)){
        std::string name = previous().lexeme;
        if(match(TokenType::LPAREN)){
            std::vector<std::unique_ptr<ASTNode>> args;
            if(!check(TokenType::RPAREN)){
                do{
                    args.push_back(parseExpression());
                }while(match(TokenType::COMMA));
            }
            consume(TokenType::RPAREN,"Expected ')' after array index/args");
            return std::make_unique<ArrayAccessNode>(name,std::move(args));
        }
        return std::make_unique<IdentifierNode>(name);
    }
    if(match(TokenType::LPAREN)){
        auto expr=parseExpression();
        consume(TokenType::RPAREN,"Expected ')' after expression");
        return expr;
    }
    throw std::runtime_error("Syntax Error: Unexpected token in expression: " + peek().lexeme);
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    return parsePrimary();
}

// statement parsing
std::unique_ptr<ASTNode> Parser::parseStatement() {
    if(check(TokenType::IMPLICIT))return parseImplicitNone();
    if(check(TokenType::INTEGER)||check(TokenType::REAL)||check(TokenType::LOGICAL))return parseDeclaration();
    if(check(TokenType::COMMON))return parseCommonBlock();
    if(check(TokenType::IF))return parseIf();
    if(check(TokenType::DO))return parseDo();
    if(check(TokenType::GOTO))return parseGoto();
    if(check(TokenType::CONTINUE))return parseContinue();
    advance();
    return nullptr;
}