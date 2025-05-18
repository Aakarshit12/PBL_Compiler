#ifndef AST_H
#define AST_H

#include <stdio.h>

typedef enum {
    NODE_BINARY_OP,
    NODE_NUMBER,
    NODE_VARIABLE,
    NODE_ERROR
} NodeType;

typedef enum {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_POWER
} OperatorType;

typedef struct ASTNode {
    NodeType type;
    int line;
    int column;
    union {
        struct {
            struct ASTNode* left;
            struct ASTNode* right;
            OperatorType operator;
        } binary_op;
        double value;
        char name;
    } data;
} ASTNode;

ASTNode* create_number_node(double value, int line, int column);
ASTNode* create_variable_node(char name, int line, int column);
ASTNode* create_binary_node(ASTNode* left, OperatorType op, ASTNode* right, int line, int column);
ASTNode* create_error_node(int line, int column);
void free_ast(ASTNode* node);
void print_ast(const ASTNode* node, int indent);

/* Write the AST to a file
 * @param node The root node of the AST
 * @param filename The name of the file to write to
 * @return 1 on success, 0 on failure
 */
int write_ast_to_file(const ASTNode* node, const char* filename);

#endif // AST_H
