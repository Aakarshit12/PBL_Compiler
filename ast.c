#include "ast.h"
#include <stdlib.h>
#include <stdio.h>

ASTNode* create_number_node(double value, int line, int column) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_NUMBER;
    node->line = line;
    node->column = column;
    node->data.value = value;
    return node;
}

ASTNode* create_variable_node(char name, int line, int column) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_VARIABLE;
    node->line = line;
    node->column = column;
    node->data.name = name;
    return node;
}

ASTNode* create_binary_node(ASTNode* left, OperatorType op, ASTNode* right, int line, int column) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_BINARY_OP;
    node->line = line;
    node->column = column;
    node->data.binary_op.left = left;
    node->data.binary_op.right = right;
    node->data.binary_op.operator = op;
    return node;
}

ASTNode* create_error_node(int line, int column) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_ERROR;
    node->line = line;
    node->column = column;
    return node;
}

void free_ast(ASTNode* node) {
    if (!node) return;
    if (node->type == NODE_BINARY_OP) {
        free_ast(node->data.binary_op.left);
        free_ast(node->data.binary_op.right);
    }
    free(node);
}

void print_ast(const ASTNode* node, int indent) {
    if (!node) {
        printf("NULL Node\n");
        return;
    }
    
    for (int i = 0; i < indent; ++i) printf("  ");
    
    switch (node->type) {
        case NODE_NUMBER:
            printf("Number(%.2f)\n", node->data.value);
            break;
            
        case NODE_VARIABLE:
            printf("Variable(%c)\n", node->data.name);
            break;
            
        case NODE_BINARY_OP:
            printf("BinaryOp(");
            switch (node->data.binary_op.operator) {
                case OP_ADD: printf("+"); break;
                case OP_SUBTRACT: printf("-"); break;
                case OP_MULTIPLY: printf("*"); break;
                case OP_DIVIDE: printf("/"); break;
                case OP_POWER: printf("^"); break;
                default: printf("unknown"); break;
            }
            printf(")\n");
            
            if (node->data.binary_op.left) {
                print_ast(node->data.binary_op.left, indent + 1);
            } else {
                for (int i = 0; i < indent + 1; ++i) printf("  ");
                printf("Left: NULL\n");
            }
            
            if (node->data.binary_op.right) {
                print_ast(node->data.binary_op.right, indent + 1);
            } else {
                for (int i = 0; i < indent + 1; ++i) printf("  ");
                printf("Right: NULL\n");
            }
            break;
            
        case NODE_ERROR:
            printf("ErrorNode\n");
            break;
            
        default:
            printf("Unknown node type\n");
            break;
    }
}

// Helper function to write AST to a file with indentation
static void write_ast_to_file_helper(const ASTNode* node, FILE* file, int indent) {
    if (!node) {
        fprintf(file, "NULL Node\n");
        return;
    }
    
    for (int i = 0; i < indent; ++i) fprintf(file, "  ");
    
    switch (node->type) {
        case NODE_NUMBER:
            fprintf(file, "Number(%.2f)\n", node->data.value);
            break;
            
        case NODE_VARIABLE:
            fprintf(file, "Variable(%c)\n", node->data.name);
            break;
            
        case NODE_BINARY_OP:
            fprintf(file, "BinaryOp(");
            switch (node->data.binary_op.operator) {
                case OP_ADD: fprintf(file, "+"); break;
                case OP_SUBTRACT: fprintf(file, "-"); break;
                case OP_MULTIPLY: fprintf(file, "*"); break;
                case OP_DIVIDE: fprintf(file, "/"); break;
                case OP_POWER: fprintf(file, "^"); break;
                default: fprintf(file, "unknown"); break;
            }
            fprintf(file, ")\n");
            
            if (node->data.binary_op.left) {
                write_ast_to_file_helper(node->data.binary_op.left, file, indent + 1);
            } else {
                for (int i = 0; i < indent + 1; ++i) fprintf(file, "  ");
                fprintf(file, "Left: NULL\n");
            }
            
            if (node->data.binary_op.right) {
                write_ast_to_file_helper(node->data.binary_op.right, file, indent + 1);
            } else {
                for (int i = 0; i < indent + 1; ++i) fprintf(file, "  ");
                fprintf(file, "Right: NULL\n");
            }
            break;
            
        case NODE_ERROR:
            fprintf(file, "ErrorNode\n");
            break;
            
        default:
            fprintf(file, "Unknown node type\n");
            break;
    }
}

int write_ast_to_file(const ASTNode* node, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file %s for writing\n", filename);
        return 0;
    }
    
    fprintf(file, "AST Structure:\n");
    write_ast_to_file_helper(node, file, 0);
    
    fclose(file);
    return 1;
}
