
# ParseIQ Compiler

## Project Overview
ParseIQ is a custom arithmetic expression compiler that demonstrates various compiler components including lexical analysis, parsing, AST construction, error handling, and code generation. The project is implemented using Flex for lexical analysis and C for the parsing and code generation phases.

## Features
- **Lexical Analysis:** Implemented using Flex to tokenize identifiers, operators, and numeric values.
- **Parsing:** Recursive descent parsing with precedence climbing to handle expressions.
- **AST Construction:** Constructs an Abstract Syntax Tree to represent parsed expressions.
- **Code Generation:** Generates stack machine code and three-address code for evaluated expressions.
- **Error Handling:** Comprehensive error reporting and summarization during parsing and code generation.

## Project Structure
- `lexer.l` - Lexical analyzer implementation using Flex
- `tokens.h` - Token type definitions
- `parser.h` / `parser.c` - Parser implementation with recursive descent parsing
- `ast.h` / `ast.c` - AST structure definition and implementation
- `codegen.h` / `codegen.c` - Code generation for stack machine and three-address code
- `error.h` / `error.c` - Error handling and reporting
- `main.c` - Integration and main execution flow
- `Makefile` - Build configuration and dependency management
- `README.md` - Project documentation

## Build Instructions
Ensure that `gcc` and `flex` are installed on your system.  
To build the project, run:
```
make
```

## Usage
To run the compiler:
```
./parseiq [options] [expression | input_file]
```
Options:
- `--stack` : Generate stack machine code
- `--3addr` : Generate three-address code
- `--ast` : Display the AST structure
- `--tokens` : Display the token stream
- `--verbose` : Show all intermediate steps

## Example Usage
```
./parseiq "3 + 4 * (2 - 1)"
./parseiq --3addr "3 + 4 * 2"
./parseiq --stack input.txt
```

## Cleanup
To clean up generated files and object files, run:
```
make clean
```

## Contributors
- Kapil Singh - Lexical Analysis and Token Definitions
- Sarthak Chauhan - Parsing and AST Implementation
- Parinay Panwar - Code Generation and Error Handling
- Aakarshit Sharma - Integration and Build Configuration
