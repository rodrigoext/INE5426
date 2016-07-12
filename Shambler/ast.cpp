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
    case igual:
	case diferente:
	case maior:
	case maior_igual:
	case menor:
	case menor_igual:
	case e_logico:
	case ou_logico:
		std::cout << "(";
		left->printTree();
		std::cout << " (" << op_name[op] << " ";
		std::cout << type_name_masc[this->type];
		std::cout << ") ";
		right->printTree();
		std::cout << ")";
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

int BinOp::computeTree() {
  return left->computeTree() + right->computeTree();
}

void Block::printTree(){
    for (Node* line: lines) {
        line->printTree();
        std::cout  << std::endl;
    }
}

int Block::computeTree(){
  int value;
  for (Node* line: lines) {
      value = line->computeTree();
      std::cout << "Computed " << value << std::endl;
  }
  return 0;
}

void Number::printTree(){
	std::cout << " valor " << type_name_masc[type] << " " << value;
}

int Number::computeTree(){
  return std::stoi(value);
}

void Variable::printTree(){
	if (kind == array) {
		if(valPosition) {
			valPosition->printTree();
			std::cout << " localizado no "; 
		}
		std::cout << "arranjo " << type_name_masc[type] << " " << id;
		
		if(next != NULL) {
			std::cout << " {+indice:";
			next->printTree();
		}
		std::cout << "} ";
		
	} else if (kind == function) {
		std::cout << "chamada de funcao " << type_name_fem[type] << " " << id;
		std::cout << " {+parametros: ";
		if (parameters != NULL) {
			parameters->printTree();
		}
		std::cout << "}";
	} else if (kind == matrix) {
		if(valPosition) {
			valPosition->printTree();
			std::cout << " localizado na "; 
		}
		std::cout << "matriz " << type_name_fem[type] << " " << id;
		if (x && y) {
			std::cout << " {+linha:";
			x->printTree();
			std::cout << "}, {+coluna:";
			y->printTree();
			std::cout << "}";
		}
	} else {
		std::cout << "variável " << type_name_fem[type] << " " << id;
	}
}

int Variable::computeTree(){
  return 0;
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
		if (kind == array) {
			std::cout << "Declaracao de arranjo " << type_name_masc[type] << " de tamanho " << tamanho->value << ": ";
		} else if (kind == matrix) {
			std::cout << "Declaracao de Matriz " << type_name_fem[type] << " de ";
			std::cout << x->value << " linhas por " << y->value << " colunas: ";
		} else {
			if(strong) {
					std::cout << "Declaracao de variavel " << type_name_fem[type] << ": ";
			} else {
				std::cout << " variavel com tipagem dinâmica " <<  type_name_fem[type] << " ";
			}

    }
		for (auto var = vars.begin(); var != vars.end(); var++) {
			std::cout << dynamic_cast<Variable *>(*var)->id;
			if(next(var) != vars.end()) std::cout << ", ";
		}
	}
}

int VarDeclaration::computeTree() {
  return 0;
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
	if (forExp) {
		std::cout << "+para: ";
		condition->printTree();
		std::cout << std::endl;
		if(decrement)
			std::cout << "+decrementa ate: ";
		else
			std::cout << "+ate: ";
		conditionFor->printTree();
		std::cout << std::endl;
	} else {
		std::cout << "+enquanto: ";
		condition->printTree();
		std::cout << std::endl;
	}
	std::cout << "+faca: " << std::endl;
	next->printTree();
	std::cout << "Fim laco";
}

double LoopExp::computeTreeD() {
	if (forExp) {
		return 1.0;
	} else {
		return 0.0;
	}
}

void ParameterDeclaration::printTree() {
	for (auto param = params.begin(); param != params.end(); param++) {
		dynamic_cast<VarDeclaration *>(*param)->printTree();
	}
}

void FunctionDeclaration::printTree() {
	if (next != NULL) {
		std::cout << "Declaracao de funcao ";
    if (type == indefinido)
      std::cout << "sem retorno: " << id << std::endl;
    else
      std::cout << type_name_fem[type] << ": " << id << std::endl;
	} else {
		std::cout << "Definicao de funcao " << type_name_fem[type] << ": " << id << std::endl;
  }
	if (parametros != NULL) {
		std::cout << "+parametros:" << std::endl;
		parametros->printTree();
	}
	if (next != NULL) {
		std::cout << "+corpo:" << std::endl;
		next->printTree();
	}
	if (retorno != NULL) {
		std::cout << "Retorno de funcao: ";
		retorno->printTree();
		std::cout << std::endl << "Fim declaracao" ;
	} else {
		std::cout << "Fim definicao" ;
	}
}

void FindExpr::printTree() {
  std::cout << "Busca por predicado onde existe um " << id << ", tal que " << id << " =";
  next->printTree();
  std::cout << std::endl;
}
