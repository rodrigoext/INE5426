#pragma once

#include <string>
#include <vector>

enum Type { inteiro, real, booleano, indefinido, dinamico};
const std::vector<std::string> type_name_masc = { "inteiro", "real", "booleano", "indefinido", "dinamico"};
const std::vector<std::string> type_name_fem = { "inteira", "real", "booleana", "indefinida", "dinamica"};
const std::vector<std::string> op_name = {"soma", "subtracao", "divisao", "multiplicacao", "associacao",
											"igual", "diferente", "menor", "maior", "menor ou igual", "maior ou igual", "e", "ou",
											"nao", "parenteses"};
enum Kind { variable, function, array, matrix};
enum Operation {
	soma, subtrai, divide, multiplica, associa,
	igual, diferente, menor, maior, menor_igual, maior_igual, e_logico, ou_logico,
	negacao, parenteses};
