#include "Parser.h"
#include <iostream>

Token Parser::peek()const{
    if(current>=tokens.size())return Token(TokenType::TOKEN_EOF,"",0,0);
    return tokens[current];
}

TokenType Parser::peekNextType()const{
    if(current+1>=tokens.size())return TokenType::TOKEN_EOF;
    return tokens[current+1].type;
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
    std::string full_error="Syntax Error [Line "+std::to_string(err_token.line)+ 
                            ", Column "+std::to_string(err_token.column)+"]: "+ 
                            error_msg+" (Found '"+err_token.lexeme+"')";
    syntax_errors.push_back(full_error);
    throw ParseError(full_error);
}

void Parser::synchronize(){
    if(isAtEnd())return;
    advance();
    while(!isAtEnd()){
        if(previous().type==TokenType::END)return;
        switch(peek().type){
            case TokenType::PROGRAM:
            case TokenType::SUBROUTINE:
            case TokenType::FUNCTION:
            case TokenType::INTEGER:
            case TokenType::REAL:
            case TokenType::LOGICAL:
            case TokenType::IF:
            case TokenType::DO:
            case TokenType::GOTO:
            case TokenType::CONTINUE:
            case TokenType::PRINT:
            case TokenType::READ:
            case TokenType::CALL:
            case TokenType::RETURN:
            case TokenType::END:
                return;
            default:
                advance();
        }
    }
}

std::unique_ptr<ASTNode> Parser::parse(){
    try{
        if(check(TokenType::PROGRAM))return parseProgram();
        else if(check(TokenType::SUBROUTINE))return parseSubroutine();
        else if(check(TokenType::FUNCTION)||check(TokenType::INTEGER)||check(TokenType::REAL)||check(TokenType::LOGICAL))return parseFunction();
        else{
            Token err_token=peek();
            std::string full_error="Syntax Error: Expected PROGRAM, SUBROUTINE, or FUNCTION at top level (Found '"+err_token.lexeme+"')";
            syntax_errors.push_back(full_error);
            return nullptr;
        }
    }catch(const ParseError&){
        synchronize();
        return nullptr;
    }
}

