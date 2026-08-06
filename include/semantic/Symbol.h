#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <vector>
#include "ArrayDeclNode.h"

enum class SymbolKind{
    VARIABLE,
    ARRAY,
    FUNCTION,
    SUBROUTINE
};

struct Symbol{
    std::string name;
    std::string type;
    SymbolKind kind;
    std::vector<ArrayDimension> dimensions;
    std::vector<std::string> param_types;
    int arity=0;
    bool is_common=false;
    std::string common_block_name="";
    Symbol()=default;
    Symbol(const std::string& n,const std::string& t, SymbolKind k)
        :name(n),type(t),kind(k){}
};

#endif