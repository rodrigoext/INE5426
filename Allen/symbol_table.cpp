#include "symbol_table.h"

using namespace STab;

extern SymbolTable symtab;


AST::Node* SymbolTable::newVariable(std::string id, Type t, Kind k, AST::Node* next){
	if ( checkId(id) ) yyerror("Variable redefinition! %s\n", id.c_str());
	    else {
	       Symbol entry(t, k, false);
	       addSymbol(id,entry);
	    }
	AST::Variable *v = new AST::Variable(id, indefinido);
	v->setNext(next);
	return v;
}

AST::Node* SymbolTable::newVariable(std::string id, Type t, Kind k, bool parameter){
    if ( checkId(id) ) {
    	yyerror("semantico: variavel %s sofrendo redefinicao.\n", id.c_str());
    }
	if (k == array) {
		Symbol entry(t, k, true);
		addSymbol(id,entry);
	} else {
		Symbol entry(t, k, false);
		addSymbol(id,entry);
	}
    return new AST::Variable(id, t, k, parameter);
}

AST::Node* SymbolTable::newFunction(std::string id, Type t, Kind k, AST::Node* parametros, bool declarada) {
	if ( checkId(id) ) {
		 if (entryList[id].funcDeclarada) {
			 yyerror("semantico: funcao %s sofrendo redeclaracao.\n", id.c_str());
		 }
	 }
	 Symbol entry(t, k, false, 0, declarada);
	 addSymbol(id, entry);
	 return new AST::FunctionDeclaration(id, parametros, NULL, NULL, t);
}

AST::Node* SymbolTable::assignVariable(std::string id){
    if ( ! checkId(id) ){
    	yyerror("semantico: variavel %s sem declaracao.\n", id.c_str());
    	return new AST::Variable(id, indefinido);
    }
    entryList[id].initialized = true;
    return new AST::Variable(id, entryList[id].type, entryList[id].kind); //Creates variable node anyway
}

AST::Node* SymbolTable::useVariable(std::string id){
    if ( ! checkId(id) ) {
    	yyerror("semantico: variavel %s sem declaracao.\n", id.c_str());
    	return new AST::Variable(id, indefinido);
    }
    if ( ! entryList[id].initialized && entryList[id].kind != function ) yyerror("semantico: variavel %s nao inicializada.\n", id.c_str());
   	return new AST::Variable(id, entryList[id].type, entryList[id].kind); //Creates variable node anyway
}

Symbol SymbolTable::getVariable(std::string id){
	return entryList[id];
}

void SymbolTable::setSimbolType(std::string id, Type t) {
    entryList[id].setType(t);
}

void SymbolTable::setSimbolKind(std::string id, Kind k) {
	entryList[id].setKind(k);
}

void SymbolTable::setSimbolSize(std::string id, int size) {
	if (size < 1)
		yyerror(("semantico: arranjo " + id + " com tamanho menor do que um.").c_str());
	entryList[id].setSize(size);
}

void SymbolTable::setFunctionDeclared(std::string id) {
	entryList[id].SetFuncaoDecladara();
}
