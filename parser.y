%{
#include "ast.h"
#include "symbol_table.h"
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
%token <integer> T_INT
%token <real> T_REAL
%token T_PLUS T_SUB T_NL T_COMMA
%token T_ASSIGN
%token T_MUL T_DIV
%token T_IGUAL T_DIFERENTE T_MAIOR T_MENOR
%token T_MAIOR_IGUAL T_MENOR_IGUAL
%token T_AND T_OR T_NEGA T_ABRE_P T_FECHA_P T_FIM
%token T_TRUE T_FALSE
%token D_INT D_REAL D_BOOL
%token <name> T_ID

/* type defines the type of our nonterminal symbols.
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <node> expr line varlist
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
        | lines error T_NL { yyerrok; }
        ;

line    : T_NL { $$ = NULL; } /*nothing here to be used */
        | expr T_FIM T_NL /*$$ = $1 when nothing is said*/
        | D_INT varlist T_FIM T_NL { $$ = $2; }
        | D_REAL varlist T_FIM T_NL { $$ = $2; }
        | D_BOOL varlist T_FIM T_NL { $$ = $2; }
        | T_ID T_ASSIGN expr T_FIM {  AST::Node* node = symtab.assignVariable($1);
                                $$ = new AST::BinOp(node,AST::assign,$3); }
        ;

expr    : T_INT { $$ = new AST::Integer($1); }
		| T_REAL { $$ = new AST::Real($1); }
        | T_ID { $$ = symtab.useVariable($1); }
        | expr T_PLUS expr { $$ = new AST::BinOp($1,AST::plus,$3); }
        | expr T_SUB expr { $$ = new AST::BinOp($1,AST::sub,$3); }
        | expr T_MUL expr { $$ = new AST::BinOp($1,AST::mul,$3); }
        | expr T_DIV expr { $$ = new AST::BinOp($1,AST::divi,$3); }
        | expr error { yyerrok; $$ = $1; } /*just a point for error recovery*/
        ;

varlist : T_ID { $$ = symtab.newVariable($1, NULL); }
        | varlist T_COMMA T_ID { $$ = symtab.newVariable($3, $1); }
        ;

%%


