#ifndef AST_TRAVERSAL_H
#define AST_TRAVERSAL_H

#include "ast_constructor.h"

void print_ast(AstNode *node);
int bool_evaluation(AstNode *node);
int evaluation(AstNode *node);

#endif
