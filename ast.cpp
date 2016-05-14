#include "ast.h"
#include "symbol_table.h"

using namespace AST;

extern STab::SymbolTable symtab;

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

/*void AssignOp::printTree(){
	 std::cout << "Atribuicao de valor para ";
	left->printTree();
	std::cout << ": ";
	right->printTree();
}*/
    
/*Node* Node::coerce(Node* otherNode){
    if (coercionNeeded(this->type, otherNode->type))
        return new AST::Coercion(this);
    else 
        return this;
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
            std::cout << "(";
            left->printTree();
            std::cout << " (" << operation_name[op] << " ";
            if ( operation_masc[op] ) std::cout << type_name_masc[this->type];
            else std::cout << type_name_fem[this->type];
            std::cout << ") ";
            right->printTree();
            std::cout << ")";
            break;
    }
    //right->printTree();
    return;
}

void UnOp::printTree(){
    std::cout << "(";
    switch(op){
        case par:
            std::cout << "(abre parenteses) ";
            next->printTree();
            std::cout << " (fecha parenteses)";
            break;
        default:
            std::cout << " (" << operation_name[op] << " ";
            if ( operation_masc[op] ) std::cout << type_name_masc[this->type];
            else std::cout << type_name_fem[this->type];
            std::cout << ") ";
            next->printTree();
            break;
    }
    std::cout << ")";
}

void Block::printTree(){
    for (Node* line: lines) {
        line->printTree();
        std::cout  << std::endl;
    }
}

void VarDeclaration::printTree(){
	std::cout << "Declaracao de variavel " << type_name_fem[type] << ": ";
	for (auto var = vars.begin(); var != vars.end(); var++) {
	        std::cout << dynamic_cast<Variable *>(*var)->id;
	        if(next(var) != vars.end()) std::cout << ", ";
	    }
}

void Variable::printTree(){
    std::cout << "variavel " << type_name_fem[type] << " "<< id;
}

void Number::printTree(){
	std::cout << "valor " << type_name_masc[type] << " " << value;
}

/*void Coercion::printTree(){
    next->printTree();
    std::cout << " para real";
}*/

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



/*
int Variable::computeTree(){
    //the value of a variable is currently stored in the symbol table
    //return symtab.entryList[id].value;
    return 1;
}*/
