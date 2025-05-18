#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokens.h"
#include "parser.h"
#include "ast.h"
#include "codegen.h"
#include "error.h"

// These are now defined in lexer.c
extern TokenValue yylval;
extern int yylineno;
extern int yycolumn;

extern int yylex();
extern void set_input_file(FILE* file);

// Print usage information
void print_usage(const char* program_name) {
    printf("Usage: %s [options] [expression | input_file]\n\n", program_name);
    printf("Options:\n");
    printf("  --stack       Generate stack machine code\n");
    printf("  --3addr      Generate three-address code\n");
    printf("  --ast        Visualize the AST (default)\n");
    printf("  --tokens     Show token stream\n");
    printf("  --verbose    Show all intermediate steps\n");
    printf("  --help       Display this help message\n\n");
    printf("Examples:\n");
    printf("  %s \"2 + 3 * 4\"\n", program_name);
    printf("  %s --stack \"2 + 3 * 4\"\n", program_name);
    printf("  %s --3addr input.txt\n", program_name);
}

// Process an expression from a string
void process_expression(const char* expr, int show_tokens, int show_ast, 
                       int gen_stack, int gen_3addr, int verbose) {
    // Create a temporary file with the expression
    FILE* temp = tmpfile();
    if (!temp) {
        fprintf(stderr, "Error: Could not create temporary file\n");
        return;
    }
    
    fprintf(temp, "%s", expr);
    rewind(temp);
    
    // Initialize error handling
    error_init();
    
    // Set up the lexer and parser
    set_token_stream(temp);
    parser_reset();
    
    // Parse the expression
    ASTNode* root = parse_expression();
    
    // Check for errors
    if (error_count() > 0) {
        error_print_summary();
        if (!root) {
            fclose(temp);
            return;
        }
    }
    
    // Process the AST
    if (root) {
        // Show tokens if requested
        if (show_tokens || verbose) {
            printf("\nToken Stream:\n");
            printf("============\n");
            // This would normally show the token stream, but we've already consumed it
            printf("(Token stream not available after parsing)\n");
        }
        
        // Show AST if requested
        if (show_ast || verbose) {
            printf("\nAST Structure:\n");
            printf("=============\n");
            print_ast(root, 0);
            
            // Write AST to file
            write_ast_to_file(root, "ast_output.txt");
            printf("\nAST structure written to ast_output.txt\n");
        }
        
        // Generate stack machine code if requested
        if (gen_stack || verbose) {
            char stack_filename[256] = "stack_output.txt";
            if (generate_stack_code(root, stack_filename)) {
                printf("\nStack machine code written to %s\n", stack_filename);
                
                // Also print to console if verbose
                if (verbose) {
                    printf("\nStack Machine Code:\n");
                    printf("==================\n");
                    generate_stack_code(root, NULL); // NULL means stdout
                }
            }
        }
        
        // Generate three-address code if requested
        if (gen_3addr || verbose) {
            char addr_filename[256] = "3addr_output.txt";
            if (generate_three_addr_code(root, addr_filename)) {
                printf("\nThree-address code written to %s\n", addr_filename);
                
                // Also print to console if verbose
                if (verbose) {
                    printf("\nThree-Address Code:\n");
                    printf("=================\n");
                    generate_three_addr_code(root, NULL); // NULL means stdout
                }
            }
        }
        
        // Clean up
        free_ast(root);
    } else {
        printf("\nError: Failed to parse expression\n");
    }
    
    fclose(temp);
}

