#pragma once

#include <iostream>
#include <vector>

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
        //virtual int computeTree(){return 0;}
        

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
        		this->type = left->type;
        		// TODO: checar se é uma variável.
        		//if (left->type != right->type) {
				//	yyerror(("semantico: operacao atribuicao espera " + type_name_masc[left->type] +
				//			" mas recebeu " + type_name_masc[right->type] + ".").c_str());
				//}
        		break;
        	case soma:
        	case divide:
        	case subtrai:
        	case multiplica:
        		this->type = inteiro;
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
        		this->type = booleano;
        		break;
        	default:
        		this->type = indefinido;
        		break;
        	}
        }
        void printTree();
        //int computeTree();
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
        //int computeTree();
};

class Block : public Node {
    public:
        NodeList lines;
        Block() { }
        void printTree();
        //int computeTree();
};

class Variable : public Node {
     public:
         std::string id;
         Kind kind;
         Node * next;
         Variable(std::string id, Type t, Kind k = variable) :
            id(id), kind(k) {
        	 	 this->next = NULL;
        	 	 this->type = t;
                //std::cout << "tipo nodo setado" << std::endl;
            }
         void setNext(Node * next) {
        	 this->next = next;
         }
         void printTree();
         //int computeTree();
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
};

class VarDeclaration : public Node {
     public:
        NodeList vars;
        Number *tamanho;
        VarDeclaration(Type t, Kind k = variable) {
        	this->type = t;
        	this->kind = k;
        	this->tamanho = NULL;
        }
        void setType(Type t) {
        	this->type = t;
        }
        void setTamanho(Number * tamanho) {
        	this->tamanho = tamanho;
        }
        void printTree();
};

}

