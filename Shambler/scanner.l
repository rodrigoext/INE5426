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
"["					        {return T_ARRAY_INIT;}
"]"					        {return T_ARRAY_END;}
"{"                 {return T_FUNC_INI;}
"}"                 {return T_FUNC_END;}
":"					        {return T_DECLARA;}
";"                 {return T_FIM;}
"."                 {return T_DOT;}
"->"                {return T_FIND;}
"find"              {return D_FIND;}
"TRUE"				{yylval.name = strdup(yytext); return T_BOOL;}
"FALSE"				{yylval.name = strdup(yytext); return T_BOOL;}
"int"				{return D_INT;}
"real"				{return D_REAL;}
"bool"				{return D_BOOL;}
"vector"            {return D_VETOR;}
"list"              {return D_LISTA;}
"if"				{return D_IF;}
"then"				{return D_THEN;}
"end"				{return D_END;}
"else"				{return D_ELSE;}
"decl"				{return D_DECL;}
"fun"				{return D_FUN;}
"def"				{return D_DEF;}
"while"				{return D_WHILE;}
"do"				{return D_DO;}
"return"			{return D_RETURN;}
{nome}				{yylval.name = strdup(yytext); return T_ID;}
\n                  {return T_NL;}
.                   {yyerror("lexico: simbolo desconhecido: %c.\n", *yytext);} /*complains about every unknown character*/

%%

void yyerror(const char *s, ...){
    va_list ap;
    va_start(ap, s);
    std::fprintf(stderr, "[Linha %d] Erro ", yylineno);
    std::vfprintf(stderr, s, ap);
    std::fprintf(stderr, "\n");
}
