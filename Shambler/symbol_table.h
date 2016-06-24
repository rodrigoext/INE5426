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
		bool funcDeclarada;
		bool strong;
		Symbol() {
			type = indefinido;
			kind = variable;
			initialized = false;
			size_array = 0;
			funcDeclarada = false;
			strong = false;
		}
		Symbol(Type type, Kind kind, bool initialized, int size = 0, bool funcDeclarada = false) :
			type(type), kind(kind), initialized(initialized), size_array(size), funcDeclarada(funcDeclarada) {  }
		void setType(Type t) {
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
		void SetFuncaoDecladara() {
			funcDeclarada = true;
		}
		void setStrong() {
			strong = true;
		}
};

class SymbolTable {
    public:
        SymbolList entryList;
        Type tempType;
        Type tempTypeFunc;
        std::string tempLegthArray;
        SymbolTable() {}
        bool checkId(std::string id) {return (entryList.find(id) != entryList.end());}
        void addSymbol(std::string id, Symbol newsymbol) {entryList[id] = newsymbol;}
        void setSymbolType(std::string id, Type t);
        void setSymbolInitialized(std::string id, bool init = true);
        void setSymbolKind(std::string id, Kind k);
        void setSymbolSize(std::string id, int size);
        void setSymbolTypeString(std::string id, Type t);
        void setFunctionDeclared(std::string id);
        AST::Node* newVariable(std::string id, Type t, Kind k, AST::Node* next);
        AST::Node* newVariable(std::string id, Type t, Kind k = variable, bool parameter = false);
        AST::Node* newFunction(std::string id, Type t, Kind k = function, AST::Node* parametros = NULL, bool declarada = false);
        AST::Node* assignVariable(std::string id);
        AST::Node* useVariable(std::string id);
        Symbol getVariable(std::string id);

};

}


