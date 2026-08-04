#include<iostream>
#include<cstdio>
#include "TokenType.h"

extern int yylex();
extern FILE* yyin;
extern char* yytext;
extern int current_line;
extern int current_col;

int main(int argc, char* argv[]){
    const char* filename=(argc>1)?argv[1]:"tests/sample_1.f";
    FILE* file=fopen(filename,"r");

    if(!file){
        std::cerr<<"Error: Tidak dapat membuka file "<<filename<<std::endl;
        return 1;
    }
    yyin=file;
    std::cout<<"------> Starting file tokenizing ("<<filename<<")\n";
    int token_code;
    while((token_code=yylex())!=0){
        TokenType type=static_cast<TokenType>(token_code);
        std::cout<<"[Line "<<current_line<<"]"<<tokenTypeToString(type)<<" : \""<<yytext<<"\"\n";
    }
    fclose(file);
    std::cout<<"------> File tokenizing finished.\n";
    return 0;
}