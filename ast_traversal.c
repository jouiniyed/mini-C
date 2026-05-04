#include <stdio.h>
#include <stdlib.h>
#include "ast_traversal.h"

static void print_ast_rec(AstNode *node, int depth){
    int i;
    if(node == NULL) return;
    for(i = 0; i < depth; i++) printf("  ");
    switch(node->node_type){
        case NODE_VALUE:
            printf("VALUE(%d)\n", node->value);
        break;

        case NODE_IDENTIFIER:
            printf("IDENTIFIER(%s)\n", node->idf);
        break;

        case NODE_OPERATION: {
            char *op_str;
            switch(node->operation.op){
                case N_PLUS:  op_str = "+"; break;
                case N_MINUS: op_str = "-"; break;
                case N_MUL:   op_str = "*"; break;
                case N_DIV:   op_str = "/"; break;
                default:      op_str = "?"; break;
            }
            printf("OPERATION(%s)\n", op_str);
            print_ast_rec(node->operation.left, depth + 1);
            print_ast_rec(node->operation.right, depth + 1);
        }
        break;

        case NODE_COMPARISON: {
            char *op_str;
            switch(node->comparison.op){
                case N_EQUAL:         op_str = "=="; break;
                case N_NOT_EQUAL:     op_str = "!="; break;
                case N_LESS:          op_str = "<";  break;
                case N_GREATER:       op_str = ">";  break;
                case N_LESS_EQUAL:    op_str = "<="; break;
                case N_GREATER_EQUAL: op_str = ">="; break;
                default:              op_str = "?";  break;
            }
            printf("COMPARISON(%s)\n", op_str);
            print_ast_rec(node->comparison.left, depth + 1);
            print_ast_rec(node->comparison.right, depth + 1);
        }
        break;

        case NODE_NOT:
            printf("NOT\n");
            print_ast_rec(node->not_op.operand, depth + 1);
        break;

        case NODE_LOGICAL: {
            char *op_str;
            switch(node->logical.op){
                case N_AND: op_str = "&&"; break;
                case N_OR:  op_str = "||"; break;
                default:    op_str = "?";  break;
            }
            printf("LOGICAL(%s)\n", op_str);
            print_ast_rec(node->logical.left, depth + 1);
            print_ast_rec(node->logical.right, depth + 1);
        }
        break;

        case NODE_ASSIGNMENT:
            printf("ASSIGNMENT\n");
            print_ast_rec(node->assignment.idf_node, depth + 1);
            print_ast_rec(node->assignment.expr_node, depth + 1);
        break;

        case NODE_SEQ_INST:
            printf("SEQ_INST\n");
            print_ast_rec(node->seq.first, depth + 1);
            print_ast_rec(node->seq.second, depth + 1);
        break;

        case NODE_CONDITION:
            printf("IF\n");
            print_ast_rec(node->condition.cond, depth + 1);
            for(i = 0; i < depth + 1; i++) printf("  ");
            printf("THEN\n");
            print_ast_rec(node->condition.then_branch, depth + 2);
            if(node->condition.else_branch != NULL){
                for(i = 0; i < depth + 1; i++) printf("  ");
                printf("ELSE\n");
                print_ast_rec(node->condition.else_branch, depth + 2);
            }
        break;

        case NODE_WHILE:
            printf("WHILE\n");
            print_ast_rec(node->while_loop.cond, depth + 1);
            for(i = 0; i < depth + 1; i++) printf("  ");
            printf("DO\n");
            print_ast_rec(node->while_loop.body, depth + 2);
        break;
    }
}

void print_ast(AstNode *node){
    print_ast_rec(node, 0);
}

int bool_evaluation(AstNode *node){
    int left;
    int right;
    if(node == NULL) return 0;
    switch(node->node_type){
        case NODE_VALUE:
            return node->value;

        case NODE_IDENTIFIER:
            return 0;

        case NODE_OPERATION:
            left = bool_evaluation(node->operation.left);
            right = bool_evaluation(node->operation.right);
            switch(node->operation.op){
                case N_PLUS:  return left + right;
                case N_MINUS: return left - right;
                case N_MUL:   return left * right;
                case N_DIV:
                    if(right == 0){
                        printf("ERROR: Division by zero\n");
                        exit(1);
                    }
                    return left / right;
                default: return 0;
            }

        case NODE_COMPARISON:
            left = bool_evaluation(node->comparison.left);
            right = bool_evaluation(node->comparison.right);
            switch(node->comparison.op){
                case N_EQUAL:         return left == right;
                case N_NOT_EQUAL:     return left != right;
                case N_LESS:          return left < right;
                case N_GREATER:       return left > right;
                case N_LESS_EQUAL:    return left <= right;
                case N_GREATER_EQUAL: return left >= right;
                default: return 0;
            }

        case NODE_NOT:
            return !bool_evaluation(node->not_op.operand);

        case NODE_LOGICAL:
            left = bool_evaluation(node->logical.left);
            right = bool_evaluation(node->logical.right);
            switch(node->logical.op){
                case N_AND: return left && right;
                case N_OR:  return left || right;
                default: return 0;
            }

        default:
            return 0;
    }
}

int evaluation(AstNode *node){
    int left;
    int right;
    if(node == NULL) return 0;
    switch(node->node_type){
        case NODE_VALUE:
            return node->value;

        case NODE_OPERATION:
            left = evaluation(node->operation.left);
            right = evaluation(node->operation.right);
            switch(node->operation.op){
                case N_PLUS:  return left + right;
                case N_MINUS: return left - right;
                case N_MUL:   return left * right;
                case N_DIV:
                    if(right == 0){
                        printf("ERROR: Division by zero\n");
                        exit(1);
                    }
                    return left / right;
                default: return 0;
            }

        default:
            printf("ERROR: non-arithmetic node in evaluation\n");
            exit(1);
    }
}
