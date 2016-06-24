%{
#include "ast.h"
#include "symbol_table.h"
#include "types.h"
STab::SymbolTable symtab;
AST::Block *programRoot; /* the root node of our program AST:: */
extern int yylex();
extern void yyerror(const char* s, ...);
%}

/* yylval == %union
 * union informs the different ways we can store data
 */
%union {
    int integer;
    float real;
    AST::Node *node;
    AST::Block *block;
    char *name;

}
/* token defines our terminal symbols (tokens).
 */
 /*
%token <integer> T_INT
%token <real> T_REAL
*/
%token T_PLUS T_SUB T_NL T_COMMA T_ARRAY_INIT T_ARRAY_END
%token T_ASSIGN T_DECLARA
%token D_IF D_THEN D_END D_ELSE
%token D_WHILE D_DO
%token D_DEF D_FUN D_DECL D_RETURN
%token T_MUL T_DIV
%token T_IGUAL T_DIFERENTE T_MAIOR T_MENOR
%token T_MAIOR_IGUAL T_MENOR_IGUAL
%token T_AND T_OR T_NEGA T_ABRE_P T_FECHA_P T_FIM
%token T_TRUE T_FALSE
%token D_INT D_REAL D_BOOL
%token <name> T_ID T_BOOL T_INT T_REAL

/* type defines the type of our nonterminal symbols.
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <node> expr line declaracao atribuicao varlist arrlist term target condicao laco parametro definicao parametros
%type <block> lines program

/* Operator precedence for mathematical operators
 * The latest it is listed, the highest the precedence
 */
%left T_PLUS T_SUB
%left T_MUL T_DIV
%nonassoc error

/* Starting rule 
 */
%start program

%%

/*Programa composto de linhas*/
program : lines { programRoot = $1; }
        ;

lines   : line { $$ = new AST::Block(); if ($1 != NULL) $$->lines.push_back($1); }
        | lines line { if($2 != NULL) $1->lines.push_back($2); }
        ;
/*Cada linha pode ser uma declaração, atribuição, condição, laco ou definicao de algo*/
line    : T_NL { $$ = NULL; } /*nothing here to be used */
        | condicao D_END D_IF
        | laco D_END D_WHILE
        | declaracao T_FIM 
        | atribuicao T_FIM
        | atribuicao
        | definicao D_END D_DEF
        | error T_FIM {yyerrok; $$ = NULL;}
        ;

/*Uma condição é composto de uma expressão e linhas, sendo que a expressão pode ser falsa
	e assim o programa irá executos outras linhas*/
condicao : D_IF expr T_NL D_THEN lines { $$ = new AST::ConditionalExp($2, $5); }
		|  D_IF expr D_THEN lines { $$ = new AST::ConditionalExp($2, $4); }
		|  D_IF expr T_NL D_THEN lines D_ELSE lines { AST::ConditionalExp * condExp;
													condExp = new AST::ConditionalExp($2, $5);
													condExp->SetSenao($7);
													$$ = condExp; }
		|  D_IF expr D_THEN lines D_ELSE lines { AST::ConditionalExp * condExp;
													condExp = new AST::ConditionalExp($2, $4);
													condExp->SetSenao($6);
													$$ = condExp; }
/*Um laço é composto de uma expressão e enquanto ela for verdadeira, executa as linhas dentro do escopo do mesmo*/													;
laco : D_WHILE expr D_DO T_NL lines { $$ = new AST::LoopExp($2, $5);}
		| D_WHILE expr D_DO lines { $$ = new AST::LoopExp($2, $4);}
		;

/*Uma declaração é composta de um tipo primitivo seguito por identificadores*/
/*Obs: No caso de funções, podemos ou não ter parametros*/
declaracao : 
        tipo T_DECLARA varlist { $$ = $3; }
        | tipo T_ARRAY_INIT T_INT T_ARRAY_END T_DECLARA arrlist { AST::VarDeclaration* vardecl = dynamic_cast< AST::VarDeclaration*>($6);
        														  AST::Number *n = new AST::Number($3, Type::inteiro);
        														  vardecl->setTamanho(n);
        														  for (auto var = vardecl->vars.begin(); var != vardecl->vars.end(); var++)
        															 symtab.setSymbolSize(dynamic_cast<AST::Variable *>(*var)->id, std::stoi(n->value));
        													      $$ = $6; }
        | D_DECL D_FUN tipofunc T_DECLARA T_ID parametros { symtab.newFunction($5, symtab.tempTypeFunc, function, NULL, false); 
        													$$ = new AST::FunctionDeclaration($5, $6, NULL, NULL, symtab.tempTypeFunc); }
        ;

