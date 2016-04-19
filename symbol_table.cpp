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