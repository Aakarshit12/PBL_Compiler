#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to generate stack code recursively
static void generate_stack_code_helper(const ASTNode* node, FILE* output) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_NUMBER:
            fprintf(output, "PUSH %.2f\n", node->data.value);
            break;
            
        case NODE_VARIABLE:
            fprintf(output, "LOAD %c\n", node->data.name);
            break;
            
        case NODE_BINARY_OP:
            // Generate code for operands first (postorder traversal)
            generate_stack_code_helper(node->data.binary_op.left, output);
            generate_stack_code_helper(node->data.binary_op.right, output);
            
            // Generate operation instruction
            switch (node->data.binary_op.operator) {
                case OP_ADD: fprintf(output, "ADD\n"); break;
                case OP_SUBTRACT: fprintf(output, "SUB\n"); break;
                case OP_MULTIPLY: fprintf(output, "MUL\n"); break;
                case OP_DIVIDE: fprintf(output, "DIV\n"); break;
                case OP_POWER: fprintf(output, "POW\n"); break;
            }
            break;
            
        case NODE_ERROR:
            fprintf(output, "ERROR\n");
            break;
    }
}

int generate_stack_code(const ASTNode* node, const char* filename) {
    FILE* output = stdout;
    int close_file = 0;
    
    if (filename) {
        output = fopen(filename, "w");
        if (!output) {
            printf("Error: Could not open file %s for writing\n", filename);
            return 0;
        }
        close_file = 1;
    }
    
    fprintf(output, "# Stack Machine Code\n");
    fprintf(output, "# ==================\n\n");
    
    generate_stack_code_helper(node, output);
    
    if (close_file) {
        fclose(output);
    }
    
    return 1;
}

// Helper function for three-address code generation
static int temp_var_counter = 0;

// Structure to hold the result of code generation
typedef struct {
    char* var_name;  // Name of the variable holding the result
    int is_temp;     // Whether this is a temporary variable
} CodeGenResult;

// Helper function to generate a new temporary variable name
static char* new_temp_var() {
    char* var_name = (char*)malloc(10);
    sprintf(var_name, "t%d", ++temp_var_counter);
    return var_name;
}

// Helper function to generate three-address code recursively
static CodeGenResult generate_three_addr_code_helper(const ASTNode* node, FILE* output) {
    CodeGenResult result = {NULL, 0};
    
    if (!node) {
        result.var_name = strdup("ERROR");
        return result;
    }
    
    switch (node->type) {
        case NODE_NUMBER: {
            char* var_name = new_temp_var();
            fprintf(output, "%s = %.2f\n", var_name, node->data.value);
            result.var_name = var_name;
            result.is_temp = 1;
            break;
        }
            
        case NODE_VARIABLE: {
            // For variables, we just use the variable name directly
            result.var_name = (char*)malloc(2);
            result.var_name[0] = node->data.name;
            result.var_name[1] = '\0';
            result.is_temp = 0;
            break;
        }
            
        case NODE_BINARY_OP: {
            // Generate code for left and right operands
            CodeGenResult left = generate_three_addr_code_helper(node->data.binary_op.left, output);
            CodeGenResult right = generate_three_addr_code_helper(node->data.binary_op.right, output);
            
            // Create a new temporary variable for the result
            char* var_name = new_temp_var();
            
            // Generate the operation
            char op_char;
            switch (node->data.binary_op.operator) {
                case OP_ADD: op_char = '+'; break;
                case OP_SUBTRACT: op_char = '-'; break;
                case OP_MULTIPLY: op_char = '*'; break;
                case OP_DIVIDE: op_char = '/'; break;
                case OP_POWER: op_char = '^'; break;
                default: op_char = '?'; break;
            }
            
            fprintf(output, "%s = %s %c %s\n", var_name, left.var_name, op_char, right.var_name);
            
            // Clean up temporary variables
            if (left.is_temp) free(left.var_name);
            if (right.is_temp) free(right.var_name);
            
            result.var_name = var_name;
            result.is_temp = 1;
            break;
        }
            
        case NODE_ERROR: {
            result.var_name = strdup("ERROR");
            result.is_temp = 1;
            break;
        }
    }
    
    return result;
}

int generate_three_addr_code(const ASTNode* node, const char* filename) {
    FILE* output = stdout;
    int close_file = 0;
    
    if (filename) {
        output = fopen(filename, "w");
        if (!output) {
            printf("Error: Could not open file %s for writing\n", filename);
            return 0;
        }
        close_file = 1;
    }
    
    fprintf(output, "# Three-Address Code\n");
    fprintf(output, "# =================\n\n");
    
    // Reset temporary variable counter
    temp_var_counter = 0;
    
    CodeGenResult result = generate_three_addr_code_helper(node, output);
    
    // Print the final result variable
    fprintf(output, "\n# Result is in variable: %s\n", result.var_name);
    
    // Clean up
    if (result.is_temp) free(result.var_name);
    
    if (close_file) {
        fclose(output);
    }
    
    return 1;
}
