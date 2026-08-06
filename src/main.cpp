#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "TokenType.h"
#include "Parser.h"
#include "PrintVisitor.h"
#include "SemanticAnalyzer.h"
#include "codegen/CodeGenVisitor.h"

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
        auto ast=parser.parse();
        if(parser.hasErrors()){
            std::cout<<"\n====== SYNTAX ERRORS ======\n";
            if(parser_out_file.is_open()){
                parser_out_file<<"====== SYNTAX ERRORS ======\n";
            }
            for(const auto& err:parser.getErrors()){
                std::cout<<"[Error] "<<err<<"\n";
                if(parser_out_file.is_open()){
                    parser_out_file<<"[Error] "<<err<<"\n";
                }
            }
            std::cout<<"===========================\n";
            if(parser_out_file.is_open()){
                parser_out_file<<"===========================\n";
            }
        }
        if(ast){
            std::cout<<"\n====== AST TREE VISUALIZATION ======\n";
            PrintVisitor consoleVisitor(std::cout);
            ast->accept(consoleVisitor);
            std::cout<<"====================================\n";
            if(parser_out_file.is_open()){
                PrintVisitor fileVisitor(parser_out_file);
                ast->accept(fileVisitor);
            }
        }
        if(parser_out_file.is_open()){
            parser_out_file.close();
            std::cout<<"------> Parser result successfully saved in "<<parser_output_path<<"\n";
        }

        // setup semantic analyzer
        fs::create_directories("tests/semantic");
        std::string semantic_output_path="tests/semantic/"+stem+"_semantic";
        std::ofstream semantic_out_file(semantic_output_path);
        std::cout<<"\n------> Starting Semantic Analysis...\n";

        if(ast&&!parser.hasErrors()){
            SemanticAnalyzer analyzer;
            bool is_valid=analyzer.analyze(*ast);
            if(is_valid){
                std::string msg="Semantic Analysis SUCCESS: No errors found.";
                std::cout<<msg<<"\n";
                if(semantic_out_file.is_open()){
                    semantic_out_file<<msg<<"\n";
                }
                analyzer.printSymbolTables(std::cout);
                if(semantic_out_file.is_open()){
                    analyzer.printSymbolTables(semantic_out_file);
                }
                std::cout<<"\n====== DECORATED AST TREE VISUALIZATION ======\n";
                PrintVisitor decoratedConsoleVisitor(std::cout,true);
                ast->accept(decoratedConsoleVisitor);
                std::cout<<"===============================================\n";

                if(semantic_out_file.is_open()){
                    semantic_out_file<<"\n====== DECORATED AST TREE VISUALIZATION ======\n";
                    PrintVisitor decoratedFileVisitor(semantic_out_file,true);
                    ast->accept(decoratedFileVisitor);
                    semantic_out_file<<"===============================================\n";
                }

                // setup code generation
                fs::create_directories("tests/codegen");
                std::string codegen_output_path="tests/codegen/"+stem+".c";
                std::ofstream codegen_out_file(codegen_output_path);
                std::cout<<"\n------> Starting Code Generation...\n";

                std::cout<<"\n====== C TARGET CODE GENERATION ======\n";
                CodeGenVisitor consoleCodeGen(std::cout);
                ast->accept(consoleCodeGen);
                std::cout<<"======================================\n";

                if(codegen_out_file.is_open()){
                    CodeGenVisitor fileCodeGen(codegen_out_file);
                    ast->accept(fileCodeGen);
                    codegen_out_file.close();
                    std::cout<<"------> Code Generation result successfully saved in "<<codegen_output_path<<"\n";
                }
            }else{
                std::cout<<"\n====== SEMANTIC ERRORS ======\n";
                if(semantic_out_file.is_open()){
                    semantic_out_file<<"====== SEMANTIC ERRORS ======\n";
                }
                for(const auto& err:analyzer.getErrors()){
                    std::cout<<"[Error] "<<err<<"\n";
                    if(semantic_out_file.is_open()){
                        semantic_out_file<<"[Error] "<<err<<"\n";
                    }
                }
                std::cout<<"=============================\n";
            }
        }else{
            std::cout<<"\n====== SEMANTIC ERRORS ======\n";
            std::string err_msg="[Error] Cannot perform semantic analysis due to prior syntax error(s).";
            std::cout<<err_msg<<"\n";
            std::cout<<"=============================\n";
            if(semantic_out_file.is_open()){
                semantic_out_file<<"====== SEMANTIC ERRORS ======\n";
                semantic_out_file<<err_msg<<"\n";
                semantic_out_file<<"=============================\n";
            }
        }
        if(semantic_out_file.is_open()){
            semantic_out_file.close();
            std::cout<<"------> Semantic result successfully saved in "<<semantic_output_path<<"\n";
        }
    }catch(const std::exception& e){
        std::cerr<<"Parser Error: "<< e.what()<<"\n";
        return 1;
    }

    return 0;
}