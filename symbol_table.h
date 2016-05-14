#pragma once

#include <set>
#include <map>

#include "ast.h"
#include "types.h"

extern void yyerror (const char *c, ...);

namespace STab {

class Symbol;

typedef std::map<std::string,Symbol> SymbolList;

class Symbol {
	public:
		Type type;
		Kind kind;
		bool initialized;      
		Symbol() {
			type = indefinido;
			kind = variable;
			initialized = false;
		}
		Symbol(Type type, Kind kind, bool initialized) :
			type(type), kind(kind), initialized(initialized) {  }
		
		void setType(Type t) {
			//sstd::cout << "Type associado" << std::endl;
			type = t;
		}

		void setInicializado() {
			initialized = true;
		}


};

class SymbolTable {
    public:
        SymbolList entryList;
        Type tempType;
        SymbolTable() {}
        bool checkId(std::string id) {return (entryList.find(id) != entryList.end());}
        void addSymbol(std::string id, Symbol newsymbol) {entryList[id] = newsymbol;}
        void setSimbolType(std::string id, Type t);
        // AST::Node* newVariable(std::string id, AST::Node* next);
        AST::Node* newVariable(std::string id, Type t);
        AST::Node* assignVariable(std::string id);
        AST::Node* useVariable(std::string id);
        Symbol getVariable(std::string id);

};

}


