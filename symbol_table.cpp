#include "symbol_table.h"

using namespace STab;

extern SymbolTable symtab;

AST::Node* SymbolTable::newVariable(std::string id, AST::Node* next){
    if ( checkId(id) ) yyerror("Variable redefinition! %s\n", id.c_str());
    else {
        //std::cout << "simbolo " << id << " adicionado" << std::endl;
       Symbol entry;
       addSymbol(id,entry);
    }
    return new AST::Variable(id, next);
}

AST::Node* SymbolTable::assignVariable(std::string id){
    if ( ! checkId(id) ) yyerror("Variable not defined yet! %s\n", id.c_str());
    entryList[id].initialized = true;
    return new AST::Variable(id, NULL); //Creates variable node anyway
}

AST::Node* SymbolTable::useVariable(std::string id){
    if ( ! checkId(id) ) yyerror("Variable not defined yet! %s\n", id.c_str());
    if ( ! entryList[id].initialized ) yyerror("Variable not initialized yet! %s\n", id.c_str());
    return new AST::Variable(id, NULL); //Creates variable node anyway
}

void SymbolTable::setSimbolType(std::string id, Type t) {
    entryList[id].setType(t);
}
