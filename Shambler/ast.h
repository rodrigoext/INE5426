#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "types.h"

extern void yyerror(const char* s, ...);

namespace AST {

class Node;

typedef std::vector<Node*> NodeList; //List of ASTs

class Node {
    public:
        Type type;
        Kind kind;
        virtual ~Node() { }
        Node() {
        	this->type = indefinido;
        	this->kind = variable;
        }
        Node(Type t) : type(t) { }
        virtual void printTree(){ }
        virtual int computeTree(){return 0;}


};

class Integer : public Node {
    public:
        int value;
        Integer(int value) : value(value) {  }
        void printTree();
        //int computeTree();
};

class Real : public Node {
     public:
        float value;
        Real(float value) : value(value) {  }
};

class Variable : public Node {
     public:
         std::string id;
         Kind kind;
         Node * next;
         bool parameter, strong;
         Node * parameters;
         Variable(std::string id, Type t, bool strong = false, Kind k = variable, bool parameter = false, Node * parameters = NULL) :
            id(id), kind(k) {
        	 	 this->next = NULL;
        	 	 this->type = t;
             this->strong = strong;
        	 	 this->parameter = parameter;
        	 	 this->parameters = parameters;
            }
         void setNext(Node * next) {
        	 this->next = next;
         }
         void setType(Type t) {
         	this->type = t;
         }
         void setKind(Kind k) {
           this->kind = k;
         }
         void SetParametros(Node * parameters) {
        	 this->parameters = parameters;
         }
         void printTree();
         int computeTree();
};

class BinOp : public Node {
    public:
        Operation op;
        Node *left;
        Node *right;
        Node *array_exp;
        BinOp(Node *left, Operation op, Node *right, Node *array_exp = NULL) :
            left(left), right(right), op(op), array_exp(array_exp) {
        	switch (op) {
        	case associa:
        		if (left->type != right->type) {
        			yyerror(("semantico: operacao " + op_name[op] + " espera " + type_name_masc[left->type] +
        					" mas recebeu " + type_name_masc[right->type] + ".").c_str());
        		}
        		break;
        	case soma:
        	case divide:
        	case subtrai:
        	case multiplica:
        		if (right->type == indefinido) {
        			yyerror(("semantico: operacao " + op_name[op] + " espera inteiro ou real mas recebeu " + type_name_masc[right->type] + ".").c_str());
        		}
        		this->type = left->type;
        		if (left->type == real && right->type == real)
        			this->type = real;
        		break;
        	case igual:
        	case diferente:
        	case maior:
        	case maior_igual:
        	case menor:
        	case menor_igual:
        	case e_logico:
        	case ou_logico:
        		/*if (right->type != booleano) {
        			yyerror(("semantico: operacao " + op_name[op] + " espera " + type_name_masc[left->type] +
        			        					" mas recebeu " + type_name_masc[right->type] + ".").c_str());
        		}*/
        		this->type = booleano;
        		break;
        	default:
        		this->type = indefinido;
        		break;
        	}
        }
        void printTree();
        int computeTree();
};

class UnOp : public Node {
    public:
        Operation op;
        Node *next;
        UnOp(Node *next, Operation op) :
            next(next), op(op) {
        	switch (op) {
        	case subtrai:
        		this->type = inteiro;
        		if (next->type == inteiro || next->type == real)
        			this->type = next->type;
        		break;
        	case negacao:
        		this->type = booleano;
        		break;
        	case parenteses:
        		this->type = next->type;
        		break;
        	default:
        		this->type = indefinido;
        	}
            }
        void printTree();
};

class AssignOp : public Node {
    public:
        Node *left;
        Node *right;
        AssignOp(Node *left, Node *right):
            left(left), right(right) {

        }
        void printTree();
};

class Block : public Node {
    public:
        NodeList lines;
        Block() { }
        void printTree();
        int computeTree();
};

class Number : public Node {
	 public:
		std::string value;
		Node * next;
		Number(std::string value, Type t) :
			value(value) {
			this->next = NULL;
			this->type = t;
		}
		void setNext(Node * node) {
			this->next = node;
		}
		void printTree();
    int computeTree();
};

class VarDeclaration : public Node {
     public:
        NodeList vars;
        Number *tamanho;
        bool parameter, strong;
        VarDeclaration(Type t, bool strong = false, Kind k = variable, bool parameter = false) {
        	this->type = t;
        	this->kind = k;
        	this->tamanho = NULL;
          this->strong = strong;
        	this->parameter = parameter;
        }
        void setType(Type t) {
        	this->type = t;
        	setTypeVars(t);
        }
        void setKind(Kind k) {
          this->kind = k;
        }
        void setTypeVars(Type t) {
        }
        void setTamanho(Number * tamanho) {
        	this->tamanho = tamanho;
        }
        void printTree();
        int computeTree();
};

class ConditionalExp : public Node {
	public:
		Node * condition;
		Node * next;
		Node * senao;
		ConditionalExp(Node * condition, Node *next) :
			condition(condition), next(next) {
			this->type = indefinido;
			this->senao = NULL;
		}
		void SetSenao(Node * senao) {
			this->senao = senao;
		}
		void printTree();
};

class LoopExp : public Node {
	public:
		Node * condition;
		Node * next;
		Node * conditionFor;
		bool forExp;
		bool decrement;
		LoopExp(Node *condition, Node *next, bool forExp = false, bool decrement = false) :
			condition(condition), next(next), forExp(forExp), decrement(decrement) {
		}
		void SetConditionFor(Node *next) {
			this->conditionFor = next;
		}
		void printTree();
};

class ParameterDeclaration : public Node {
	public:
		NodeList params;
		ParameterDeclaration() {
		}
		void printTree();
};

class FunctionDeclaration : public Node {
	public:
		std::string id;
		Node * parametros;
		Node * next;
		Node * retorno;
		Type type;
		FunctionDeclaration(std::string id, Node *parametros, Node *next, Node *retorno, Type t) :
			id(id), parametros(parametros), next(next), retorno(retorno), type(t) {
			if (retorno != NULL) {
				if (retorno->type != this->type) {
					yyerror(("semantico: funcao " + id + " espera retorno do tipo " + type_name_masc[t]).c_str());
				}
			}
		}
		void SetParametros(Node *parametros) {
			this->parametros = parametros;
		}
		void printTree();
};

class FindExpr : public Node {
  public:
    std::string id;
    Node * next;
    Type type;
    FindExpr(std::string id, Node * next, Type t) :
      id(id), next(next), type(t) {

      }
    void printTree();
};
}
