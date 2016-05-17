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

void AssignOp::printTree(){
	 std::cout << "Atribuicao de valor para ";
	left->printTree();
	std::cout << ": ";
	right->printTree();
}

void UnOp::printTree() {
	switch(op){
	default:
		std::cout << "(" << op_name[op] << " ";
		if ( op ) std::cout << type_name_masc[this->type];
		else std::cout << type_name_fem[this->type];
		std::cout << ") ";
		next->printTree();
	}
}

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
        std::cout << " (" << op_name[op] << " ";
        std::cout << type_name_fem[this->type];
        std::cout << ") ";
        right->printTree();
        std::cout << ")";
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
	std::cout << "Declaracao de variavel " << type_name_fem[type] << ": ";
	for (auto var = vars.begin(); var != vars.end(); var++) {
	        std::cout << dynamic_cast<Variable *>(*var)->id;
	        if(next(var) != vars.end()) std::cout << ", ";
	    }
}

void ArrayDeclaration::printTree(){
	std::cout << "Declaracao de arranjo " << type_name_masc[type] << " de tamanho " << tamanho << ": ";
	for (auto var = arrays.begin(); var != arrays.end(); var++) {
	        std::cout << dynamic_cast<Variable *>(*var)->id;
	        if(next(var) != arrays.end()) std::cout << ", ";
	    }
}


void Number::printTree(){
	std::cout << "valor " << type_name_masc[type] << " " << value;
}

void Variable::printTree(){
	std::cout << "variável " << type_name_fem[type] << " " << id;
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



/*
int Variable::computeTree(){
    //the value of a variable is currently stored in the symbol table
    //return symtab.entryList[id].value;
    return 1;
}*/
