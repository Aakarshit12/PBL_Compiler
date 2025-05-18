# ParseIQ Compiler

A custom arithmetic expression compiler demonstrating hand-written parsing, AST construction, optimizations, and code generation.

## Features
- Flex-based lexical analysis
- Hand-written recursive descent parser with precedence climbing
- Custom AST structure
- Error detection and recovery
- Foundation for optimizations and code generation

## Build Instructions

Requires GCC and Flex.

```
make
```

## Usage

```
./parseiq [inputfile]
```
If no input file is given, reads from stdin.

## Project Structure
- `lexer.l` — Flex lexer
- `tokens.h` — Token definitions
- `ast.h`, `ast.c` — AST node structure
- `parser.h`, `parser.c` — Parser implementation
- `main.c` — Driver

## Next Steps
- Implement optimizations (constant folding, algebraic simplification)
- Add code generation (stack machine and three-address code)
- Add error handling improvements and visualization tools

## Example

Input:
```
3 + 4 * (2 - 1)
```

AST Output:
```
BinaryOp(+)
  Number(3.00)
  BinaryOp(*)
    Number(4.00)
    BinaryOp(-)
      Number(2.00)
      Number(1.00)
```
