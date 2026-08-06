#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "TokenType.h"
#include "Parser.h"
#include "PrintVisitor.h"

namespace fs = std::filesystem;

extern int yylex();
extern FILE* yyin;
extern char* yytext;
extern int current_line;
extern int current_col;

int main(int argc, char* argv[]) {
    std::string input_path;
    if (argc > 1) {
        input_path = argv[1];
    } else {
        std::cout << "Input Fortran 77 filename (ex: sample_1.f): ";
        std::cin >> input_path;
    }
    if (!fs::exists(input_path) && fs::exists("tests/" + input_path)) {
        input_path = "tests/" + input_path;
    }

    FILE* file = fopen(input_path.c_str(), "r");
    if (!file) {
        std::cerr << "Error: Cannot open file " << input_path << std::endl;
        return 1;
    }
    fs::path p(input_path);
    std::string stem = p.stem().string();
    // setup lexer
    fs::create_directories("tests/lexer");
    std::string lexer_output_path = "tests/lexer/" + stem + "_lexer";
    std::ofstream lexer_out_file(lexer_output_path);
    yyin=file;
    std::cout<<"------> Starting file tokenizing (" << input_path << ")\n";
    std::vector<Token> tokens;
    int token_code;
    while((token_code = yylex()) != 0){
        TokenType type = static_cast<TokenType>(token_code);
        Token tok(type, std::string(yytext), current_line, current_col);
        tokens.push_back(tok);
        std::string line_out="[Line " + std::to_string(current_line) + "] " +
                               tokenTypeToString(type) + " : \"" + yytext + "\"";
        std::cout<<line_out<<"\n";
        if(lexer_out_file.is_open()){
            lexer_out_file<<line_out<<"\n";
        }
    }
    fclose(file);
    if(lexer_out_file.is_open()){
        lexer_out_file.close();
        std::cout<<"------> Lexer result successfully saved in "<< lexer_output_path<<"\n";
    }
    std::cout<<"------> File tokenizing finished.\n\n";
    // setup parser
    fs::create_directories("tests/parser");
    std::string parser_output_path = "tests/parser/" + stem + "_parser";
    std::ofstream parser_out_file(parser_output_path);
    std::cout<<"------> Starting AST Parsing...\n";
    Parser parser(tokens);
    try{
        auto ast = parser.parse();
        if(ast){
            std::cout<<"\n====== AST TREE VISUALIZATION ======\n";
            PrintVisitor consoleVisitor(std::cout);
            ast->accept(consoleVisitor);
            std::cout<<"====================================\n";
            if(parser_out_file.is_open()){
                PrintVisitor fileVisitor(parser_out_file);
                ast->accept(fileVisitor);
                parser_out_file.close();
                std::cout<<"------> Parser result successfully saved in "<< parser_output_path<<"\n";
            }
        }
    }catch(const std::exception& e){
        std::cerr<<"Parser Error: "<< e.what()<<"\n";
        return 1;
    }

    return 0;
}