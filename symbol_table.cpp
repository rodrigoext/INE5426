#include "symbol_table.h"

using namespace STab;

extern SymbolTable symtab;

AST::Node* SymbolTable::newVariable(std::string id, AST::Node* next){
    if ( checkId(id) ) yyerror("Variable redefinition! %s\n", id.c_str());
    else {
        Symbol entry;
        switch (temp_) {
            case integer:
            entry = Symbol(integer, variable, 0, false);
            addSymbol(id,entry);
            std::cout << "novo inteiro" << std::endl;
            break;
            case real:
            std::cout << "novo real" << std::endl;
            default:
            std::cout << "erro new variable" << std::endl;
            break;
        }
       //Symbol entry(integer, variable, 0, false);
       //addSymbol(id,entry);
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

void SymbolTable::setTempType(Type temp){
    temp_ = temp;
}