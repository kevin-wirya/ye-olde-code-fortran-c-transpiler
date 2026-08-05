#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

#include<string>
enum class TokenType {
    // keywords
    PROGRAM = 1,
    END,
    SUBROUTINE,
    FUNCTION,
    RETURN,
    CALL,
    IMPLICIT,
    NONE,
    INTEGER,
    REAL,
    LOGICAL,
    COMMON,
    DO,
    CONTINUE,
    IF,
    THEN,
    ELSE,
    ENDIF,
    GOTO,
    STOP,
    PRINT,
    READ,
    // logical operator and boolean literals
    DOT_TRUE,
    DOT_FALSE,
    DOT_AND,
    DOT_OR,
    DOT_NOT,
    // relational operators
    DOT_EQ,
    DOT_NE,
    DOT_LT,
    DOT_LE,
    DOT_GT,
    DOT_GE,
    // arithmetic operators
    PLUS,
    MINUS,
    STAR,
    SLASH,
    POWER,
    ASSIGN,
    // delimiters and punctuation
    LPAREN,
    RPAREN,
    COMMA,
    // others
    LABEL,
    IDENTIFIER,
    INT_LITERAL,
    REAL_LITERAL,
    STRING_LITERAL,
    COMMENT,
    CONTINUATION_MARKER,
    // special tokens
    UNKNOWN, 
    TOKEN_EOF      
};

struct Token{
    TokenType type;
    std::string lexeme;
    int line;
    int column;
    Token(TokenType t,const std::string& lex,int ln=1,int col=1)
        :type(t),lexeme(lex),line(ln),column(col){}
};

inline std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::PROGRAM: return "PROGRAM";
        case TokenType::END: return "END";
        case TokenType::SUBROUTINE: return "SUBROUTINE";
        case TokenType::FUNCTION: return "FUNCTION";
        case TokenType::RETURN: return "RETURN";
        case TokenType::CALL: return "CALL";
        case TokenType::IMPLICIT: return "IMPLICIT";
        case TokenType::NONE: return "NONE";
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::REAL: return "REAL";
        case TokenType::LOGICAL: return "LOGICAL";
        case TokenType::COMMON: return "COMMON";
        case TokenType::DO: return "DO";
        case TokenType::CONTINUE: return "CONTINUE";
        case TokenType::IF: return "IF";
        case TokenType::THEN: return "THEN";
        case TokenType::ELSE: return "ELSE";
        case TokenType::ENDIF: return "ENDIF";
        case TokenType::GOTO: return "GOTO";
        case TokenType::STOP: return "STOP";
        case TokenType::PRINT: return "PRINT";
        case TokenType::READ: return "READ";
        case TokenType::DOT_TRUE: return ".TRUE.";
        case TokenType::DOT_FALSE: return ".FALSE.";
        case TokenType::DOT_AND: return ".AND.";
        case TokenType::DOT_OR: return ".OR.";
        case TokenType::DOT_NOT: return ".NOT.";
        case TokenType::DOT_EQ: return ".EQ.";
        case TokenType::DOT_NE: return ".NE.";
        case TokenType::DOT_LT: return ".LT.";
        case TokenType::DOT_LE: return ".LE.";
        case TokenType::DOT_GT: return ".GT.";
        case TokenType::DOT_GE: return ".GE.";
        case TokenType::PLUS: return "+";
        case TokenType::MINUS: return "-";
        case TokenType::STAR: return "*";
        case TokenType::SLASH: return "/";
        case TokenType::POWER: return "**";
        case TokenType::ASSIGN: return "=";
        case TokenType::LPAREN: return "(";
        case TokenType::RPAREN: return ")";
        case TokenType::COMMA: return ",";
        case TokenType::LABEL: return "LABEL";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::INT_LITERAL: return "INT_LITERAL";
        case TokenType::REAL_LITERAL: return "REAL_LITERAL";
        case TokenType::STRING_LITERAL: return "STRING_LITERAL";
        case TokenType::COMMENT: return "COMMENT";
        case TokenType::CONTINUATION_MARKER: return "CONTINUATION_MARKER";
        case TokenType::UNKNOWN: return "UNKNOWN";
        case TokenType::TOKEN_EOF: return "EOF";
        default: return "UNKNOWN_TOKEN";
    }
}

#endif
