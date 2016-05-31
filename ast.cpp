#include "ast.h"
#include "symbol_table.h"

using namespace AST;

extern STab::SymbolTable symtab;

/* Print methods */
void Integer::printTree(){
    std::cout << value;
    return;
}

void AssignOp::printTree(){
	 std::cout << "Atribuicao de valor para ";
	left->printTree();
	std::cout << ": ";
	right->printTree();
}

void UnOp::printTree() {
	switch(op){
	case parenteses:
		std::cout << "(abre parenteses) ";
		next->printTree();
		std::cout << " (fecha parenteses)";
		break;
	default:
		std::cout << "(" << op_name[op] << " ";
		if ( op ) std::cout << type_name_masc[this->type];
		else std::cout << type_name_fem[this->type];
		std::cout << ") ";
		next->printTree();
	}
}

void BinOp::printTree(){
    switch(op){

    case associa:
        std::cout << "Atribuicao de valor para ";
        left->printTree();
        if (array_exp != NULL) {
        	array_exp->printTree();
        	std::cout << "}";
        }
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
    return;
}

void Block::printTree(){
    for (Node* line: lines) {
        line->printTree();
        std::cout  << std::endl;
    }
}

void VarDeclaration::printTree(){
	if (parameter) {
		if (kind == array)
			std::cout << "parametro arranjo " << type_name_masc[type] << " de tamanho " << tamanho->value << ": ";
		else
			std::cout << "parametro " << type_name_masc[type] << ": ";
		for (auto var = vars.begin(); var != vars.end(); var++) {
			std::cout << dynamic_cast<Variable *>(*var)->id;
			if(next(var) != vars.end()) std::cout << ", ";
		}
		std::cout << std::endl;
	} else {
		if (kind == array)
			std::cout << "Declaracao de arranjo " << type_name_masc[type] << " de tamanho " << tamanho->value << ": ";
		else
			std::cout << "Declaracao de variavel " << type_name_fem[type] << ": ";
		for (auto var = vars.begin(); var != vars.end(); var++) {
			std::cout << dynamic_cast<Variable *>(*var)->id;
			if(next(var) != vars.end()) std::cout << ", ";
		}
	}
}


void Number::printTree(){
	std::cout << " valor " << type_name_masc[type] << " " << value;
}

void Variable::printTree(){
	if (kind == array) {
		std::cout << "arranjo " << type_name_masc[type] << " " << id << " {+indice:";
		if(next != NULL)
			next->printTree();
		//std::cout << "} ";
	}
	else
		std::cout << "variável " << type_name_fem[type] << " " << id;

}

void ConditionalExp::printTree() {
	std::cout << "Expressao condicional" << std::endl;
	std::cout << "+se: ";
	condition->printTree();
	std::cout << std::endl;
	std::cout << "+entao: " << std::endl;
	next->printTree();
	if (senao != NULL) {
		std::cout << "+senao" << std::endl;
		senao->printTree();
	}
	std::cout << "Fim expressao condicional";
	std::cout << std::endl;
}

void LoopExp::printTree() {
	std::cout << "Laco" << std::endl;
	std::cout << "+enquanto: ";
	condition->printTree();
	std::cout << std::endl;
	std::cout << "+faca: " << std::endl;
	next->printTree();
	std::cout << "Fim laco";
}

void ParameterDeclaration::printTree() {
	for (auto param = params.begin(); param != params.end(); param++) {
		dynamic_cast<VarDeclaration *>(*param)->printTree();
	}
}

void FunctionDeclaration::printTree() {
	if (next != NULL)
		std::cout << "Declaracao de funcao " << type_name_fem[type] << ": " << id << std::endl;
	else
		std::cout << "Definicao de funcao " << type_name_fem[type] << ": " << id << std::endl;
	std::cout << "+parametros:" << std::endl;
	if (parametros != NULL)
		parametros->printTree();
	if (next != NULL)
		next->printTree();
	std::cout << std::endl << "Fim declaracao" ;
}
