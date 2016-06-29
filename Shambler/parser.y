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
%token T_PLUS T_SUB T_NL T_COMMA T_ARRAY_INIT T_ARRAY_END T_FUNC_INI T_FUNC_END
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
%type <node> expr line declaracao atribuicao varlist term funcao parametro parametros
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
        | declaracao T_FIM
        | declaracao
        | atribuicao T_FIM
        | atribuicao
        | funcao
        | error T_FIM {yyerrok; $$ = NULL;}
        ;

declaracao :
        tipo varlist { $$ = $2; }
        ;

varlist : T_ID { if (symtab.checkId($1)) {
                    $$ = symtab.assignVariable($1);
                    symtab.declared = true;
                  } else {
                    $$ = new AST::VarDeclaration(symtab.tempType, symtab.strong);
                    if (symtab.strong)
                      ((AST::VarDeclaration*)($$))->vars.push_back(symtab.newVariable($1, symtab.tempType, symtab.strong));
                    else
                      ((AST::VarDeclaration*)($$))->vars.push_back(symtab.newVariable($1, indefinido, symtab.strong));
                   symtab.declared = false;
                  }
               }
        | varlist T_COMMA T_ID { $$ = $1;
                                 ((AST::VarDeclaration*)($$))->vars.push_back(symtab.newVariable($3, symtab.tempType, symtab.strong));
                                }
        ;

tipo	: D_INT { symtab.tempType = Type::inteiro; symtab.strong = true; }
		| D_REAL { symtab.tempType = Type::real; symtab.strong = true; }
		| D_BOOL { symtab.tempType = Type::booleano; symtab.strong = true; }
		;

atribuicao:
        tipo varlist T_IGUAL expr {
                                      AST::VarDeclaration* vardecl = ((AST::VarDeclaration*)($2));
                     									vardecl->setType(symtab.tempType);
                     									for (auto var = vardecl->vars.begin(); var != vardecl->vars.end(); var++) {
                      									symtab.setSymbolType(((AST::Variable *)(*var))->id, symtab.tempType);
                      									symtab.setSymbolInitialized(((AST::Variable *)(*var))->id);
                      									((AST::Variable *)(*var))->setType(symtab.tempType);
                     									}
                     									$$ = new AST::BinOp($2, associa, $4);
                                      symtab.strong = false;
       								              }
       	| varlist T_IGUAL expr {
                                  if (symtab.declared) {
                                    AST::Variable* v = dynamic_cast< AST::Variable*>($1);
                                    $$ = new AST::BinOp($1, associa, $3);
                                  } else {
                                    AST::VarDeclaration* vardecl = dynamic_cast<AST::VarDeclaration*>($1);
                  									vardecl->setType(symtab.tempType);
                  									for (auto var = vardecl->vars.begin(); var != vardecl->vars.end(); var++) {
                  										symtab.setSymbolType(dynamic_cast<AST::Variable *>(*var)->id, symtab.tempType);
                  										symtab.setSymbolInitialized(dynamic_cast<AST::Variable *>(*var)->id);
                  										dynamic_cast<AST::Variable*>(*var)->setType(symtab.tempType);
                  									}
                  									$$ = new AST::BinOp($1, associa, $3);
                                  }
       							            }
        ;

expr    : term { $$ = $1; }

        | T_ID T_ARRAY_INIT expr T_ARRAY_END
        {
          AST::Variable* v = ((AST::Variable*)(symtab.useVariable($1)));
        	v->setNext($3);
          $$ = v;
        }

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

term   :  T_INT { $$ = new AST::Number($1, Type::inteiro); symtab.tempType = Type::inteiro; }
		    | T_REAL { $$ = new AST::Number($1, Type::real); symtab.tempType = Type::real; }
		    | T_BOOL { $$ = new AST::Number($1, Type::booleano); symtab.tempType = Type::booleano; }
        | T_ID { $$ = symtab.useVariable($1); }
        ;

funcao: T_ID parametros T_FUNC_INI lines T_FUNC_END
        {
          symtab.newFunction($1, Type::indefinido, function);
          $$ = new AST::FunctionDeclaration($1, $2, $4, NULL, indefinido);
        }

        | T_ID parametros T_FUNC_INI lines D_RETURN expr lines T_FUNC_END
        {
          symtab.newFunction($1, ((AST::Variable*)$6)->type, function);
          $$ = new AST::FunctionDeclaration($1, $2, $4, $6, ((AST::Variable*)$6)->type);
        }
        ;

parametros: T_ABRE_P parametro T_FECHA_P
        {
          $$ = NULL;
        }
        ;

parametro: T_ID
        {
          $$ = NULL;
        }

        | parametro T_COMMA T_ID
        {
          $$ = NULL;
        }
        ;



%%