/*Lista de identificadores usados para declaração de variáveis*/        
varlist : T_ID { $$ = new AST::VarDeclaration(symtab.tempType);
                 dynamic_cast< AST::VarDeclaration*>($$)->vars.push_back(symtab.newVariable($1, symtab.tempType));
               }
        | varlist T_COMMA T_ID { $$ = $1;
                                 dynamic_cast< AST::VarDeclaration*>($$)->vars.push_back(symtab.newVariable($3, symtab.tempType));
                                }
        ;
arrlist :  T_ID { $$ = new AST::VarDeclaration(symtab.tempType, array);
                 dynamic_cast< AST::VarDeclaration*>($$)->vars.push_back(symtab.newVariable($1, symtab.tempType, array));
               }
        | arrlist T_COMMA T_ID { $$ = $1;
                                 dynamic_cast< AST::VarDeclaration*>($$)->vars.push_back(symtab.newVariable($3, symtab.tempType, array));
                                }
        ;
                               
/*Análago a declaração temos a definição de uma função, porém aqui, 
/*também é definida o escopo dessa função.*/        
definicao: D_DEF D_FUN tipofunc T_DECLARA T_ID T_ABRE_P parametro T_FECHA_P
					lines D_RETURN expr T_FIM line {symtab.newFunction($5, symtab.tempTypeFunc, function, NULL, true); 
													$$ = new AST::FunctionDeclaration($5, $7, $9, $11, symtab.tempTypeFunc); } 

/*Parametros são definidos por uma ou mais parametro entre parenteses*/
parametros:  { $$ = NULL; }
		| T_ABRE_P parametro T_FECHA_P { $$ = $2; }
		;	
/*Um parametros é composto pela declaração de uma variável*/
parametro : { $$ = NULL;}
		| tipo T_DECLARA T_ID { $$ = new AST::ParameterDeclaration();
				 AST::VarDeclaration * vd = new AST::VarDeclaration(symtab.tempType, variable, true);
                 dynamic_cast< AST::VarDeclaration*>(vd)->vars.push_back(symtab.newVariable($3, symtab.tempType, variable, true));
                 dynamic_cast< AST::ParameterDeclaration*>($$)->params.push_back(vd);
               }
        | parametro T_COMMA tipo T_DECLARA T_ID { $$ = $1; 
        										  AST::VarDeclaration * vd = new AST::VarDeclaration(symtab.tempType, variable, true);
                 								  vd->vars.push_back(symtab.newVariable($5, symtab.tempType, variable, true));
                								  dynamic_cast< AST::ParameterDeclaration*>($$)->params.push_back(vd);
								               	}
		/*Parametros do tipo arranjo*/ 	
       
       | tipo T_ARRAY_INIT T_INT T_ARRAY_END T_DECLARA T_ID { $$ = new AST::ParameterDeclaration();
															 AST::VarDeclaration * vd = new AST::VarDeclaration(symtab.tempType, array, true);
															 AST::Number *n = new AST::Number($3, Type::inteiro);
													  		 vd->setTamanho(n);
														     vd->vars.push_back(symtab.newVariable($6, symtab.tempType, array, true));
														     symtab.setSymbolSize($6, std::stoi(n->value));
														     dynamic_cast< AST::ParameterDeclaration*>($$)->params.push_back(vd);
															}       
		| parametro T_COMMA tipo T_ARRAY_INIT T_INT T_ARRAY_END T_DECLARA T_ID
												{
												  AST::VarDeclaration* vardecl = new AST::VarDeclaration(symtab.tempType, array, true);
												  AST::Number *n = new AST::Number($5, Type::inteiro);
												  vardecl->setTamanho(n);
												  vardecl->vars.push_back(symtab.newVariable($8, symtab.tempType, array, true));
												  symtab.setSymbolSize($8, std::stoi(n->value));
											      $$ = $1;
											      dynamic_cast< AST::ParameterDeclaration*>($$)->params.push_back(vardecl);
												}
		;

/*Definição do tipo da função que esta sendo definida ou declarada*/
tipofunc: D_INT { symtab.tempTypeFunc = Type::inteiro; }
		| D_REAL { symtab.tempTypeFunc = Type::real; }
		| D_BOOL { symtab.tempTypeFunc = Type::booleano; }
		;
