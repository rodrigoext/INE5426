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
        virtual ~Node() { }
        Node() { }
        Node(Type t) : type(t) { }
        virtual void printTree(){ }
        //Node* coerce(Node* otherNode);
        

};

class Integer : public Node {
    public:
        int value;
        Integer(int value) : value(value) {  }
        void printTree();
};

class BinOp : public Node {
    public:
        Operation op;
        Node *left;
        Node *right;
        BinOp(Node *left, Operation op, Node *right) :
            left(left), right(right), op(op) { 
                switch (op) {
                    case associa:
                        this->type = left->type;
                        // TODO: checar se é uma variável.
                        if (left->type != right->type) {
                         yyerror(("semantico: operacao " + op_name[op] + " espera " + type_name_masc[left->type] +
                                 " mas recebeu " + type_name_masc[right->type] + ".").c_str());
                        }
                        break;
                    case soma:
                    case divide:
                    case subtrai:
                    case multiplica:
                        this->type = inteiro;
                        if (left->type == real && right->type == real)
                            this->type = real;
                        if (left->type != inteiro && left->type != real) {
                         yyerror(("semantico: operacao " + op_name[op] + " espera inteiro ou real mas recebeu " + 
                                 type_name_masc[left->type] + ".").c_str());
                        }
                        if (right->type != inteiro && right->type != real) {
                         yyerror(("semantico: operacao " + op_name[op] + " espera inteiro ou real mas recebeu " + 
                                 type_name_masc[right->type] + ".").c_str());
                        }
                        break;
                    case igual:
                    case diferente:
                    case maior:
                    case maior_igual:
                    case menor:
                    case menor_igual:
                        this->type = booleano;
                        if (left->type != inteiro && left->type != real) {
                         yyerror(("semantico: operacao " + op_name[op] + " espera inteiro ou real mas recebeu " + 
                                 type_name_masc[left->type] + ".").c_str());
                        }
                        if (right->type != inteiro && right->type != real) {
                         yyerror(("semantico: operacao " + op_name[op] + " espera inteiro ou real mas recebeu " + 
                                 type_name_masc[right->type] + ".").c_str());
                        }
                    case e_logico:
                    case ou_logico:
                        this->type = booleano;
                        if (right->type != booleano) {
                         yyerror(("semantico: operacao " + op_name[op] + " espera booleano mas recebeu " + 
                                 type_name_masc[right->type] + ".").c_str());
                        }
                        if (left->type != booleano) {
                         yyerror(("semantico: operacao " + op_name[op] + " espera booleano mas recebeu " + 
                                 type_name_masc[left->type] + ".").c_str());
                        }
                        break;
                    default:
                        this->type = indefinido;
                        break;
                }
            }
        void printTree();
};

class UnOp : public Node {
    public:
        Operation op;
        Node *next;
        UnOp(Node *next, Operation op) :
            next(next), op(op) { 
                switch (op) {
                    case menosunario:
                        this->type = inteiro;
                        if (next->type == inteiro || next->type == real)
                            this->type = next->type;
                        else
                            yyerror(("semantico: operacao " + op_name[op] + " espera inteiro ou real mas recebeu " + 
                                 type_name_masc[next->type] + ".").c_str());
                        break;
                    case negacao:
                        this->type = booleano;
                        if (next->type != booleano) {
                         yyerror(("semantico: operacao " + op_name[op] + " espera booleano mas recebeu " + 
                                 type_name_masc[next->type] + ".").c_str());
                        }
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

class Block : public Node {
    public:
        NodeList lines;
        Block() { }
        void printTree();
};

class VarDeclaration : public Node {
     public:
        NodeList vars;
        VarDeclaration(Type t) {
            this->type = t;
        }
        void setType(Type t) {
            this->type = t;
        }
        void printTree();
};

class ArrayDeclaration : public Node {
    public:
        NodeList arrays;
        std::string tamanho;
        ArrayDeclaration(Type t) {
            this->type = t;
        }
        void printTree();
};

class Variable : public Node {
     public:
         std::string id;
         Variable(std::string id, Type t) :
            id(id) {
                this->type = t;
                //std::cout << "tipo nodo setado" << std::endl;
            }
         void printTree();
};

class Number : public Node {
	 public:
		std::string value;
		Number(std::string value, Type t) :
			value(value) {
                this->type = t;
		    }
		void printTree();
};

/*class Coercion : public Node {
    public:
        Node *next;
        Coercion(Node *next):
            next(next){
                this->type = real;
            }
        void printTree();
};  */

}