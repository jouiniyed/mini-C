#ifndef AST_TYPE_H
#define AST_TYPE_H

typedef enum {
    N_PLUS,
    N_MINUS,
    N_MUL,
    N_DIV
} TypeOperator;

typedef enum {
    N_EQUAL,
    N_NOT_EQUAL,
    N_LESS,
    N_GREATER,
    N_LESS_EQUAL,
    N_GREATER_EQUAL
} TypeComparisonOperator;

typedef enum {
    N_AND,
    N_OR
} TypeLogicalOperator;

typedef enum {
    NODE_VALUE,
    NODE_IDENTIFIER,
    NODE_OPERATION,
    NODE_COMPARISON,
    NODE_NOT,
    NODE_LOGICAL,
    NODE_ASSIGNMENT,
    NODE_SEQ_INST,
    NODE_CONDITION,
    NODE_WHILE
} NodeType;

#endif
