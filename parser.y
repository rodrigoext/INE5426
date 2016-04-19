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
    AST::Node *node;
    AST::Block *block;
    char *name;
}

/* token defines our terminal symbols (tokens).
 */
%token <integer> T_INT
%token T_PLUS T_NL
%token T_DEF
%token T_MUL
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
%left T_PLUS
%left T_MUL
%nonassoc error

/* Starting rule 
 */
%start program

%%

program : lines { programRoot = $1; }
        ;
        

lines   : line { $$ = new AST::Block(); $$->lines.push_back($1); }
        | lines line { if($2 != NULL) $1->lines.push_back($2); }
        | lines error T_NL { yyerrok; }
        ;

line    : T_NL { $$ = NULL; } /*nothing here to be used */
        | expr T_NL /*$$ = $1 when nothing is said*/
        | T_DEF varlist T_NL { $$ = $2; }
        ;

expr    : T_INT { $$ = new AST::Integer($1); }
        | expr T_PLUS expr { $$ = new AST::BinOp($1,AST::plus,$3); }
        | expr T_MUL expr { $$ = new AST::BinOp($1,AST::mul,$3); }
        | expr error { yyerrok; $$ = $1; } /*just a point for error recovery*/
        ;

varlist : T_ID { $$ = symtab.newVariable($1, NULL); }
        | varlist T_ID { $$ = symtab.newVariable($2, $1); }
        ;

%%


