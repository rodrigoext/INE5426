#include "ast.h"
#include "symbol_table.h"

using namespace AST;

extern STab::SymbolTable symtab;

/* Print methods */
void Integer::printTree(){
    std::cout << value;
    return;
}

void BinOp::printTree(){
    left->printTree();
    switch(op){
        case mul: std::cout << " * ";
        break;
        case plus: std::cout << " + ";
        break;
    }
    right->printTree();
    return;
}

void Block::printTree(){
    for (Node* line: lines) {
        line->printTree();
        std::cout << std::endl;
    }
}

/* Compute methods */
int Integer::computeTree(){
    return value;
}

int BinOp::computeTree(){
    int value, lvalue, rvalue;
    lvalue = left->computeTree();
    rvalue = right->computeTree();
    switch(op){
        case mul: value = lvalue * rvalue;
        std::cout << "Encontrou uma multiplicação entre: " 
        << lvalue << " e " << rvalue << "." << std::endl;
        break; 
        case plus: value = lvalue + rvalue;
        std::cout << "Encontrou uma soma entre: " 
        << lvalue << " e " << rvalue << "." << std::endl;
        break; 
        break;  
    }
    return value;
}

int Block::computeTree(){
    int value;
    for (Node* line: lines) {
        value = line->computeTree();
         std::cout << "Computed " << value << std::endl;
    }
    return 0;
}

void Variable::printTree(){
    if (next != NULL){
        next->printTree();
        std::cout << ", ";
    }
    std::cout << id;
}

int Variable::computeTree(){
    //the value of a variable is currently stored in the symbol table
    return symtab.entryList[id].value;
}
