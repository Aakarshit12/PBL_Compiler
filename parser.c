#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

static TokenType current_token;
static TokenValue current_value;

// Forward declaration of static function
static TokenType next_token();

void set_token_stream(FILE* input) {
    // Call the lexer's set_input_file function
    extern void set_input_file(FILE* file);
    set_input_file(input);
}

void parser_reset() {
    current_token = TOKEN_UNKNOWN;
    next_token(); // Get the first token to start parsing
}

// Get the next token from our lexer
static TokenType next_token() {
    extern int yylex();
    current_token = yylex();
    current_value = yylval;
    return current_token;
}

// Forward declarations
static ASTNode* parse_term();
static ASTNode* parse_factor();

ASTNode* parse_expression() {
    printf("DEBUG: Entering parse_expression, current_token = %d\n", current_token);
    ASTNode* left = parse_term();
    printf("DEBUG: In parse_expression after parse_term, current_token = %d\n", current_token);
    
    if (left == NULL) {
        printf("DEBUG: parse_term returned NULL\n");
        return NULL;
    }
    
    while (current_token == TOKEN_PLUS || current_token == TOKEN_MINUS) {
        OperatorType op = (current_token == TOKEN_PLUS) ? OP_ADD : OP_SUBTRACT;
        printf("DEBUG: In parse_expression loop, found %s\n", (current_token == TOKEN_PLUS) ? "+" : "-");
        next_token();
        ASTNode* right = parse_term();
        
        if (right == NULL) {
            printf("DEBUG: Right side of expression is NULL\n");
            free_ast(left);
            return NULL;
        }
        
        left = create_binary_node(left, op, right, left->line, left->column);
        printf("DEBUG: Created binary node for %s\n", (op == OP_ADD) ? "+" : "-");
    }
    
    printf("DEBUG: Exiting parse_expression, returning node of type %d\n", left ? left->type : -1);
    return left;
}

static ASTNode* parse_term() {
    printf("DEBUG: Entering parse_term, current_token = %d\n", current_token);
    ASTNode* left = parse_factor();
    
    if (left == NULL) {
        printf("DEBUG: parse_factor returned NULL in parse_term\n");
        return NULL;
    }
    
    printf("DEBUG: In parse_term after parse_factor, current_token = %d\n", current_token);
    
    while (current_token == TOKEN_MUL || current_token == TOKEN_DIV) {
        OperatorType op = (current_token == TOKEN_MUL) ? OP_MULTIPLY : OP_DIVIDE;
        printf("DEBUG: In parse_term loop, found %s\n", (current_token == TOKEN_MUL) ? "*" : "/");
        next_token();
        ASTNode* right = parse_factor();
        
        if (right == NULL) {
            printf("DEBUG: Right side of term is NULL\n");
            free_ast(left);
            return NULL;
        }
        
        left = create_binary_node(left, op, right, left->line, left->column);
        printf("DEBUG: Created binary node for %s\n", (op == OP_MULTIPLY) ? "*" : "/");
    }
    
    printf("DEBUG: Exiting parse_term, returning node of type %d\n", left ? left->type : -1);
    return left;
}

static ASTNode* parse_factor() {
    printf("DEBUG: Entering parse_factor, current_token = %d\n", current_token);
    
    if (current_token == TOKEN_INT) {
        printf("DEBUG: Found INTEGER: %d\n", current_value.ival);
        ASTNode* node = create_number_node(current_value.ival, yylineno, yycolumn);
        next_token();
        printf("DEBUG: Exiting parse_factor with INTEGER node, next token = %d\n", current_token);
        return node;
    } else if (current_token == TOKEN_FLOAT) {
        printf("DEBUG: Found FLOAT: %f\n", current_value.fval);
        ASTNode* node = create_number_node(current_value.fval, yylineno, yycolumn);
        next_token();
        printf("DEBUG: Exiting parse_factor with FLOAT node, next token = %d\n", current_token);
        return node;
    } else if (current_token == TOKEN_VARIABLE) {
        printf("DEBUG: Found VARIABLE: %c\n", current_value.cval);
        ASTNode* node = create_variable_node(current_value.cval, yylineno, yycolumn);
        next_token();
        printf("DEBUG: Exiting parse_factor with VARIABLE node, next token = %d\n", current_token);
        return node;
    } else if (current_token == TOKEN_LPAREN) {
        printf("DEBUG: Found LPAREN\n");
        next_token();
        ASTNode* node = parse_expression();
        if (node == NULL) {
            printf("DEBUG: Expression inside parentheses is NULL\n");
            return NULL;
        }
        
        if (current_token == TOKEN_RPAREN) {
            printf("DEBUG: Found matching RPAREN\n");
            next_token();
        } else {
            printf("DEBUG: Missing RPAREN, found token = %d\n", current_token);
            // Error: expected ')'
            free_ast(node);
            return create_error_node(yylineno, yycolumn);
        }
        printf("DEBUG: Exiting parse_factor with parenthesized expression, next token = %d\n", current_token);
        return node;
    } else {
        printf("DEBUG: Unexpected token in parse_factor: %d\n", current_token);
        // Error: unexpected token
        return create_error_node(yylineno, yycolumn);
    }
}
