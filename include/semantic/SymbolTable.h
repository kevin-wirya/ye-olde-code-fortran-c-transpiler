#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "Symbol.h"
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

class Scope{
    public:
        std::unordered_map<std::string, Symbol> symbols;
        std::shared_ptr<Scope> parent;
        Scope(std::shared_ptr<Scope> p=nullptr): parent(p){}
};

class SymbolTable{
    private:
        std::shared_ptr<Scope> current_scope;
        std::shared_ptr<Scope> global_scope;
    public:
        SymbolTable();
        void enterScope();
        void exitScope();
        bool declare(const Symbol& sym);
        bool updateType(const std::string& name,const std::string& type);
        const Symbol* lookup(const std::string& name)const;
        const Symbol* lookupLocal(const std::string& name)const;
        bool isGlobalScope()const;
};

#endif