// main parsing
std::unique_ptr<ASTNode> Parser::parseProgram(){
    try{
        consume(TokenType::PROGRAM,"Expected PROGRAM keyword");
        Token name_token=consume(TokenType::IDENTIFIER,"Expected program name identifier");
        std::vector<std::unique_ptr<ASTNode>> body;
        while(!isAtEnd()&&!check(TokenType::END)){
            try{
                auto stmt=parseStatement();
                if(stmt)body.push_back(std::move(stmt));
            }catch(const ParseError&){
                synchronize();
            }
        }
        consume(TokenType::END,"Expected END statement at program closure");
        if(match(TokenType::PROGRAM))match(TokenType::IDENTIFIER);
        while(!isAtEnd()){
            try{
                if(check(TokenType::SUBROUTINE)){
                    body.push_back(parseSubroutine());
                }else if(check(TokenType::FUNCTION)||(check(TokenType::INTEGER)&&peekNextType()==TokenType::FUNCTION)||(check(TokenType::REAL)&&peekNextType()==TokenType::FUNCTION)||(check(TokenType::LOGICAL)&&peekNextType()==TokenType::FUNCTION)||(check(TokenType::CHARACTER)&&peekNextType()==TokenType::FUNCTION)){
                    body.push_back(parseFunction());
                }else break;
            }catch(const ParseError&){
                synchronize();
            }
        }
        return std::make_unique<ProgramNode>(name_token.lexeme,std::move(body));
    }catch(const ParseError&){
        synchronize();
        return nullptr;
    }
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
    std::string ret_type="";
    if(match(TokenType::INTEGER))ret_type="INTEGER";
    else if(match(TokenType::REAL))ret_type="REAL";
    else if(match(TokenType::LOGICAL))ret_type="LOGICAL";
    else if(match(TokenType::CHARACTER)){
        ret_type="CHARACTER";
        if(match(TokenType::STAR)){
            Token lenToken=consume(TokenType::INT_LITERAL,"Expected integer length after CHARACTER*");
            ret_type+="*"+lenToken.lexeme;
        }
    }
    consume(TokenType::FUNCTION,"Expected FUNCTION keyword");
    Token name_token=consume(TokenType::IDENTIFIER,"Expected function name identifier");
    if(ret_type.empty()){
        char fc=toupper(name_token.lexeme[0]);
        ret_type=(fc>='I'&&fc<='N')?"INTEGER":"REAL";
    }
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
    else if(match(TokenType::CHARACTER)){
        typeName="CHARACTER";
        if(match(TokenType::STAR)){
            Token lenToken=consume(TokenType::INT_LITERAL,"Expected integer length after CHARACTER*");
            typeName+="*"+lenToken.lexeme;
        }
    }
    else throw std::runtime_error("Expected type specification in declaration");
    std::vector<std::string> scalar_vars;
    do{
        Token varToken=consume(TokenType::IDENTIFIER,"Expected variable name in declaration");
        if(check(TokenType::LPAREN)){
            consume(TokenType::LPAREN,"Expected '(' for array dimensions");
            std::vector<ArrayDimension> dims;
            do{
                Token upperToken(TokenType::UNKNOWN,"");
                if(check(TokenType::INT_LITERAL)||check(TokenType::IDENTIFIER)){
                    upperToken=advance();
                }else{
                    throw std::runtime_error("Expected integer or identifier for dimension size");
                }
                dims.push_back(ArrayDimension(upperToken.lexeme));
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
    std::vector<std::unique_ptr<ASTNode>> then_body;
    std::vector<std::unique_ptr<ASTNode>> else_body;
    if(match(TokenType::THEN)){
        while(!isAtEnd()&&!check(TokenType::ELSE)&&!check(TokenType::ENDIF)){
            then_body.push_back(parseStatement());
        }
        if(match(TokenType::ELSE)){
            while(!isAtEnd()&&!check(TokenType::ENDIF)){
                else_body.push_back(parseStatement());
            }
        }
        consume(TokenType::ENDIF,"Expected ENDIF after IF block");
    } else {
        then_body.push_back(parseStatement());
    }
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
    active_do_labels.push_back(target_label);
    std::vector<std::unique_ptr<ASTNode>> body;
    while(!isAtEnd()){
        if(check(TokenType::INT_LITERAL)||check(TokenType::LABEL)){
            if(std::stoi(peek().lexeme)==target_label){
                int cnt=0;
                for(int l:active_do_labels)if(l==target_label)cnt++;
                if(cnt==1){
                    advance();
                    if(match(TokenType::CONTINUE))body.push_back(std::make_unique<ContinueNode>(target_label));
                } else body.push_back(std::make_unique<ContinueNode>(target_label));
                active_do_labels.pop_back();
                break;
            }
        }
        body.push_back(parseStatement());
    }
    return std::make_unique<DoNode>(target_label,var_token.lexeme,std::move(start_expr),std::move(end_expr),std::move(step_expr),std::move(body));
}

std::unique_ptr<ASTNode> Parser::parseGoto() {
    consume(TokenType::GOTO,"Expected GOTO keyword");
    if(check(TokenType::LPAREN)){
        consume(TokenType::LPAREN,"Expected '(' in computed GOTO");
        std::vector<int> labels;
        do{
            Token label_token=consume(TokenType::INT_LITERAL,"Expected label number in computed GOTO list");
            labels.push_back(std::stoi(label_token.lexeme));
        }while(match(TokenType::COMMA));
        consume(TokenType::RPAREN, "Expected ')' after computed GOTO labels");
        match(TokenType::COMMA);
        auto selector=parseExpression();
        return std::make_unique<ComputedGotoNode>(std::move(labels),std::move(selector));
    }else{
        Token label_token=consume(TokenType::INT_LITERAL,"Expected label number after GOTO");
        int label=std::stoi(label_token.lexeme);
        return std::make_unique<GotoNode>(label);
    }
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
    if(match(TokenType::DOT_TRUE)||match(TokenType::DOT_FALSE))return std::make_unique<StringLiteralNode>(previous().lexeme);
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

std::unique_ptr<ASTNode> Parser::parseExpression(){
    return parseLogicalOr();
}

std::unique_ptr<ASTNode> Parser::parseLogicalOr(){
    auto expr=parseLogicalAnd();
    while(match(TokenType::DOT_OR)){
        std::string op=previous().lexeme;
        auto right=parseLogicalAnd();
        expr=std::make_unique<BinaryOpNode>(std::move(expr),op,std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseLogicalAnd(){
    auto expr=parseLogicalNot();
    while(match(TokenType::DOT_AND)){
        std::string op=previous().lexeme;
        auto right=parseLogicalNot();
        expr=std::make_unique<BinaryOpNode>(std::move(expr),op,std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseLogicalNot(){
    if(match(TokenType::DOT_NOT)){
        std::string op=previous().lexeme;
        auto right=parseLogicalNot();
        return std::make_unique<UnaryOpNode>(op,std::move(right));
    }
    return parseRelational();
}

std::unique_ptr<ASTNode> Parser::parseRelational(){
    auto expr=parseAdditive();
    while(check(TokenType::DOT_EQ)||check(TokenType::DOT_NE)||
            check(TokenType::DOT_LT)||check(TokenType::DOT_LE)||
            check(TokenType::DOT_GT)||check(TokenType::DOT_GE)){
        advance();
        std::string op=previous().lexeme;
        auto right=parseAdditive();
        expr=std::make_unique<BinaryOpNode>(std::move(expr),op,std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseAdditive(){
    auto expr=parseMultiplicative();
    while(match(TokenType::PLUS)||match(TokenType::MINUS)){
        std::string op=previous().lexeme;
        auto right=parseMultiplicative();
        expr=std::make_unique<BinaryOpNode>(std::move(expr),op,std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseMultiplicative(){
    auto expr=parsePower();
    while(match(TokenType::STAR)||match(TokenType::SLASH)){
        std::string op=previous().lexeme;
        auto right=parsePower();
        expr=std::make_unique<BinaryOpNode>(std::move(expr),op,std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::parsePower(){
    auto expr=parseUnary();
    if(match(TokenType::POWER)){
        std::string op=previous().lexeme;
        auto right=parsePower();
        expr=std::make_unique<BinaryOpNode>(std::move(expr),op,std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseUnary(){
    if(match(TokenType::PLUS)||match(TokenType::MINUS)){
        std::string op=previous().lexeme;
        auto right=parseUnary();
        return std::make_unique<UnaryOpNode>(op,std::move(right));
    }
    return parsePrimary();
}

// statement and i/o parsing
std::unique_ptr<ASTNode> Parser::parsePrint(){
    consume(TokenType::PRINT,"Expected PRINT keyword");
    std::string fmt="*";
    if(match(TokenType::STAR))fmt="*";
    consume(TokenType::COMMA,"Expected ',' after PRINT format specifier");
    std::vector<std::unique_ptr<ASTNode>> exprs;
    do{
        exprs.push_back(parseExpression());
    }while(match(TokenType::COMMA));
    return std::make_unique<PrintNode>(fmt, std::move(exprs));
}

std::unique_ptr<ASTNode> Parser::parseRead() {
    consume(TokenType::READ,"Expected READ keyword");
    std::string fmt="*";
    if(match(TokenType::STAR))fmt="*";
    consume(TokenType::COMMA,"Expected ',' after READ format specifier");
    std::vector<std::string> vars;
    do{
        Token v = consume(TokenType::IDENTIFIER,"Expected variable name in READ statement");
        vars.push_back(v.lexeme);
    }while(match(TokenType::COMMA));
    return std::make_unique<ReadNode>(fmt, std::move(vars));
}

std::unique_ptr<ASTNode> Parser::parseAssign() {
    Token targetToken=consume(TokenType::IDENTIFIER,"Expected variable identifier in assignment");
    std::vector<std::unique_ptr<ASTNode>> indices;
    if(match(TokenType::LPAREN)){
        if(!check(TokenType::RPAREN)){
            do{
                indices.push_back(parseExpression());
            }while(match(TokenType::COMMA));
        }
        consume(TokenType::RPAREN,"Expected ')' after array indices in assignment");
    }
    consume(TokenType::ASSIGN,"Expected '=' in assignment");
    auto expr=parseExpression();
    if(indices.empty()){
        return std::make_unique<AssignNode>(targetToken.lexeme,std::move(expr));
    }
    return std::make_unique<AssignNode>(targetToken.lexeme,std::move(indices),std::move(expr));
}

std::unique_ptr<ASTNode> Parser::parseCall() {
    consume(TokenType::CALL,"Expected CALL keyword");
    Token subToken=consume(TokenType::IDENTIFIER,"Expected subroutine name in CALL");
    std::vector<std::unique_ptr<ASTNode>> args;
    if (match(TokenType::LPAREN)) {
        if (!check(TokenType::RPAREN)) {
            do {
                args.push_back(parseExpression());
            } while (match(TokenType::COMMA));
        }
        consume(TokenType::RPAREN,"Expected ')' after CALL arguments");
    }
    return std::make_unique<CallNode>(subToken.lexeme, std::move(args));
}

std::unique_ptr<ASTNode> Parser::parseReturn() {
    consume(TokenType::RETURN,"Expected RETURN keyword");
    return std::make_unique<ReturnNode>();
}

std::unique_ptr<ASTNode> Parser::parseStop() {
    consume(TokenType::STOP,"Expected STOP keyword");
    return std::make_unique<ReturnNode>();
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    int stmt_line=peek().line;
    std::unique_ptr<ASTNode> stmt=nullptr;
    if(check(TokenType::INT_LITERAL)||check(TokenType::LABEL)){
        int label=std::stoi(peek().lexeme);
        advance();
        if(match(TokenType::CONTINUE)) return std::make_unique<ContinueNode>(label);
        return std::make_unique<ContinueNode>(label);
    }
    else if(check(TokenType::IMPLICIT))stmt=parseImplicitNone();
    else if(check(TokenType::INTEGER)||check(TokenType::REAL)||check(TokenType::LOGICAL)||check(TokenType::CHARACTER))stmt=parseDeclaration();
    else if(check(TokenType::COMMON))stmt=parseCommonBlock();
    else if(check(TokenType::IF))stmt=parseIf();
    else if(check(TokenType::DO))stmt=parseDo();
    else if(check(TokenType::GOTO))stmt=parseGoto();
    else if(check(TokenType::CONTINUE))stmt=parseContinue();
    else if(check(TokenType::PRINT))stmt=parsePrint();
    else if(check(TokenType::READ))stmt=parseRead();
    else if(check(TokenType::CALL))stmt=parseCall();
    else if(check(TokenType::RETURN))stmt=parseReturn();
    else if(check(TokenType::STOP))stmt=parseStop();
    else if(check(TokenType::IDENTIFIER))stmt=parseAssign();
    else{
        advance();
        return nullptr;
    }
    if(stmt)stmt->line=stmt_line;
    return stmt;
}