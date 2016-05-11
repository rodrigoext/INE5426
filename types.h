#pragma once

#include <string>
#include <vector>

enum Type { integer, real, booleano, indefinido};
const std::vector<std::string> type_name_masc = { "inteiro", "real", "booleano", "indefinido"};
const std::vector<std::string> type_name_fem = { "inteira", "real", "booleana", "indefinida"};
enum Kind { variable, function};
enum Operation { 
	soma, subtrai, divide, multiplica, assign,
	igual, diferente, menor, maior };
