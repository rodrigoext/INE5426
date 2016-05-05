#include "symbol_table.h"

using namespace STab;

extern SymbolTable symtab;

AST::Node* SymbolTable::newVariable(std::string id, AST::Node* next){
    if ( checkId(id) ) yyerror("Variable redefinition! %s\n", id.c_str());
    else {
       Symbol entry(integer, variable, 0, false);
       addSymbol(id,entry);
    }
    return new AST::Variable(id, next);
}

AST::Node* SymbolTable::newVariable(std::string id, AST::Node* next, Type type){
    if ( checkId(id) ) yyerror("Variable redefinition! %s\n", id.c_str());
    else {
        Symbol entry;
        switch (type){
            case type == real:
            entry = Symbol(type, variable, 0.0f, false);
            break;
            case type == integer:
            entry = Symbol(type, variable, 0, false);
            break;
            default:
            yyerror("Variable without type! %s\n", id.c_str());
        }
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

bool SymbolTable::setSymbolType(std::string id, Type type){
    if ( ! checkId(id) ){
        yyerror("Variable not defined yet! %s\n", id.c_str());
        return false;
    }
    entryList[id].type = type;
    return true;
}