/*Definição do tipo da variável que esta sendo declarada*/
tipo	: D_INT { symtab.tempType = Type::inteiro; }
		| D_REAL { symtab.tempType = Type::real; }
		| D_BOOL { symtab.tempType = Type::booleano; }
		| { symtab.tempType = Type::indefinido; }
		;

/*Uma atribuição é composta por uma expressão ou função já declarada, para uma determinada variável*/
/*Obs: No caso de atribuir a chamada de uma função para uma variável, temos a possibilidade de ter parametros*/
atribuicao:
        target T_ASSIGN expr parametros { AST::Variable* v = dynamic_cast< AST::Variable*>($1);
        								  if (v->kind == array)
        									yyerror(("semantico: arranjo " + v->id + " com uso como variavel.").c_str());
        								  $$ = new AST::BinOp($1, associa, $3); }
        | target T_ARRAY_INIT expr T_ARRAY_END T_ASSIGN expr parametros {if ($7 != NULL)
        																	dynamic_cast< AST::FunctionDeclaration*>($3)->SetParametros($7); 
        																$$ = new AST::BinOp($1, associa, $6, $3); }
       	| varlist T_IGUAL expr { AST::VarDeclaration* vardecl = dynamic_cast< AST::VarDeclaration*>($1);
       									vardecl->setType(symtab.tempType);
       									for (auto var = vardecl->vars.begin(); var != vardecl->vars.end(); var++) {
        									symtab.setSymbolType(dynamic_cast<AST::Variable *>(*var)->id, symtab.tempType);
        									symtab.setSymbolInitialized(dynamic_cast<AST::Variable *>(*var)->id);
        									dynamic_cast<AST::Variable*>(*var)->setType(symtab.tempType);
       									}
       									$$ = new AST::BinOp($1, associa, $3); 
       									}
        ;

/*Variável que será usada na atribuição*/
target  : T_ID { $$ = symtab.assignVariable($1); }
		;

/*Uma expressão é composta por operações binárias ou unárias.*/
expr    : term { $$ = $1; }
        | T_ID T_ARRAY_INIT expr T_ARRAY_END { AST::Variable* v = dynamic_cast< AST::Variable*>(symtab.useVariable($1));
        										v->setNext($3);
        										$$ = v; }
        | expr T_PLUS expr { $$ = new AST::BinOp($1, soma, $3); }
        | expr T_SUB expr { $$ = new AST::BinOp($1, subtrai, $3); }
        | expr T_MUL expr { $$ = new AST::BinOp($1, multiplica, $3); }
        | expr T_DIV expr { $$ = new AST::BinOp($1, divide, $3); }
        | expr T_MAIOR expr { $$ = new AST::BinOp($1, maior, $3); }
        | expr T_MENOR expr { $$ = new AST::BinOp($1, menor, $3); }
        | expr T_MAIOR_IGUAL expr { $$ = new AST::BinOp($1, maior_igual, $3); }
        | expr T_MENOR_IGUAL expr { $$ = new AST::BinOp($1, menor_igual, $3); }
        | expr T_DIFERENTE expr { $$ = new AST::BinOp($1, diferente, $3); }
        | expr T_IGUAL expr { $$ = new AST::BinOp($1, igual, $3); }
        | expr T_AND expr { $$ = new AST::BinOp($1, e_logico, $3); }
        | expr T_OR expr { $$ = new AST::BinOp($1, ou_logico, $3); }
        | T_NEGA expr { $$ = new AST::UnOp($2, negacao); }
        | T_SUB expr { $$ = new AST::UnOp($2, subtrai); }
        | T_ABRE_P expr T_FECHA_P { $$ = new AST::UnOp($2, parenteses); }
        | expr error { yyerrok; $$ = $1; } /*just a point for error recovery*/
        ;

/*Uma expressão pode ser um termo que inclui: usar uma variável já declarada ou um valor (real, inteiro e booleano)*/        
term   :  T_INT { $$ = new AST::Number($1, Type::inteiro); symtab.tempType = Type::inteiro; }
		| T_REAL { $$ = new AST::Number($1, Type::real); symtab.tempType = Type::real; }
		| T_BOOL { $$ = new AST::Number($1, Type::booleano); symtab.tempType = Type::booleano; }
        | T_ID { $$ = symtab.useVariable($1); }
        ;



%%


