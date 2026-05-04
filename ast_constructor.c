#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ast_constructor.h"

AstNode *create_value(int value){
    AstNode *node = malloc(sizeof(AstNode));
    assert(node && "Memory allocation failed\n");
    node->node_type = NODE_VALUE;
    node->value = value;
    return node;
}

AstNode *create_identifier(char *idf){
    AstNode *node = malloc(sizeof(AstNode));
    assert(node && "Memory allocation failed\n");
    node->node_type = NODE_IDENTIFIER;
    strncpy(node->idf, idf, MAX_IDF_LENGTH);
    node->idf[MAX_IDF_LENGTH - 1] = '\0';
    return node;
}

AstNode *create_operation(TypeOperator op, AstNode *left, AstNode *right){
    AstNode *node = malloc(sizeof(AstNode));
    assert(node && "Memory allocation failed\n");
    node->node_type = NODE_OPERATION;
    node->operation.op = op;
    node->operation.left = left;
    node->operation.right = right;
    return node;
}

AstNode *create_comparison_operation(TypeComparisonOperator op, AstNode *left, AstNode *right){
    AstNode *node = malloc(sizeof(AstNode));
    assert(node && "Memory allocation failed\n");
    node->node_type = NODE_COMPARISON;
    node->comparison.op = op;
    node->comparison.left = left;
    node->comparison.right = right;
    return node;
}

AstNode *create_not_operation(AstNode *operand){
    AstNode *node = malloc(sizeof(AstNode));
    assert(node && "Memory allocation failed\n");
    node->node_type = NODE_NOT;
    node->not_op.operand = operand;
    return node;
}

AstNode *create_and_or_operation(TypeLogicalOperator op, AstNode *left, AstNode *right){
    AstNode *node = malloc(sizeof(AstNode));
    assert(node && "Memory allocation failed\n");
    node->node_type = NODE_LOGICAL;
    node->logical.op = op;
    node->logical.left = left;
    node->logical.right = right;
    return node;
}

AstNode *create_assignment(AstNode *idf, AstNode *expr){
    AstNode *node = malloc(sizeof(AstNode));
    assert(node && "Memory allocation failed\n");
    node->node_type = NODE_ASSIGNMENT;
    node->assignment.idf_node = idf;
    node->assignment.expr_node = expr;
    return node;
}

AstNode *create_seq_inst(AstNode *first, AstNode *second){
    AstNode *node = malloc(sizeof(AstNode));
    assert(node && "Memory allocation failed\n");
    node->node_type = NODE_SEQ_INST;
    node->seq.first = first;
    node->seq.second = second;
    return node;
}

AstNode *create_condition(AstNode *cond, AstNode *then_branch, AstNode *else_branch){
    AstNode *node = malloc(sizeof(AstNode));
    assert(node && "Memory allocation failed\n");
    node->node_type = NODE_CONDITION;
    node->condition.cond = cond;
    node->condition.then_branch = then_branch;
    node->condition.else_branch = else_branch;
    return node;
}

AstNode *create_while(AstNode *cond, AstNode *body){
    AstNode *node = malloc(sizeof(AstNode));
    assert(node && "Memory allocation failed\n");
    node->node_type = NODE_WHILE;
    node->while_loop.cond = cond;
    node->while_loop.body = body;
    return node;
}

void free_ast(AstNode *node){
    if(node == NULL) return;
    switch(node->node_type){
        case NODE_VALUE:
        case NODE_IDENTIFIER:
        break;

        case NODE_OPERATION:
            free_ast(node->operation.left);
            free_ast(node->operation.right);
        break;

        case NODE_COMPARISON:
            free_ast(node->comparison.left);
            free_ast(node->comparison.right);
        break;

        case NODE_NOT:
            free_ast(node->not_op.operand);
        break;

        case NODE_LOGICAL:
            free_ast(node->logical.left);
            free_ast(node->logical.right);
        break;

        case NODE_ASSIGNMENT:
            free_ast(node->assignment.idf_node);
            free_ast(node->assignment.expr_node);
        break;

        case NODE_SEQ_INST:
            free_ast(node->seq.first);
            free_ast(node->seq.second);
        break;

        case NODE_CONDITION:
            free_ast(node->condition.cond);
            free_ast(node->condition.then_branch);
            free_ast(node->condition.else_branch);
        break;

        case NODE_WHILE:
            free_ast(node->while_loop.cond);
            free_ast(node->while_loop.body);
        break;
    }
    free(node);
}
