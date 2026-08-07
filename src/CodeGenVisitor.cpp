#include "codegen/CodeGenVisitor.h"

void CodeGenVisitor::visit(ProgramNode &node) {
  os << "int main(void){\n";
  for (auto &stmt : node.body) {
    if (stmt)
      stmt->accept(*this);
  }
  os << "    return 0;\n";
  os << "}\n";
}

void CodeGenVisitor::visit(SubroutineNode &node) {
  os << "void " << node.name << "(";
  for (size_t i = 0; i < node.parameters.size(); ++i) {
    os << getCType(node.parameters[i]) << "* " << node.parameters[i]
       << (i + 1 < node.parameters.size() ? ", " : "");
  }
  os << "){\n";
  for (auto &stmt : node.body) {
    if (stmt)
      stmt->accept(*this);
  }
  os << "}\n\n";
}

void CodeGenVisitor::visit(FunctionNode &node) {
  std::string ret = "int";
  if (node.returnType == "REAL")
    ret = "float";
  else if (node.returnType == "LOGICAL")
    ret = "bool";
  os << ret << " " << node.name << "(";
  for (size_t i = 0; i < node.parameters.size(); ++i) {
    os << getCType(node.parameters[i]) << "* " << node.parameters[i]
       << (i + 1 < node.parameters.size() ? ", " : "");
  }
  os << "){\n";
  for (auto &stmt : node.body) {
    if (stmt)
      stmt->accept(*this);
  }
  os << "}\n\n";
}

void CodeGenVisitor::visit(ImplicitNoneNode &node) {
  // tidak menghasilkan kode
}

void CodeGenVisitor::visit(TypeDeclNode &node) {
  if (node.line > 0)
    os << "    // line " << node.line << "\n";
  std::string ctype = "int";
  std::string char_suffix = "";
  if (node.type_name == "REAL")
    ctype = "float";
  else if (node.type_name == "LOGICAL")
    ctype = "bool";
  else if (node.type_name.rfind("CHARACTER", 0) == 0) {
    ctype = "char";
    int len = 1;
    if (node.type_name.size() > 9 && node.type_name[9] == '*') {
      len = std::stoi(node.type_name.substr(10));
    }
    char_suffix = "[" + std::to_string(len + 1) + "]";
    for (const auto &v : node.variable_names) {
      string_lengths[v] = len;
    }
  }
  os << "    " << ctype << " ";
  for (size_t i = 0; i < node.variable_names.size(); ++i) {
    os << node.variable_names[i] << char_suffix;
    os << (i + 1 < node.variable_names.size() ? ", " : "");
  }
  os << ";\n";
  if (ctype == "char") {
    for (const auto &v : node.variable_names) {
      os << "    " << v << "[0] = '\\0';\n";
    }
  }
}

void CodeGenVisitor::visit(ArrayDeclNode &node) {
  if (node.line > 0)
    os << "    // line " << node.line << "\n";
  std::string ctype = "int";
  int char_len = -1;
  if (node.type_name == "REAL")
    ctype = "float";
  else if (node.type_name == "LOGICAL")
    ctype = "bool";
  else if (node.type_name.rfind("CHARACTER", 0) == 0) {
    ctype = "char";
    char_len = 1;
    if (node.type_name.size() > 9 && node.type_name[9] == '*') {
      char_len = std::stoi(node.type_name.substr(10));
    }
    string_lengths[node.array_name] = char_len;
  }
  int total_size = 1;
  std::vector<std::string> dims;
  for (const auto &dim : node.dimensions) {
    try {
      total_size *= std::stoi(dim.upper_bound);
    } catch (...) {
      total_size *= 10;
    }
    dims.push_back(dim.upper_bound);
  }
  array_dims[node.array_name] = dims;
  if (char_len > 0) {
    os << "    " << ctype << " " << node.array_name << "[" << total_size << "]["
       << (char_len + 1) << "];\n";
  } else {
    os << "    " << ctype << " " << node.array_name << "[" << total_size
       << "];\n";
  }
}

void CodeGenVisitor::visit(CommonBlockNode &node) {
  // handled in CodeGenVisitor constructor
}

void CodeGenVisitor::visit(IfNode &node) {
  if (node.line > 0)
    os << "    // line " << node.line << "\n";
  os << "    if(";
  if (node.condition)
    node.condition->accept(*this);
  os << "){\n";
  for (auto &stmt : node.then_body) {
    if (stmt)
      stmt->accept(*this);
  }
  os << "    }";
  if (!node.else_body.empty()) {
    os << " else {\n";
    for (auto &stmt : node.else_body) {
      if (stmt)
        stmt->accept(*this);
    }
    os << "    }";
  }
  os << "\n";
}

