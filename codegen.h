#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"

/* Generate stack machine code from AST
 * @param node The root node of the AST
 * @param filename The name of the file to write to (or NULL for stdout)
 * @return 1 on success, 0 on failure
 */
int generate_stack_code(const ASTNode* node, const char* filename);

/* Generate three-address code from AST
 * @param node The root node of the AST
 * @param filename The name of the file to write to (or NULL for stdout)
 * @return 1 on success, 0 on failure
 */
int generate_three_addr_code(const ASTNode* node, const char* filename);

#endif // CODEGEN_H
