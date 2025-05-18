#ifndef TOKENS_H
#define TOKENS_H

typedef enum {
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_POW,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_VARIABLE,
    TOKEN_UNKNOWN,
    TOKEN_EOF
} TokenType;

typedef union {
    int ival;
    double fval;
    char cval;
} TokenValue;

extern TokenValue yylval;
extern int yylineno;
extern int yycolumn;

#endif // TOKENS_H
