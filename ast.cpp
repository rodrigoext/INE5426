#include "ast.h"
#include "symbol_table.h"

using namespace AST;

extern STab::SymbolTable symtab;

/* Print methods */
void Integer::printTree(){
    std::cout << value;
    return;
}
    
/*Node* Node::coerce(Node* otherNode){
    if (coercionNeeded(this->type, otherNode->type))
        return new AST::Coercion(this);
    else 
        return this;
}*/

void BinOp::printTree(){
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
            if ( op_masc[op] ) std::cout << type_name_masc[this->type];
            else std::cout << type_name_fem[this->type];
            std::cout << ") ";
            right->printTree();
            std::cout << ")";
            break;
    }
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
            std::cout << " (" << op_name[op] << " ";
            if ( op_masc[op] ) std::cout << type_name_masc[this->type];
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

void ArrayDeclaration::printTree(){
    std::cout << "Declaracao de arranjo " << type_name_masc[type] << " de tamanho " << tamanho << ": ";
    for (auto var = arrays.begin(); var != arrays.end(); var++) {
        std::cout << dynamic_cast<Variable *>(*var)->id;
        if(next(var) != arrays.end()) std::cout << ", ";
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
