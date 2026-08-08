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
#include "ast/ComputedGotoNode.h"
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
#include <unordered_set>
#include <algorithm>

class CodeGenVisitor: public ASTVisitor{
    private:
        std::ostream& os;
        const std::unordered_map<std::string, CommonBlockInfo>* common_blocks;
        const std::vector<TabEntry>* tab;
        bool in_subprogram = false;
        std::unordered_set<std::string> current_params;
        std::string current_func_name;
        std::string current_func_ret_var;
        std::string toLower(std::string s) const{
            std::transform(s.begin(), s.end(), s.begin(), ::tolower);
            return s;
        }
        std::unordered_map<std::string, std::string> common_var_map;
        void scanCommonBlocks(const std::vector<std::unique_ptr<ASTNode>>& body) {
            common_var_map.clear();
            if (!common_blocks) return;
            for (const auto& stmt : body) {
                if (auto cb_node = dynamic_cast<CommonBlockNode*>(stmt.get())) {
                    std::string bname = toLower(cb_node->block_name);
                    for (const auto& pair : *common_blocks) {
                        if (toLower(pair.first) == bname) {
                            const auto& struct_vars = pair.second.variable_names;
                            for (size_t i = 0; i < cb_node->variable_names.size() && i < struct_vars.size(); ++i) {
                                common_var_map[toLower(cb_node->variable_names[i])] = bname + "." + toLower(struct_vars[i]);
                            }
                        }
                    }
                }
            }
        }
        bool isCommonVar(const std::string& v) const{
            std::string lv=toLower(v);
            if(common_var_map.find(lv)!=common_var_map.end())return true;
            if(common_blocks) {
                for(const auto& pair: *common_blocks){
                    for(const auto& cb_var: pair.second.variable_names){
                        if(toLower(cb_var)==lv)return true;
                    }
                }
            }
            return false;
        }

        std::string getVarName(const std::string& name) const {
            std::string lname = toLower(name);
            auto it = common_var_map.find(lname);
            if(it!=common_var_map.end()){
                return it->second;
            }
            if(common_blocks){
                for(const auto& pair:*common_blocks){
                    for(const auto& var: pair.second.variable_names){
                        if(toLower(var)==lname){
                            return toLower(pair.first)+"."+lname;
                        }
                    }
                }
            }
            return lname;
        }

        std::string getCType(const std::string& name) const{
            if(!tab) return "int";
            std::string lname=toLower(name);
            for(const auto& entry: *tab){
                if(toLower(entry.id)==lname&&(entry.obj=="variable"||entry.obj=="array")){
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
            if(common_blocks){
                for(const auto& pair: *common_blocks){
                    os<<"struct "<<toLower(pair.first)<<"_t {\n";
                    for(const auto& var: pair.second.variable_names){
                        os<<"    "<<getCType(var)<<" "<<toLower(var)<<";\n";
                    }
                    os<<"} "<<toLower(pair.first)<<";\n\n";
                }
            }
        }
        std::unordered_map<std::string, std::vector<std::string>> array_dims;
        std::unordered_map<std::string, int> string_lengths;
        std::unordered_set<int> emitted_labels;
        std::unordered_set<int> referenced_labels;
        std::unordered_set<std::string> global_subprogram_names;
        bool hasStringVars(ASTNode* node);
        void collectReferencedLabels(ASTNode* node);
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
        void visit(ComputedGotoNode& node) override;
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