void CodeGenVisitor::visit(DoNode &node) {
  if (node.line > 0)
    os << "    // line " << node.line << "\n";
  os << "    for(" << node.loop_variable << "=";
  if (node.start_expr)
    node.start_expr->accept(*this);
  os << "; " << node.loop_variable << "<=";
  if (node.end_expr)
    node.end_expr->accept(*this);
  os << "; " << node.loop_variable << "+=";
  if (node.step_expr)
    node.step_expr->accept(*this);
  else
    os << "1";
  os << "){\n";
  for (auto &stmt : node.body) {
    if (stmt)
      stmt->accept(*this);
  }
  os << "    }\n";
}

void CodeGenVisitor::visit(AssignNode &node) {
  if (node.line > 0)
    os << "    // line " << node.line << "\n";
  if (string_lengths.find(node.target_variable) != string_lengths.end()) {
    int len = string_lengths[node.target_variable];
    os << "    F77_STR_ASSIGN(" << node.target_variable;
    if (!node.index_expressions.empty()) {
      os << "[";
      printFlattenedIndex(node.target_variable, node.index_expressions);
      os << "]";
    }
    os << ", ";
    if (node.expression)
      node.expression->accept(*this);
    os << ", " << len << ");\n";
  } else {
    os << "    " << node.target_variable;
    if (!node.index_expressions.empty()) {
      os << "[";
      printFlattenedIndex(node.target_variable, node.index_expressions);
      os << "]";
    }
    os << " = ";
    if (node.expression)
      node.expression->accept(*this);
    os << ";\n";
  }
}

void CodeGenVisitor::visit(PrintNode &node) {
  if (node.line > 0)
    os << "    // line " << node.line << "\n";
  os << "    printf(\"";
  std::string fmt = "";
  for (size_t i = 0; i < node.expressions.size(); ++i) {
    if (dynamic_cast<StringLiteralNode *>(node.expressions[i].get())) {
      fmt += "%s";
    } else if (auto ident =
                   dynamic_cast<IdentifierNode *>(node.expressions[i].get())) {
      std::string type = getCType(ident->name);
      if (type == "float")
        fmt += "%f";
      else if (type == "char")
        fmt += "%s";
      else
        fmt += "%d";
    } else {
      fmt += "%d";
    }
    if (i + 1 < node.expressions.size())
      fmt += " ";
  }
  os << fmt << "\\n\"";
  for (size_t i = 0; i < node.expressions.size(); ++i) {
    os << ", ";
    node.expressions[i]->accept(*this);
  }
  os << ");\n";
}

void CodeGenVisitor::visit(ReadNode &node) {
  if (node.line > 0)
    os << "    // line " << node.line << "\n";
  os << "    scanf(\"";
  std::string fmt = "";
  for (size_t i = 0; i < node.variables.size(); ++i) {
    std::string type = getCType(node.variables[i]);
    if (type == "float")
      fmt += "%f";
    else if (type == "char")
      fmt += "%s";
    else
      fmt += "%d";
    if (i + 1 < node.variables.size())
      fmt += " ";
  }
  os << fmt << "\"";
  for (size_t i = 0; i < node.variables.size(); ++i) {
    os << ", ";
    if (getCType(node.variables[i]) != "char")
      os << "&";
    os << node.variables[i];
  }
  os << ");\n";
}

void CodeGenVisitor::visit(CallNode &node) {
  if (node.line > 0)
    os << "    // line " << node.line << "\n";
  os << "    " << node.subroutine_name << "(";
  for (size_t i = 0; i < node.arguments.size(); ++i) {
    os << "&";
    node.arguments[i]->accept(*this);
    os << (i + 1 < node.arguments.size() ? ", " : "");
  }
  os << ");\n";
}

void CodeGenVisitor::visit(GotoNode &node) {
  if (node.line > 0)
    os << "    // line " << node.line << "\n";
  os << "    goto label_" << node.target_label << ";\n";
}

void CodeGenVisitor::visit(ComputedGotoNode &node) {
  if (node.line > 0)
    os << "    // line " << node.line << "\n";
  os << "    switch(";
  if (node.selector_expr)
    node.selector_expr->accept(*this);
  os << "){\n";
  for (size_t i = 0; i < node.labels.size(); ++i) {
    os << "        case " << (i + 1) << ": goto label_" << node.labels[i]
       << "; break;\n";
  }
  os << "    }\n";
}

