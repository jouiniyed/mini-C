#ifndef PARSER_H
#define PARSER_H

#include "ast_constructor.h"
#include "lexer.h"
#include "ast_type.h"
#include "symbol_table.h"
#include "ast_traversal.h"

void parse(char *filename, AstNode **A);

void rec_gae();

void rec_seq_term();


void rec_seq_term_tail();


void rec_term();


void rec_seq_factor();

void rec_seq_factor_tail();


void rec_factor();

void rec_seq_assignment(AstNode **A);

void rec_assignment(AstNode **A);

void rec_relation(AstNode **A);

void rec_primary(AstNode **A);

void rec_bool_term(AstNode **A);

void rec_boolean_expression(AstNode **A);

void rec_program(AstNode **A);

void rec_seq_inst(AstNode **A);

void rec_seq_inst_tail(AstNode **A);

void rec_if_part(AstNode **A);

void rec_else_part(AstNode **A);

void rec_while_loop(AstNode **A);

//void rec_expression(AstNode **A); // Recently added 

int op_star_slash();

int op_plus_minus();

TypeOperator get_opperator(TokenType type);

TypeComparisonOperator get_comparison_operator(TokenType type);

#endif
