#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "tokens.h"

TokenValue yylval;
int yylineno = 1;
int yycolumn = 1;
static FILE* input_file = NULL;
static char current_char = ' ';
static int is_eof = 0;

void set_input_file(FILE* file) {
    input_file = file;
    yylineno = 1;
    yycolumn = 1;
    current_char = ' ';
    is_eof = 0;
}

static void read_char() {
    if (is_eof) return;
    
    int c = fgetc(input_file);
    if (c == EOF) {
        is_eof = 1;
        current_char = '\0'; // Use null character to represent EOF
        return;
    }
    
    current_char = (char)c;
    yycolumn++;
    if (current_char == '\n') {
        yylineno++;
        yycolumn = 1;
    }
}

int yylex() {
    // Skip whitespace
    while (isspace(current_char)) {
        read_char();
    }
    
    // Check for EOF
    if (is_eof || current_char == '\0') {
        printf("DEBUG: Lexer returning TOKEN_EOF\n");
        return TOKEN_EOF;
    }
    
    printf("DEBUG: Lexer processing char: '%c'\n", current_char);
    
    // Check for digits (numbers)
    if (isdigit(current_char)) {
        int has_decimal = 0;
        char num_buffer[256] = {0};
        int i = 0;
        
        // Read all digits (and possibly a decimal point)
        while (isdigit(current_char) || (current_char == '.' && !has_decimal)) {
            if (current_char == '.') {
                has_decimal = 1;
            }
            num_buffer[i++] = current_char;
            read_char();
        }
        num_buffer[i] = '\0';
        
        if (has_decimal) {
            yylval.fval = atof(num_buffer);
            printf("DEBUG: Lexer returning TOKEN_FLOAT: %f\n", yylval.fval);
            return TOKEN_FLOAT;
        } else {
            yylval.ival = atoi(num_buffer);
            printf("DEBUG: Lexer returning TOKEN_INT: %d\n", yylval.ival);
            return TOKEN_INT;
        }
    }
    
    // Check for variables (single lowercase letters)
    if (islower(current_char)) {
        yylval.cval = current_char; // Store the variable name
        char var_name = current_char;
        read_char();
        return TOKEN_VARIABLE;
    }
    
    // Check for operators and parentheses
    switch (current_char) {
        case '+':
            read_char();
            printf("DEBUG: Lexer returning TOKEN_PLUS\n");
            return TOKEN_PLUS;
        case '-':
            read_char();
            printf("DEBUG: Lexer returning TOKEN_MINUS\n");
            return TOKEN_MINUS;
        case '*':
            read_char();
            printf("DEBUG: Lexer returning TOKEN_MUL\n");
            return TOKEN_MUL;
        case '/':
            read_char();
            printf("DEBUG: Lexer returning TOKEN_DIV\n");
            return TOKEN_DIV;
        case '^':
            read_char();
            printf("DEBUG: Lexer returning TOKEN_POW\n");
            return TOKEN_POW;
        case '(':
            read_char();
            printf("DEBUG: Lexer returning TOKEN_LPAREN\n");
            return TOKEN_LPAREN;
        case ')':
            read_char();
            printf("DEBUG: Lexer returning TOKEN_RPAREN\n");
            return TOKEN_RPAREN;
        default:
            // Unknown character
            printf("DEBUG: Lexer returning TOKEN_UNKNOWN for char '%c'\n", current_char);
            read_char();
            return TOKEN_UNKNOWN;
    }
}
