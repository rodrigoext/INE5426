#pragma once

#include <set>
#include <map>

#include "ast.h"

extern void yyerror (const char *c, ...);

namespace STab {

class Symbol;

enum Type { integer, real, string};
enum Kind { variable, function};

typedef std::map<std::string,Symbol> SymbolList;

class Symbol {
	public:
		Type type;
		Kind kind;
		int64_t value;        
		bool initialized;      
		Symbol(Type type, Kind kind, int64_t value, bool initialized) :
		    type(type), kind(kind), value(value), initialized(initialized) {  }
		Symbol() {
			type = integer; 
			kind = variable; 
			value = 0; 
			initialized = false;}
};

class SymbolTable {
    public:
        SymbolList entryList;
        SymbolTable() {}
        bool checkId(std::string id) {return entryList.find(id) != entryList.end();}
        void addSymbol(std::string id, Symbol newsymbol) {entryList[id] = newsymbol;}
        AST::Node* newVariable(std::string id, AST::Node* next);
        AST::Node* assignVariable(std::string id);
        AST::Node* useVariable(std::string id);
};

}


