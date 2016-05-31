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
%token D_DEF D_FUN D_DECL
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
%type <node> expr line declaracao atribuicao varlist arrlist term target condicao laco parametro
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

program : lines { programRoot = $1; }
        ;

lines   : line { $$ = new AST::Block(); if ($1 != NULL) $$->lines.push_back($1); }
        | lines line { if($2 != NULL) $1->lines.push_back($2); }
        | lines error T_NL { yyerrok; $$ = NULL;}
        ;

line    : T_NL { $$ = NULL; } /*nothing here to be used */
        | declaracao T_FIM 
        | atribuicao T_FIM
        | condicao D_END D_IF
        | laco D_END D_WHILE
        ;
        
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
													;
laco : D_WHILE expr D_DO T_NL lines { $$ = new AST::LoopExp($2, $5);}
		| D_WHILE expr D_DO lines { $$ = new AST::LoopExp($2, $4);}
		;

declaracao : 
        tipo T_DECLARA varlist { $$ = $3; }
        | tipo T_ARRAY_INIT T_INT T_ARRAY_END T_DECLARA arrlist { AST::VarDeclaration* vardecl = dynamic_cast< AST::VarDeclaration*>($6);
        														  AST::Number *n = new AST::Number($3, Type::inteiro);
        														  vardecl->setTamanho(n);
        														  for (auto var = vardecl->vars.begin(); var != vardecl->vars.end(); var++)
        															 symtab.setSimbolSize(dynamic_cast<AST::Variable *>(*var)->id, std::stoi(n->value));
        													      $$ = $6; }
        | D_DECL D_FUN tipofunc T_DECLARA T_ID T_ABRE_P parametro T_FECHA_P { $$ = NULL; }
        ;
        
parametro : { $$ = NULL;}
		| tipo T_DECLARA T_ID { $$ = new AST::VarDeclaration(symtab.tempType);
                 dynamic_cast< AST::VarDeclaration*>($$)->vars.push_back(symtab.newVariable($3, symtab.tempType));
               }
        | parametro T_COMMA tipo T_DECLARA T_ID { $$ = new AST::VarDeclaration(symtab.tempType);
								                 dynamic_cast< AST::VarDeclaration*>($$)->vars.push_back(symtab.newVariable($5, symtab.tempType));
								               	}
		;

tipofunc: D_INT { symtab.tempTypeFunc = Type::inteiro; }
		| D_REAL { symtab.tempTypeFunc = Type::real; }
		| D_BOOL { symtab.tempTypeFunc = Type::booleano; }
		;

tipo	: D_INT { symtab.tempType = Type::inteiro; }
		| D_REAL { symtab.tempType = Type::real; }
		| D_BOOL { symtab.tempType = Type::booleano; }
		;

atribuicao:
        target T_ASSIGN expr { $$ = new AST::BinOp($1, associa, $3); }
        | target T_ARRAY_INIT expr T_ARRAY_END T_ASSIGN expr { $$ = new AST::BinOp($1, associa, $6, $3); }
        ;
target  : T_ID { $$ = symtab.assignVariable($1); }
		;
		
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
        
term   :  T_INT { $$ = new AST::Number($1, Type::inteiro);}
		| T_REAL { $$ = new AST::Number($1, Type::real); }
		| T_BOOL { $$ = new AST::Number($1, Type::booleano); }
        | T_ID { $$ = symtab.useVariable($1); }
        ;

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
%%