// Process an expression from a file
void process_file(const char* filename, int show_tokens, int show_ast, 
                 int gen_stack, int gen_3addr, int verbose) {
    FILE* input = fopen(filename, "r");
    if (!input) {
        fprintf(stderr, "Error: Could not open file: %s\n", filename);
        return;
    }
    
    // Initialize error handling
    error_init();
    
    // Set up the lexer and parser
    set_token_stream(input);
    parser_reset();
    
    // Parse the expression
    ASTNode* root = parse_expression();
    
    // Check for errors
    if (error_count() > 0) {
        error_print_summary();
        if (!root) {
            fclose(input);
            return;
        }
    }
    
    // Process the AST
    if (root) {
        // Generate base filename without extension
        char base_filename[256];
        strncpy(base_filename, filename, sizeof(base_filename) - 1);
        base_filename[sizeof(base_filename) - 1] = '\0';
        
        char* dot = strrchr(base_filename, '.');
        if (dot) {
            *dot = '\0'; // Remove extension
        }
        
        // Show tokens if requested
        if (show_tokens || verbose) {
            printf("\nToken Stream:\n");
            printf("============\n");
            // This would normally show the token stream, but we've already consumed it
            printf("(Token stream not available after parsing)\n");
        }
        
        // Show AST if requested
        if (show_ast || verbose) {
            printf("\nAST Structure:\n");
            printf("=============\n");
            print_ast(root, 0);
            
            // Write AST to file
            char ast_filename[256];
            snprintf(ast_filename, sizeof(ast_filename), "%s_ast.txt", base_filename);
            if (write_ast_to_file(root, ast_filename)) {
                printf("\nAST structure written to %s\n", ast_filename);
            }
        }
        
        // Generate stack machine code if requested
        if (gen_stack || verbose) {
            char stack_filename[256];
            snprintf(stack_filename, sizeof(stack_filename), "%s_stack.txt", base_filename);
            if (generate_stack_code(root, stack_filename)) {
                printf("\nStack machine code written to %s\n", stack_filename);
                
                // Also print to console if verbose
                if (verbose) {
                    printf("\nStack Machine Code:\n");
                    printf("==================\n");
                    generate_stack_code(root, NULL); // NULL means stdout
                }
            }
        }
        
        // Generate three-address code if requested
        if (gen_3addr || verbose) {
            char addr_filename[256];
            snprintf(addr_filename, sizeof(addr_filename), "%s_3addr.txt", base_filename);
            if (generate_three_addr_code(root, addr_filename)) {
                printf("\nThree-address code written to %s\n", addr_filename);
                
                // Also print to console if verbose
                if (verbose) {
                    printf("\nThree-Address Code:\n");
                    printf("=================\n");
                    generate_three_addr_code(root, NULL); // NULL means stdout
                }
            }
        }
        
        // Clean up
        free_ast(root);
    } else {
        printf("\nError: Failed to parse expression\n");
    }
    
    fclose(input);
}

int main(int argc, char** argv) {
    // Default options
    int show_tokens = 0;
    int show_ast = 1; // Show AST by default
    int gen_stack = 0;
    int gen_3addr = 0;
    int verbose = 0;
    
    // Check for help option
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
    }
    
    // Parse command-line options
    int arg_index = 1;
    while (arg_index < argc && argv[arg_index][0] == '-') {
        if (strcmp(argv[arg_index], "--stack") == 0) {
            gen_stack = 1;
            show_ast = 0; // Turn off default AST display
        } else if (strcmp(argv[arg_index], "--3addr") == 0) {
            gen_3addr = 1;
            show_ast = 0; // Turn off default AST display
        } else if (strcmp(argv[arg_index], "--ast") == 0) {
            show_ast = 1;
        } else if (strcmp(argv[arg_index], "--tokens") == 0) {
            show_tokens = 1;
            show_ast = 0; // Turn off default AST display
        } else if (strcmp(argv[arg_index], "--verbose") == 0) {
            verbose = 1;
            show_ast = 1;
            show_tokens = 1;
            gen_stack = 1;
            gen_3addr = 1;
        } else {
            fprintf(stderr, "Unknown option: %s\n\n", argv[arg_index]);
            print_usage(argv[0]);
            return 1;
        }
        arg_index++;
    }
    
    // Check if we have an input
    if (arg_index >= argc) {
        // No input provided, read from stdin
        printf("Enter an expression: ");
        fflush(stdout);
        
        // Read the expression from stdin
        char expr[1024];
        if (fgets(expr, sizeof(expr), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        
        // Process the expression
        process_expression(expr, show_tokens, show_ast, gen_stack, gen_3addr, verbose);
    } else {
        // Check if the argument is a file or an expression
        FILE* test_file = fopen(argv[arg_index], "r");
        if (test_file) {
            // It's a file
            fclose(test_file);
            process_file(argv[arg_index], show_tokens, show_ast, gen_stack, gen_3addr, verbose);
        } else {
            // It's an expression
            process_expression(argv[arg_index], show_tokens, show_ast, gen_stack, gen_3addr, verbose);
        }
    }
    
    return 0;
}
