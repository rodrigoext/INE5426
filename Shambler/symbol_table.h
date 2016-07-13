#pragma once

#include <set>
#include <map>

#include "ast.h"
#include "types.h"

extern void yyerror (const char *c, ...);

namespace STab {

class Symbol;

typedef std::map<std::string,Symbol> SymbolList;
typedef std::vector<SymbolList> BlockSymbolList;

class Symbol {
	public:
		Type type;
		Kind kind;
		bool initialized;
		int size_array;
		bool funcDeclarada;
		bool strong;
		double values;
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
		void setValues(double v) {
			values = v;
		}
		double getValueAt(int pos) {
			return values;
		}
};

class SymbolTable {
    public:
        SymbolList entryList;
        Type tempType;
        Type tempTypeFunc;
		bool strong, declared;
		int scopePos;
        std::string tempLegthArray;
        SymbolTable() {}
        bool checkId(std::string id) {return (entryList.find(id) != entryList.end());}
        void addSymbol(std::string id, Symbol newsymbol) {entryList[id] = newsymbol;}
        bool setSymbolType(std::string id, Type t);
        void setSymbolInitialized(std::string id, bool init = true);
        void setSymbolKind(std::string id, Kind k);
        void setSymbolSize(std::string id, int size);
		void setSymbolStrong(std::string id);
        void setSymbolTypeString(std::string id, Type t);
        void setFunctionDeclared(std::string id);
		void setSymbolValues(std::string id, double val);
		double getSymbolValueAtPosition(std::string id, int pos = 0);
        AST::Node* newVariable(std::string id, Type t, Kind k, AST::Node* next);
        AST::Node* newVariable(std::string id, Type t, bool strong = false, Kind k = variable, bool parameter = false);
        AST::Node* newFunction(std::string id, Type t, Kind k = function,  AST::Node* parametros = NULL, AST::Node * lines = NULL, AST::Node * ret = NULL, bool declarada = false);
        AST::Node* assignVariable(std::string id);
        AST::Node* useVariable(std::string id);
		AST::Node* usetFunction(std::string id);
        Symbol getVariable(std::string id);

};

}
