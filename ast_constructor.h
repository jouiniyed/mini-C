#ifndef AST_CONSTRUCTOR_H
#define AST_CONSTRUCTOR_H

#include "ast_type.h"
#include "symbol_table.h"

typedef struct AstNode {
    NodeType node_type;
    union {
        int value;
        char idf[MAX_IDF_LENGTH];
        struct {
            TypeOperator op;
            struct AstNode *left;
            struct AstNode *right;
        } operation;
        struct {
            TypeComparisonOperator op;
            struct AstNode *left;
            struct AstNode *right;
        } comparison;
        struct {
            struct AstNode *operand;
        } not_op;
        struct {
            TypeLogicalOperator op;
            struct AstNode *left;
            struct AstNode *right;
        } logical;
        struct {
            struct AstNode *idf_node;
            struct AstNode *expr_node;
        } assignment;
        struct {
            struct AstNode *first;
            struct AstNode *second;
        } seq;
        struct {
            struct AstNode *cond;
            struct AstNode *then_branch;
            struct AstNode *else_branch;
        } condition;
        struct {
            struct AstNode *cond;
            struct AstNode *body;
        } while_loop;
    };
} AstNode;

AstNode *create_value(int value);
AstNode *create_identifier(char *idf);
AstNode *create_operation(TypeOperator op, AstNode *left, AstNode *right);
AstNode *create_comparison_operation(TypeComparisonOperator op, AstNode *left, AstNode *right);
AstNode *create_not_operation(AstNode *operand);
AstNode *create_and_or_operation(TypeLogicalOperator op, AstNode *left, AstNode *right);
AstNode *create_assignment(AstNode *idf, AstNode *expr);
AstNode *create_seq_inst(AstNode *first, AstNode *second);
AstNode *create_condition(AstNode *cond, AstNode *then_branch, AstNode *else_branch);
AstNode *create_while(AstNode *cond, AstNode *body);
void free_ast(AstNode *node);

#endif
