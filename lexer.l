%{
#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>
int yylineno = 1;
int yycolumn = 1;
#define YY_USER_ACTION yycolumn = yycolumn + yyleng;
%}

DIGIT   [0-9]
ID      [a-z]
WS      [ \t]+
NEWLINE \n
%%

{WS}            {/* skip whitespace, update column */}
{NEWLINE}        { yylineno++; yycolumn = 1; }
{ID}             { return TOKEN_VARIABLE; }
{DIGIT}+\.{DIGIT}+ { yylval.fval = atof(yytext); return TOKEN_FLOAT; }
{DIGIT}+         { yylval.ival = atoi(yytext); return TOKEN_INT; }
"+"              { return TOKEN_PLUS; }
"-"              { return TOKEN_MINUS; }
"*"              { return TOKEN_MUL; }
"/"              { return TOKEN_DIV; }
"^"              { return TOKEN_POW; }
"("              { return TOKEN_LPAREN; }
")"              { return TOKEN_RPAREN; }
.                { return TOKEN_UNKNOWN; }

%%
int yywrap(void) { return 1; }
