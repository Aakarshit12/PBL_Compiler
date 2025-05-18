#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "tokens.h"

ASTNode* parse_expression();
void set_token_stream(FILE* input);
void parser_reset();

#endif // PARSER_H