void CodeGenVisitor::visit(ReturnNode &node) {
  if (node.line > 0)
    os << "    // line " << node.line << "\n";
  os << "    return;\n";
}

void CodeGenVisitor::visit(ContinueNode &node) {
  if (node.line > 0)
    os << "    // line " << node.line << "\n";
  os << "label_" << node.label << ":;\n";
}

void CodeGenVisitor::visit(BinaryOpNode &node) {
  os << "(";
  if (node.left)
    node.left->accept(*this);
  std::string cop = node.op;
  if (cop == ".AND.")
    cop = "&&";
  else if (cop == ".OR.")
    cop = "||";
  else if (cop == ".EQ.")
    cop = "==";
  else if (cop == ".NE.")
    cop = "!=";
  else if (cop == ".GT.")
    cop = ">";
  else if (cop == ".GE.")
    cop = ">=";
  else if (cop == ".LT.")
    cop = "<";
  else if (cop == ".LE.")
    cop = "<=";
  os << " " << cop << " ";
  if (node.right)
    node.right->accept(*this);
  os << ")";
}

void CodeGenVisitor::visit(UnaryOpNode &node) {
  std::string cop = node.op;
  if (cop == ".NOT.")
    cop = "!";
  os << cop;
  if (node.operand)
    node.operand->accept(*this);
}

void CodeGenVisitor::visit(IdentifierNode &node) { os << node.name; }

void CodeGenVisitor::visit(NumberLiteralNode &node) { os << node.value; }

void CodeGenVisitor::visit(StringLiteralNode &node) {
  std::string val = node.value;
  if (val == ".TRUE.")
    os << "true";
  else if (val == ".FALSE.")
    os << "false";
  else if (val.size() >= 2 && val.front() == '\'' && val.back() == '\'') {
    os << "\"" << val.substr(1, val.size() - 2) << "\"";
  } else {
    os << val;
  }
}

void CodeGenVisitor::visit(ArrayAccessNode &node) {
  std::string upper_name = node.array_name;
  for (auto &c : upper_name)
    c = toupper(c);

  if (upper_name == "ABS" || upper_name == "IABS") {
    os << "abs(";
    if (!node.indices.empty())
      node.indices[0]->accept(*this);
    os << ")";
  } else if (upper_name == "FABS" || upper_name == "ABS") {
    os << "fabs(";
    if (!node.indices.empty())
      node.indices[0]->accept(*this);
    os << ")";
  } else if (upper_name == "SQRT") {
    os << "sqrt(";
    if (!node.indices.empty())
      node.indices[0]->accept(*this);
    os << ")";
  } else if (upper_name == "SIN") {
    os << "sin(";
    if (!node.indices.empty())
      node.indices[0]->accept(*this);
    os << ")";
  } else if (upper_name == "COS") {
    os << "cos(";
    if (!node.indices.empty())
      node.indices[0]->accept(*this);
    os << ")";
  } else if (upper_name == "TAN") {
    os << "tan(";
    if (!node.indices.empty())
      node.indices[0]->accept(*this);
    os << ")";
  } else if (upper_name == "EXP") {
    os << "exp(";
    if (!node.indices.empty())
      node.indices[0]->accept(*this);
    os << ")";
  } else if (upper_name == "LOG" || upper_name == "ALOG") {
    os << "log(";
    if (!node.indices.empty())
      node.indices[0]->accept(*this);
    os << ")";
  } else if (upper_name == "MOD") {
    if (node.indices.size() >= 2) {
      os << "(";
      node.indices[0]->accept(*this);
      os << " % ";
      node.indices[1]->accept(*this);
      os << ")";
    }
  } else {
    os << node.array_name << "[";
    printFlattenedIndex(node.array_name, node.indices);
    os << "]";
  }
}

void CodeGenVisitor::printFlattenedIndex(
    const std::string &array_name,
    const std::vector<std::unique_ptr<ASTNode>> &indices) {
  if (array_dims.find(array_name) != array_dims.end() && indices.size() > 1) {
    const auto &dims = array_dims[array_name];
    os << "(";
    for (size_t i = 0; i < indices.size(); ++i) {
      if (i > 0)
        os << "+";
      os << "((";
      indices[i]->accept(*this);
      os << ")-1)";
      for (size_t j = 0; j < i; ++j) {
        if (j < dims.size()) {
          os << "*" << dims[j];
        }
      }
    }
    os << ")";
  } else {
    os << "(";
    if (!indices.empty())
      indices[0]->accept(*this);
    os << " - 1)";
  }
}