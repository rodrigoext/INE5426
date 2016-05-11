#include "ast.h"
#include "symbol_table.h"

using namespace AST;

extern STab::SymbolTable symtab;

std::vector<std::string> type_names = {"inteira", "real", "booleana"};

/* Print methods */
void Integer::printTree(){
    std::cout << value;
    return;
}
/*
void Real::printTree(){
    std::cout << value;
    return;
}*/

void BinOp::printTree(){
    //left->printTree();
    switch(op){
        case assign:
        std::cout << "Atribuicao de valor para ";
        left->printTree();
        std::cout << ": ";
        right->printTree();
        break;
        default:
        break;
    }
    //right->printTree();
    return;
}

void Block::printTree(){
    for (Node* line: lines) {
        line->printTree();
        std::cout  << std::endl;
    }
}

void VarDeclaration::printTree(){
    for(int i = 0; i < 3; ++i) {
        std::cout << "Declaracao de variavel " << type_names[i] << ": ";
        for (int j = 0; j < vars.size(); ++j) {
            if (vars[j]->type == i)
                std::cout << dynamic_cast<Variable *>(vars[i])->id;
        }
        std::cout << std::endl;
    }
}
/* Compute methods */
//int Integer::computeTree(){
//    return value;
//}

/*
float Real::computeTree(){
    return value;
}*/

/*
int BinOp::computeTree(){
    int value, lvalue, rvalue;
    lvalue = left->computeTree();
    rvalue = right->computeTree();
    switch(op){
        case plus: value = lvalue + rvalue;
        std::cout << "Encontrou uma soma entre: " 
        << lvalue << " e " << rvalue << "." << std::endl;
        break;
        case sub: value = lvalue - rvalue;
        std::cout << "Encontrou uma subtração entre: " 
        << lvalue << " e " << rvalue << "." << std::endl;
        break;
        case mul: value = lvalue * rvalue;
        std::cout << "Encontrou uma multiplicação entre: " 
        << lvalue << " e " << rvalue << "." << std::endl;
        break; 
        case divi: value = lvalue / rvalue;
        std::cout << "Encontrou uma divisao entre: " 
        << lvalue << " e " << rvalue << "." << std::endl;
        break;
        case assign:
            Variable* leftvar = dynamic_cast<Variable*>(left);
            symtab.entryList[leftvar->id].value = rvalue;

            value = rvalue;   
        break;  
    }
    return value;
    return 1;
}*/


/*
int Block::computeTree(){
    int value;
    for (Node* line: lines) {
        value = line->computeTree();
         std::cout << "Computed " << value << std::endl;
    }
    return 0;
}
*/

void Variable::printTree(){
    if (next != NULL){
        next->printTree();
        std::cout << ", ";
    }
    std::cout << id;
}

/*
int Variable::computeTree(){
    //the value of a variable is currently stored in the symbol table
    //return symtab.entryList[id].value;
    return 1;
}*/
