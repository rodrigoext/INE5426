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
		bool initialized;      
		Symbol() {
			type = indefinido;
			initialized = false;
		}
		
		Symbol(Type type, bool initialized) :
		    type(type), initialized(initialized) {  }
		
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
        SymbolTable() {}
        bool checkId(std::string id) {return (entryList.find(id) != entryList.end() &&
        									  entryList.find(id)->second.type != indefinido);}
        void addSymbol(std::string id, Symbol newsymbol) {entryList[id] = newsymbol;}
        void setSimbolType(std::string id, Type t);
        AST::Node* newVariable(std::string id, AST::Node* next);
        AST::Node* assignVariable(std::string id);
        AST::Node* useVariable(std::string id);
    private:
    	Type temp_;
};

}


