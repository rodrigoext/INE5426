#pragma once

#include <string>
#include <vector>

enum Type { integer, real, booleano, indefinido};
const std::vector<std::string> type_name_masc = { "inteiro", "real", "booleano", "indefinido"};
const std::vector<std::string> type_name_fem = { "inteira", "real", "booleana", "indefinida"};
enum Kind { variable, function};
enum Operation { 
	soma, 
	subtrai, 
	divide, 
	multiplica, 
	assign,
	igual, 
	diferente, 
	menor, 
	maior };
const std::vector<std::string> operation_name = {
    "soma",
    "subtracao",
    "divisao",
    "multiplicacao",
    "atribuicao",
    "igual",
    "diferente",
    "menor",
    "maior"
    };

const std::vector<bool> operation_masc = {
    false,  //soma inteira
    false,  //subtracao inteira
    false,  //divisao inteira
    false,  //multiplicacao inteira
    false,  //atribuicao intera (nunca usado)
    true,   //igual inteiro
    true,   //diferente inteiro
    true,   //menor inteiro
    true,   //maior inteiro
};