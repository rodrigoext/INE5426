%{
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "parser.h"
extern int yylineno;
void yyerror(const char* s, ...);
%}

%option noyywrap nodefault yylineno

int [0-9]+
nome [a-z]+
real  [0-9]+\.[0-9]*|[0-9]*\.[0-9]+

%%

[ \t]               ; //ignores spaces
{int}              	{yylval.name = strdup(yytext); return T_INT;}
{real}				{yylval.name = strdup(yytext); return T_REAL;}		
"+"                 {return T_PLUS;}
"-"                 {return T_SUB;}
"/"                 {return T_DIV;}
"*"					{return T_MUL;}
","                 {return T_COMMA;}
":="                {return T_ASSIGN;}
"="                 {return T_IGUAL;}
"~="                {return T_DIFERENTE;}
">"                 {return T_MAIOR;}
"<"                 {return T_MENOR;}
">="                {return T_MAIOR_IGUAL;}
"<="                {return T_MENOR_IGUAL;}
"AND"               {return T_AND;}
"OR"                {return T_OR;}
"~"                 {return T_NEGA;}
"("                 {return T_ABRE_P;}
")"                 {return T_FECHA_P;}
"["					{return T_ARRAY_INIT;}
"]"					{return T_ARRAY_END;}
":"					{return T_DECLARA;}
";"                 {return T_FIM;}
"TRUE"				{yylval.name = strdup(yytext); return T_BOOL;}
"FALSE"				{yylval.name = strdup(yytext); return T_BOOL;}
"int"				{return D_INT;}
"real"				{return D_REAL;}
"bool"				{return D_BOOL;}
{nome}				{yylval.name = strdup(yytext); return T_ID;}
\n                  {return T_NL;}
.                   {yyerror("Unknown token! %c\n", *yytext);} /*complains about every unknown character*/

%%

void yyerror(const char *s, ...){
    va_list ap;
    va_start(ap, s);
    std::fprintf(stderr, "Line %d: error: ", yylineno);
    std::vfprintf(stderr, s, ap);
}
