%%
.    { printf("Got char: %c\n", yytext[0]); }
%%
int yywrap(void) { return 1; }
int main() { yylex(); return 0; }
