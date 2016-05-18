#include "types.h"

extern void yyerror (const char *c, ...);

Type unType(Type next, Operation op) {
	Type tipo;
	switch(op){
		case nologic:
			tipo = booleano;
			break;
		case menosunario:
			tipo = integer;
			if (next == integer || next == real)
				tipo = next;
			break;
		case par:
			tipo = next;
			break;
		default:
			tipo = indefinido;
	}
	return tipo;
}

Type binType(Type left, Type right, Operation op){
	Type tipo;
	switch (op){
		case soma:
		case subtrai:
		case divide:
		case multiplica:
			tipo = integer;
			if (left == real && right == real)
				tipo = real;
			break;
		case assign:
			tipo = left;
			break;
		case igual:
		case diferente:
		case maior:
		case menor:
		case maiorouigual:
		case menorouigual:
		case andlogic:
		case orlogic:
			tipo = booleano;
			break;
		default:
			tipo = indefinido;
	}
	return tipo;
}