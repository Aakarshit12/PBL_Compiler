#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ERRORS 100
#define MAX_ERROR_MSG_LEN 256

typedef struct {
    ErrorType type;
    int line;
    int column;
    char message[MAX_ERROR_MSG_LEN];
} ErrorRecord;

static ErrorRecord errors[MAX_ERRORS];
static int error_counter = 0;

void error_init() {
    error_counter = 0;
}

void error_report(ErrorType type, int line, int column, const char* message) {
    if (error_counter >= MAX_ERRORS) {
        fprintf(stderr, "Too many errors, stopping compilation\n");
        return;
    }
    
    errors[error_counter].type = type;
    errors[error_counter].line = line;
    errors[error_counter].column = column;
    strncpy(errors[error_counter].message, message, MAX_ERROR_MSG_LEN - 1);
    errors[error_counter].message[MAX_ERROR_MSG_LEN - 1] = '\0';
    
    // Print the error immediately
    const char* type_str = "";
    switch (type) {
        case ERROR_SYNTAX: type_str = "Syntax Error"; break;
        case ERROR_UNEXPECTED_TOKEN: type_str = "Unexpected Token"; break;
        case ERROR_MISSING_TOKEN: type_str = "Missing Token"; break;
        case ERROR_UNDEFINED_VARIABLE: type_str = "Undefined Variable"; break;
        case ERROR_DIVISION_BY_ZERO: type_str = "Division by Zero"; break;
        case ERROR_INTERNAL: type_str = "Internal Error"; break;
    }
    
    fprintf(stderr, "%s at line %d, column %d: %s\n", 
            type_str, line, column, message);
    
    error_counter++;
}

int error_count() {
    return error_counter;
}

void error_print_summary() {
    if (error_counter == 0) {
        printf("No errors detected.\n");
        return;
    }
    
    printf("\nError Summary (%d error%s):\n", 
           error_counter, error_counter == 1 ? "" : "s");
    printf("======================\n");
    
    for (int i = 0; i < error_counter; i++) {
        const char* type_str = "";
        switch (errors[i].type) {
            case ERROR_SYNTAX: type_str = "Syntax Error"; break;
            case ERROR_UNEXPECTED_TOKEN: type_str = "Unexpected Token"; break;
            case ERROR_MISSING_TOKEN: type_str = "Missing Token"; break;
            case ERROR_UNDEFINED_VARIABLE: type_str = "Undefined Variable"; break;
            case ERROR_DIVISION_BY_ZERO: type_str = "Division by Zero"; break;
            case ERROR_INTERNAL: type_str = "Internal Error"; break;
        }
        
        printf("%d. %s at line %d, column %d: %s\n", 
               i + 1, type_str, errors[i].line, errors[i].column, errors[i].message);
    }
}

void error_reset() {
    error_counter = 0;
}
