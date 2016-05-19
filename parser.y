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
%type <node> expr line declaracao atribuicao varlist arrlist
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
        ;

declaracao : 
        tipo T_DECLARA varlist { $$ = $3; }
        | tipo T_ARRAY_INIT expr T_ARRAY_END T_DECLARA arrlist { $$ = $6; } 
        ;

tipo	: D_INT { symtab.tempType = Type::integer; }
		| D_REAL { symtab.tempType = Type::real; }
		| D_BOOL { symtab.tempType = Type::booleano; }
		;

atribuicao:
        T_ID T_ASSIGN expr {  AST::Node* node = symtab.assignVariable($1);
                                $$ = new AST::AssignOp(node,$3); }
        ;

expr    : T_INT { $$ = new AST::Number($1, Type::integer);  symtab.tempLegthArray = dynamic_cast<AST::Number*>($$)->value;}
		| T_REAL { $$ = new AST::Number($1, Type::real); }
		| T_BOOL { $$ = new AST::Number($1, Type::booleano); }
        | T_ID { $$ = symtab.useVariable($1); }
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
        | T_NEGA expr { $$ = new AST::UnOp($2, negacao); }
        | T_SUB expr { $$ = new AST::UnOp($2, subtrai); }
        | T_ABRE_P expr T_FECHA_P { $$ = new AST::UnOp($2, parenteses); }
        | expr error { yyerrok; $$ = $1; } /*just a point for error recovery*/
        ;

varlist : T_ID { $$ = new AST::VarDeclaration(symtab.tempType);
                 dynamic_cast< AST::VarDeclaration*>($$)->vars.push_back(symtab.newVariable($1, symtab.tempType));
               }
        | varlist T_COMMA T_ID { $$ = $1;
                                 dynamic_cast< AST::VarDeclaration*>($$)->vars.push_back(symtab.newVariable($3, symtab.tempType));
                                }
        ;
arrlist :  T_ID { $$ = new AST::ArrayDeclaration(symtab.tempType);
                 dynamic_cast< AST::ArrayDeclaration*>($$)->arrays.push_back(symtab.newVariable($1, symtab.tempType));
                 dynamic_cast< AST::ArrayDeclaration*>($$)->tamanho = symtab.tempLegthArray;
               }
        | arrlist T_COMMA T_ID { $$ = $1;
                                 dynamic_cast< AST::ArrayDeclaration*>($$)->arrays.push_back(symtab.newVariable($3, symtab.tempType));
                                }
%%


