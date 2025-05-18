#ifndef ERROR_H
#define ERROR_H

// Error types
typedef enum {
    ERROR_SYNTAX,
    ERROR_UNEXPECTED_TOKEN,
    ERROR_MISSING_TOKEN,
    ERROR_UNDEFINED_VARIABLE,
    ERROR_DIVISION_BY_ZERO,
    ERROR_INTERNAL
} ErrorType;

// Initialize the error handling system
void error_init();

// Report an error with line and column information
void error_report(ErrorType type, int line, int column, const char* message);

// Get the number of errors reported so far
int error_count();

// Print a summary of all errors
void error_print_summary();

// Reset the error system
void error_reset();

#endif // ERROR_H
