#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "ASTVisitor.h"
#include "SymbolTable.h"
#include <string>
#include <vector>


class SemanticAnalyzer : public ASTVisitor {
private:
  SymbolTable symbol_table;
  std::vector<std::string> errors;
  bool has_implicit_none;
  void reportError(const std::string &msg);

public:
  SemanticAnalyzer();
  bool analyze(ASTNode &root);
  const std::vector<std::string> &getErrors() const { return errors; }
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