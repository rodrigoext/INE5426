#pragma once

#include <iostream>
#include <vector>

#include "types.h"

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
        //virtual int computeTree(){return 0;}
        

};

class Integer : public Node {
    public:
        int value;
        Integer(int value) : value(value) {  }
        void printTree();
        //int computeTree();
};

/*class Real : public Node {
     public:
        float value;
        Real(float value) : value(value) {  }
};*/

class BinOp : public Node {
    public:
        Operation op;
        Node *left;
        Node *right;
        BinOp(Node *left, Operation op, Node *right) :
            left(left), right(right), op(op) { 
                this->type = binType(left->type, right->type, op);
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
                this->type = unType(next->type, op);
            }
        void printTree();
};

/*class AssignOp : public Node {
    public:
        Node *left;
        Node *right;
        AssignOp(Node *left, Node *right):
            left(left), right(right) { }
        void printTree();
        //int computeTree();
};*/


class Block : public Node {
    public:
        NodeList lines;
        Block() { }
        void printTree();
        //int computeTree();
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

class Variable : public Node {
     public:
         std::string id;
         Variable(std::string id, Type t) :
            id(id) {
                this->type = t;
                //std::cout << "tipo nodo setado" << std::endl;
            }
         void printTree();
         //int computeTree();
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