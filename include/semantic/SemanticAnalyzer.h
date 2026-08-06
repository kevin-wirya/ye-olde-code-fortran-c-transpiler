#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "ASTVisitor.h"
#include "SymbolTable.h"
#include <string>
#include <vector>
#include <unordered_map>

struct CommonBlockInfo{
    std::string name;
    std::vector<std::string> variable_names;
};

struct TabEntry {
    int idx;
    std::string id;
    std::string obj;
    std::string type;
    int ref = 0;
    int nrm = 1;
    int lev = 0;
    int adr = 0;
    int link = 0;
};

struct BtabEntry {
    int idx;
    std::string name;
    int last = 0;
    int lpar = 0;
    int psze = 0;
    int vsze = 0;
};

struct AtabEntry {
    int idx;
    std::string name;
    std::string xtyp;
    std::string etyp;
    int eref = 0;
    int low = 1;
    int high = 1;
    int elsz = 4;
    int size = 4;
};

class SemanticAnalyzer : public ASTVisitor {
private:
  SymbolTable symbol_table;
  std::vector<std::string> errors;
  bool has_implicit_none;
  std::unordered_map<std::string,CommonBlockInfo> global_common_blocks;

  std::vector<TabEntry> tab;
  std::vector<BtabEntry> btab;
  std::vector<AtabEntry> atab;
  int current_level = 0;
  int current_btab_idx = 0;
  int current_offset = 0;

  void initPredefinedSymbolTables();
  void reportError(const std::string &msg);
  std::string getInferredType(ASTNode *node);

public:
  SemanticAnalyzer();
  bool analyze(ASTNode &root);
  const std::vector<std::string> &getErrors() const { return errors; }
  void printSymbolTables(std::ostream &os) const;
  void visit(ProgramNode &node) override;
  void visit(SubroutineNode &node) override;
  void visit(FunctionNode &node) override;
  void visit(ImplicitNoneNode &node) override;
  void visit(TypeDeclNode &node) override;
  void visit(ArrayDeclNode &node) override;
  void visit(CommonBlockNode &node) override;
  void visit(AssignNode &node) override;
  void visit(IfNode &node) override;
  void visit(DoNode &node) override;
  void visit(GotoNode &node) override;
  void visit(ContinueNode &node) override;
  void visit(NumberLiteralNode &node) override;
  void visit(StringLiteralNode &node) override;
  void visit(IdentifierNode &node) override;
  void visit(ArrayAccessNode &node) override;
  void visit(BinaryOpNode &node) override;
  void visit(UnaryOpNode &node) override;
  void visit(PrintNode &node) override;
  void visit(ReadNode &node) override;
  void visit(CallNode &node) override;
  void visit(ReturnNode &node) override;
};

#endif