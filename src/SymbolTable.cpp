#include "SymbolTable.h"
#include <iostream>

SymbolTable::SymbolTable(){
    global_scope=std::make_shared<Scope>(nullptr);
    current_scope=global_scope;
}

void SymbolTable::enterScope(){
    auto new_scope=std::make_shared<Scope>(current_scope);
    current_scope=new_scope;
}

void SymbolTable::exitScope(){
    if(current_scope->parent)current_scope=current_scope->parent;
    else std::cerr<<"Warning: Attempted to exit global scope\n";
}

bool SymbolTable::declare(const Symbol& sym){
    if(current_scope->symbols.count(sym.name)){
        return false;
    }
    current_scope->symbols[sym.name]=sym;
    return true;
}

const Symbol* SymbolTable::lookup(const std::string& name)const{
    auto curr=current_scope;
    while(curr!=nullptr){
        auto it=curr->symbols.find(name);
        if(it!=curr->symbols.end())return &(it->second);
        curr=curr->parent;
    }
    return nullptr;
}

const Symbol* SymbolTable::lookupLocal(const std::string& name)const{
    auto it=current_scope->symbols.find(name);
    if(it!=current_scope->symbols.end())return &(it->second);
    return nullptr;
}

bool SymbolTable::isGlobalScope()const{
    return current_scope==global_scope;
}