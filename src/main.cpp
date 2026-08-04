#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include "TokenType.h"

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
    fs::create_directories("tests/lexer");
    std::string output_path = "tests/lexer/" + stem + "_lexer";
    std::ofstream out_file(output_path);
    if (!out_file.is_open()) {
        std::cerr << "Warning: Failed to create output file " << output_path << std::endl;
    }
    yyin = file;
    std::cout << "------> Starting file tokenizing (" << input_path << ")\n";
    int token_code;
    while ((token_code = yylex()) != 0) {
        TokenType type = static_cast<TokenType>(token_code);
        std::string line_out = "[Line " + std::to_string(current_line) + "] " +
                               tokenTypeToString(type) + " : \"" + yytext + "\"";
        std::cout << line_out << "\n";
        if (out_file.is_open()) {
            out_file << line_out << "\n";
        }
    }
    fclose(file);
    if (out_file.is_open()) {
        out_file.close();
        std::cout << "------> Lexer result successfully saved in " << output_path << "\n";
    }
    std::cout << "------> File tokenizing finished.\n";
    return 0;
}