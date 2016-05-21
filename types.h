#pragma once


#include <string>
#include <vector>

enum Type { inteiro, real, booleano, indefinido};
const std::vector<std::string> type_name_masc = { "inteiro", "real", "booleano", "indefinido"};
const std::vector<std::string> type_name_fem = { "inteira", "real", "booleana", "indefinida"};
enum Kind { variable, function};
enum Operation { 
	soma, 
	subtrai, 
	divide, 
	multiplica, 
	associa,
	igual, 
	diferente, 
	maior, 
	menor,
    maior_igual,
    menor_igual, 
    parenteses,
    menosunario,
    e_logico,
    ou_logico,
    negacao
};
const std::vector<std::string> op_name = {
    "soma",
    "subtracao",
    "divisao",
    "multiplicacao",
    "atribuicao",
    "igual",
    "diferente",
    "maior",
    "menor",
    "maior ou igual",
    "menor ou igual",
    "parenteses",
    "menos unario",
    "e",
    "ou",
    "nao"
};

const std::vector<bool> op_masc = {
    false,  //soma inteira
    false,  //subtracao inteira
    false,  //divisao inteira
    false,  //multiplicacao inteira
    false,  //atribuicao intera (nunca usado)
    true,   //igual inteiro
    true,   //diferente inteiro
    true,   //maior inteiro
    true,   //menor inteiro
    true,   //maior ou igual inteiro
    true,   //menor ou igual inteiro
    false,  //parenteses intero (nunca usado)
    true,   //menos unario inteiro
    true,   // e booleano
    true,    // ou booleano
    true    // nao booleano
};

//bool coercionNeeded(Type myType, Type otherType);