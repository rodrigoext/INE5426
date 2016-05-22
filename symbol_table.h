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
		int size_array;
		Symbol() {
			type = indefinido;
			kind = variable;
			initialized = false;
			size_array = 0;
		}
		Symbol(Type type, Kind kind, bool initialized, int size = 0) :
			type(type), kind(kind), initialized(initialized), size_array(size) {  }
		
		void setType(Type t) {
			//sstd::cout << "Type associado" << std::endl;
			type = t;
		}

		void setSize(int size) {
			size_array = size;
		}

		void setKind(Kind k) {
			kind = k;
		}

		void setInicializado() {
			initialized = true;
		}


};

class SymbolTable {
    public:
        SymbolList entryList;
        Type tempType;
        std::string tempLegthArray;
        SymbolTable() {}
        bool checkId(std::string id) {return (entryList.find(id) != entryList.end());}
        void addSymbol(std::string id, Symbol newsymbol) {entryList[id] = newsymbol;}
        void setSimbolType(std::string id, Type t);
        void setSimbolKind(std::string id, Kind k);
        void setSimbolSize(std::string id, int size);
        AST::Node* newVariable(std::string id, Type t, Kind k, AST::Node* next);
        AST::Node* newVariable(std::string id, Type t, Kind k = variable);
        AST::Node* assignVariable(std::string id);
        AST::Node* useVariable(std::string id);
        Symbol getVariable(std::string id);

};

}


