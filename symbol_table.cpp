#include "symbol_table.h"

using namespace STab;

extern SymbolTable symtab;


AST::Node* SymbolTable::newVariable(std::string id, AST::Node* next){
	if ( checkId(id) ) yyerror("Variable redefinition! %s\n", id.c_str());
	    else {
	       Symbol entry(indefinido, variable, false);
	       addSymbol(id,entry);
	    }
	return new AST::Variable(id, indefinido);
}

AST::Node* SymbolTable::newVariable(std::string id, Type t){
    if ( checkId(id) ) yyerror("Variable redefinition! %s\n", id.c_str());
    else {
       Symbol entry(t, variable, false);
       addSymbol(id,entry);
    }
    return new AST::Variable(id, t);
}

AST::Node* SymbolTable::assignVariable(std::string id){
    if ( ! checkId(id) ) yyerror("Variable not defined yet! %s\n", id.c_str());
    entryList[id].initialized = true;
    return new AST::Variable(id, symtab.tempType); //Creates variable node anyway
}

AST::Node* SymbolTable::useVariable(std::string id){
    if ( ! checkId(id) ) yyerror("Variable not defined yet! %s\n", id.c_str());
    if ( ! entryList[id].initialized ) yyerror("Variable not initialized yet! %s\n", id.c_str());
    return new AST::Variable(id, entryList[id].type); //Creates variable node anyway
}

Symbol SymbolTable::getVariable(std::string id){
	return entryList[id];
}

void SymbolTable::setSimbolType(std::string id, Type t) {
    entryList[id].setType(t);
}
