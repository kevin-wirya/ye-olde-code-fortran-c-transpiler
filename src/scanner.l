%{
#include "TokenType.h"
#include <iostream>
#include <cstdlib>
#include <string>
int current_line = 1;
int current_col = 1;
%}

%option case-insensitive
%option noyywrap

%%

   /* fortran comments line */
^[C*].*                             { current_line++; current_col = 1; }

   /* whitespace and newline */
[ \t]+                              {current_col+=yyleng; }
\n|\r\n                             {current_line++; current_col=1; }

   /* keywords f77 */
"PROGRAM"                           {current_col+=yyleng;return static_cast<int>(TokenType::PROGRAM);}
"END"                               {current_col+=yyleng;return static_cast<int>(TokenType::END);}
"SUBROUTINE"                        {current_col+=yyleng;return static_cast<int>(TokenType::SUBROUTINE);}
"FUNCTION"                          {current_col+=yyleng;return static_cast<int>(TokenType::FUNCTION);}
"RETURN"                            {current_col+=yyleng;return static_cast<int>(TokenType::RETURN);}
"CALL"                              {current_col+=yyleng;return static_cast<int>(TokenType::CALL);}
"IMPLICIT"                          {current_col+=yyleng;return static_cast<int>(TokenType::IMPLICIT);}
"NONE"                              {current_col+=yyleng;return static_cast<int>(TokenType::NONE);}
"INTEGER"                           {current_col+=yyleng;return static_cast<int>(TokenType::INTEGER);}
"REAL"                              {current_col+=yyleng;return static_cast<int>(TokenType::REAL);}
"LOGICAL"                           {current_col+=yyleng;return static_cast<int>(TokenType::LOGICAL);}
"COMMON"                            {current_col+=yyleng;return static_cast<int>(TokenType::COMMON);}
"DO"                                {current_col+=yyleng;return static_cast<int>(TokenType::DO);}
"CONTINUE"                          {current_col+=yyleng;return static_cast<int>(TokenType::CONTINUE);}
"IF"                                {current_col+=yyleng;return static_cast<int>(TokenType::IF);}
"THEN"                              {current_col+=yyleng;return static_cast<int>(TokenType::THEN);}
"ELSE"                              {current_col+=yyleng;return static_cast<int>(TokenType::ELSE);}
"ENDIF"                             {current_col+=yyleng;return static_cast<int>(TokenType::ENDIF);}
"GOTO"                              {current_col+=yyleng;return static_cast<int>(TokenType::GOTO);}
"STOP"                              {current_col+=yyleng;return static_cast<int>(TokenType::STOP);}
"PRINT"                             {current_col+=yyleng;return static_cast<int>(TokenType::PRINT);}
"READ"                              {current_col+=yyleng;return static_cast<int>(TokenType::READ);}

   /* logical operators and boolean literals */
"\.TRUE\."                          {current_col+=yyleng;return static_cast<int>(TokenType::DOT_TRUE);}
"\.FALSE\."                         {current_col+=yyleng;return static_cast<int>(TokenType::DOT_FALSE);}
"\.AND\."                           {current_col+=yyleng;return static_cast<int>(TokenType::DOT_AND);}
"\.OR\."                            {current_col+=yyleng;return static_cast<int>(TokenType::DOT_OR);}
"\.NOT\."                           {current_col+=yyleng;return static_cast<int>(TokenType::DOT_NOT);}

   /* relational operators */
"\.EQ\."                            {current_col+=yyleng;return static_cast<int>(TokenType::DOT_EQ);}
"\.NE\."                            {current_col+=yyleng;return static_cast<int>(TokenType::DOT_NE);}
"\.LT\."                            {current_col+=yyleng;return static_cast<int>(TokenType::DOT_LT);}
"\.LE\."                            {current_col+=yyleng;return static_cast<int>(TokenType::DOT_LE);}
"\.GT\."                            {current_col+=yyleng;return static_cast<int>(TokenType::DOT_GT);}
"\.GE\."                            {current_col+=yyleng;return static_cast<int>(TokenType::DOT_GE);}

   /* arithmetic operators and delimiters */
"\+"                                {current_col+=yyleng;return static_cast<int>(TokenType::PLUS);}
"-"                                 {current_col+=yyleng;return static_cast<int>(TokenType::MINUS);}
"\*"                                {current_col+=yyleng;return static_cast<int>(TokenType::STAR);}
"/"                                 {current_col+=yyleng;return static_cast<int>(TokenType::SLASH);}
"\*\*"                              {current_col+=yyleng;return static_cast<int>(TokenType::POWER);}
"="                                 {current_col+=yyleng;return static_cast<int>(TokenType::ASSIGN);}
"\("                                {current_col+=yyleng;return static_cast<int>(TokenType::LPAREN);}
"\)"                                {current_col+=yyleng;return static_cast<int>(TokenType::RPAREN);}
","                                 {current_col+=yyleng;return static_cast<int>(TokenType::COMMA);}

   /* literals and identifiers */
'[^'\n]*'                           {current_col+=yyleng;return static_cast<int>(TokenType::STRING_LITERAL);}
[0-9]+\.[0-9]*([eE][+-]?[0-9]+)?    {current_col+=yyleng;return static_cast<int>(TokenType::REAL_LITERAL);}
\.[0-9]+([eE][+-]?[0-9]+)?          {current_col+=yyleng;return static_cast<int>(TokenType::REAL_LITERAL);}
[0-9]+                              {current_col+=yyleng;return static_cast<int>(TokenType::INT_LITERAL);}
[a-zA-Z][a-zA-Z0-9_]*               {current_col+=yyleng;return static_cast<int>(TokenType::IDENTIFIER);}

   /* unknown characters */
.                                   {current_col+=yyleng;return static_cast<int>(TokenType::UNKNOWN);}

%%