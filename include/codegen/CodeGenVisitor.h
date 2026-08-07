#ifndef CODE_GEN_VISITOR_H
#define CODE_GEN_VISITOR_H

#include "ASTVisitor.h"
#include "ProgramNode.h"
#include "SubroutineNode.h"
#include "FunctionNode.h"
#include "TypeDeclNode.h"
#include "ArrayDeclNode.h"
#include "CommonBlockNode.h"
#include "ImplicitNoneNode.h"
#include "IfNode.h"
#include "DoNode.h"
#include "AssignNode.h"
#include "PrintNode.h"
#include "ReadNode.h"
#include "CallNode.h"
#include "GotoNode.h"
#include "ContinueNode.h"
#include "ReturnNode.h"
#include "BinaryOpNode.h"
#include "UnaryOpNode.h"
#include "IdentifierNode.h"
#include "NumberLiteralNode.h"
#include "StringLiteralNode.h"
#include "ArrayAccessNode.h"
#include "SemanticAnalyzer.h"

#include <iostream>
#include <string>
#include <vector>
#include<unordered_map>

class CodeGenVisitor: public ASTVisitor{
    private:
        std::ostream& os;
        const std::unordered_map<std::string, CommonBlockInfo>* common_blocks;
        const std::vector<TabEntry>* tab;
        std::string getCType(const std::string& name) const{
            if(!tab) return "int";
            for(const auto& entry: *tab){
                if(entry.id==name&&(entry.obj=="variable"||entry.obj=="array")){
                    if(entry.type=="REAL")return "float";
                    if(entry.type=="LOGICAL")return "bool";
                    if(entry.type.find("CHARACTER")==0)return "char";
                    return "int";
                }
            }
            return "int";
        }
    public:
        explicit CodeGenVisitor(std::ostream& outStream=std::cout, 
                                const std::unordered_map<std::string, CommonBlockInfo>* cb=nullptr,
                                const std::vector<TabEntry>* t=nullptr)
                                : os(outStream), common_blocks(cb), tab(t) {
            os << "#include <stdio.h>\n";
            os << "#include <math.h>\n";
            os<<"#include <stdlib.h>\n";
            os<<"#include <stdbool.h>\n";
            os<<"#include <string.h>\n\n";
            os<<"#define F77_STR_ASSIGN(dest, src, len) \\\n";
            os<<"    do { \\\n";
            os<<"        strncpy(dest, src, len); \\\n";
            os<<"        int _l = strlen(src); \\\n";
            os<<"        for(int _i = _l; _i < len; _i++) dest[_i] = ' '; \\\n";
            os<<"        dest[len] = '\\0'; \\\n";
            os<<"    } while(0)\n\n";
            if(common_blocks){
                for(const auto& pair: *common_blocks){
                    os<<"struct "<<pair.first<<"_t {\n";
                    for(const auto& var: pair.second.variable_names){
                        os<<"    "<<getCType(var)<<" "<<var<<";\n";
                    }
                    os<<"} "<<pair.first<<";\n\n";
                }
            }
        }
        std::unordered_map<std::string, std::vector<std::string>> array_dims;
        std::unordered_map<std::string, int> string_lengths;
        void printFlattenedIndex(const std::string& array_name, const std::vector<std::unique_ptr<ASTNode>>& indices);
        void visit(ProgramNode& node) override;
        void visit(SubroutineNode& node) override;
        void visit(FunctionNode& node) override;
        void visit(ImplicitNoneNode& node) override;
        void visit(TypeDeclNode& node) override;
        void visit(ArrayDeclNode& node) override;
        void visit(CommonBlockNode& node) override;
        void visit(IfNode& node) override;
        void visit(DoNode& node) override;
        void visit(AssignNode& node) override;
        void visit(PrintNode& node) override;
        void visit(ReadNode& node) override;
        void visit(CallNode& node) override;
        void visit(GotoNode& node) override;
        void visit(ContinueNode& node) override;
        void visit(ReturnNode& node) override;
        void visit(BinaryOpNode& node) override;
        void visit(UnaryOpNode& node) override;
        void visit(IdentifierNode& node) override;
        void visit(NumberLiteralNode& node) override;
        void visit(StringLiteralNode& node) override;
        void visit(ArrayAccessNode& node) override;
};

#endif 