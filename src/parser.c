#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"


SymbolTable table = {.head = NULL};


// Ensures that the whole input is consumed after parsing 
// Detects extra tokens like unexpected parentheses 
void parse(char *filename, AstNode **A){
    start(filename);
    rec_program(A);
    if(current_token().type == END_OF_SEQUENCE){
        printf("SYNTAX IS OK\n");
        printf("\n");
        print_table(&table);
    } else {
        printf("ERROR: unexpected token (ligne %u, colonne %u) : [%s]\n",
               current_token().ligne,
               current_token().column,
               Type_to_string(current_token().type));
        exit(1);
    }
}

void rec_gae(AstNode **A){
    rec_seq_term(A);
}

void rec_seq_term(AstNode **A){
    AstNode *B;
    rec_term(&B);
    rec_seq_term_tail(B, A);
}


void rec_seq_term_tail(AstNode *A, AstNode **B){
    
    AstNode *C = NULL;
    AstNode *D = NULL;
    TypeOperator op;

    if(op_plus_minus(&op)){
        rec_term(&C);
        D = create_operation(op,A,C);
        rec_seq_term_tail(D,B);

    } else {
        *B = A;
    }

}




void rec_term(AstNode **A){
    rec_seq_factor(A);
}


void rec_seq_factor(AstNode **A){
    AstNode *B;
    rec_factor(&B);
    rec_seq_factor_tail(B,A);
}


void rec_seq_factor_tail(AstNode *A, AstNode **B){
    
    AstNode *C = NULL;
    AstNode *D = NULL;

    TypeOperator op;

    if(op_star_slash(&op)){
        rec_factor(&C);
        D = create_operation(op, A, C);
        rec_seq_factor_tail(D, B);
    } else {
        *B = A;
    }

}


void rec_factor(AstNode **A){

    int value;
    int found;
    int bool_value;
    switch(current_token().type){
        case INTEGER:
            *A = create_value(current_token().value);
            advance();
        break;

        case LPAR:
            advance();
            //rec_gae(A); 
            //rec_relation(A);
            rec_boolean_expression(A); 
            if(current_token().type == RPAR){
                advance();
            } else {
                printf("ERROR: closing parenthesis expected (ligne %u, column %u)\n",current_token().ligne,current_token().column);
                exit(1);
            }
        break;
        
        case IDENTIFIER: 
            found = lookup(&table,current_token().string,&value, &bool_value);
            if(found){
                *A = create_value(value);
                advance();
            } else {
                printf("ERROR: %s is not declared\n",current_token().string);
                exit(1);

                /*
                    *A = create_identifier(current_token().string);
                    advance();
                */
            }
        break;
        default :
            printf("ERROR: Integer, identifier or opening parenthesis expected (ligne %u, column %u)\n",current_token().ligne,current_token().column);
            exit(1);
    }
}







void rec_relation(AstNode **A){
    AstNode *left = NULL;
    AstNode *right = NULL;
    TypeComparisonOperator op;

    // Parse left operand (arithmetic expression)
    rec_gae(&left);

    // If there's a comparison operator after it
    if(current_token().type == EQUAL || 
       current_token().type == NOT_EQUAL || 
       current_token().type == LESS || 
       current_token().type == GREATER || 
       current_token().type == LESS_EQUAL || 
       current_token().type == GREATER_EQUAL) {

        op = get_comparison_operator(current_token().type);
        advance();

        // Parse right operand (arithmetic expression)
        rec_gae(&right);

        *A = create_comparison_operation(op, left, right);
    }
    else {
        *A = left;
    }
}


/*void rec_relation(AstNode **A){
    AstNode *left = NULL;
    AstNode *right = NULL;
    TypeComparisonOperator op;

    // Parse left operand (arithmetic expression)
    rec_gae(&left);

    // Parse any number of comparison operators in sequence
    while (current_token().type == EQUAL ||
           current_token().type == NOT_EQUAL ||
           current_token().type == LESS ||
           current_token().type == GREATER ||
           current_token().type == LESS_EQUAL ||
           current_token().type == GREATER_EQUAL) {

        op = get_comparison_operator(current_token().type);
        advance();

        // Parse next operand (arithmetic expression)
        rec_gae(&right);

        // Chain the comparison operations
        left = create_comparison_operation(op, left, right);
    }

    *A = left;
}
*/



void rec_primary(AstNode **A) {
    AstNode *expr = NULL;
    if (current_token().type == NEGATION) {
        advance();
        rec_primary(&expr);
        *A = create_not_operation(expr);
    }
    else {
        rec_relation(A);
    }
}




void rec_bool_term(AstNode **A){
    AstNode *left = NULL;
    AstNode *right = NULL;

    rec_primary(&left);
    
    while(current_token().type == AND){
        advance();
        rec_primary(&right);

        left = create_and_or_operation(N_AND, left, right);
    }

    *A = left;
}

void rec_boolean_expression(AstNode **A){
    AstNode *left = NULL;
    AstNode *right = NULL;

    rec_bool_term(&left);

    while(current_token().type == OR){
        advance();
        rec_bool_term(&right);

        left = create_and_or_operation(N_OR, left, right);
    }

    *A = left;
}



/* void rec_seq_assignment(AstNode **A){
    AstNode *B;
    AstNode *rest;
    if(current_token().type == IDENTIFIER){
        rec_assignment(&B);
        rec_seq_assignment(&rest);
        *A = create_sequence_assignment(B,rest);
    } else {
        *A = NULL;
    }
} */ 



void rec_assignment(AstNode **A){

    AstNode *B = NULL; // the left child = the identifier
    AstNode *C = NULL; // the right child = the gae

    int value;
    char idf[MAX_IDF_LENGTH]; // name of identifier 

    if(current_token().type == IDENTIFIER){
        strcpy(idf,current_token().string);
        B = create_identifier(idf);
        advance();
    } else {
        printf("ERROR: Identifier expected (ligne %u, column %u)\n",current_token().ligne,current_token().column);
        exit(1);
    }

    if(current_token().type == ASSIGN){
        advance();
    } else {
        printf("ERROR: Assignment operator expected (ligne %u, column %u)\n",current_token().ligne,current_token().column);
        exit(1);
    }

    rec_boolean_expression(&C); // Contains the AST of the assigned expression
    
    if(C != NULL){
        value = bool_evaluation(C);
        insert_or_update(&table, idf, value);
    }

    if(current_token().type == SEMI){
        advance();
    } else {
        printf("ERROR: Semi column expected (ligne %u, column %u)\n",current_token().ligne,current_token().column);
        exit(1);
    }
    *A = create_assignment(B,C);
    
}


void rec_program(AstNode **A){
    rec_seq_inst(A);
}

/* This function needs more explication */
void rec_seq_inst(AstNode **A){
    AstNode *B = NULL;
    AstNode *C = NULL;
    switch(current_token().type){
        case IDENTIFIER:
            rec_assignment(&B);
        break;

        case IF:
            rec_if_part(&B);
        break;

        case WHILE:
            rec_while_loop(&B);
        break;

        default:
            B = NULL;
        break;
    }


    if(B != NULL){
        rec_seq_inst(&C);
        //printf("inst=%p, rest_of_inst=%p\n", (void*)B, (void*)C);
        *A = create_seq_inst(B,C);
        
    } else {
        *A = C;
    }

}

/*void rec_seq_inst_tail(AstNode **A){

    if(current_token().type == SEMI){
        advance();
        rec_seq_inst(A);
    } else {
        *A = NULL;
    }

}*/


void rec_if_part(AstNode **A){
    AstNode *B = NULL;
    AstNode *C = NULL;
    AstNode *D = NULL;
    
    if(current_token().type == IF){
        advance();
        if(current_token().type == LPAR){
            advance();
            rec_boolean_expression(&B);
            if(current_token().type == RPAR){
                advance();
                if(current_token().type == LCURL){
                    advance();
                    rec_seq_inst(&C);
                    if(current_token().type == RCURL){
                        advance();
                        rec_else_part(&D);
                        *A = create_condition(B,C,D);
                    } else {
                        printf("ERROR: expected '}' (ligne %u, column %u)\n",current_token().ligne,current_token().column);
                        exit(1);
                    }
                } else {
                    printf("ERROR: expected '{' (ligne %u, column %u)\n",current_token().ligne,current_token().column);
                    exit(1);
                }
            } else {
                printf("ERROR: expected ')' (ligne %u, column %u)\n",current_token().ligne,current_token().column);
                exit(1);
            }
        } else {
            printf("ERROR: expected '(' (ligne %u, column %u)\n",current_token().ligne,current_token().column);
            exit(1);
        }

    }
}

void rec_else_part(AstNode **A){
    if(current_token().type == ELSE){
        advance();
        if(current_token().type == LCURL){
            advance();
            rec_seq_inst(A);
            if(current_token().type == RCURL){
                advance();
            } else {
                printf("ERROR: expected '}' (ligne %u, column %u)\n",current_token().ligne,current_token().column);
                exit(1);
            }
        } else {
            printf("ERROR: expected '{' (ligne %u, column %u)\n",current_token().ligne,current_token().column);
            exit(1);
        }
    } else {
        *A = NULL;
    }
}


void rec_while_loop(AstNode **A){
    AstNode *B = NULL;
    AstNode *C = NULL;
    if(current_token().type == WHILE){
        advance();
        if(current_token().type == LPAR){
            advance();
            rec_boolean_expression(&B);
            if(current_token().type == RPAR){
                advance();
                if(current_token().type == LCURL){
                    advance();
                    rec_seq_inst(&C);
                    if(current_token().type == RCURL){
                        advance();
                        *A = create_while(B,C);
                    } else {
                        printf("ERROR: expected '}' (ligne %u, column %u)\n",current_token().ligne,current_token().column);
                        exit(1);
                    }
                } else {
                    printf("ERROR: expected '{' (ligne %u, column %u)\n",current_token().ligne,current_token().column);
                    exit(1);
                }
            } else {
                printf("ERROR: expected ')' (ligne %u, column %u)\n",current_token().ligne,current_token().column);
                exit(1);
            }
        } else {
            printf("ERROR: expected '(' (ligne %u, column %u)\n",current_token().ligne,current_token().column);
            exit(1);
        }
    }
}

int op_star_slash(TypeOperator *op){
    switch(current_token().type){
        case MUL:
        case DIV:
            *op = get_opperator(current_token().type);
            advance();
            return 1;
    
        default:
            return 0;
    }
}

int op_plus_minus(TypeOperator *op){
    switch(current_token().type){
        case PLUS:
        case MINUS:
            *op = get_opperator(current_token().type);
            advance();
            return 1;
        
        default:
            return 0;
    }
}



TypeOperator get_opperator(TokenType type){
    switch(type){
        case PLUS : 
            return N_PLUS;
        break;

        case MINUS :
            return N_MINUS;
        break;

        case MUL : 
            return N_MUL;
        break;

        case DIV : 
            return N_DIV;
        break;

        default :
            printf("OPERATOR ERROR\n");
            exit(1);
    }
}   


TypeComparisonOperator get_comparison_operator(TokenType type){
    switch(type){

        case EQUAL: return N_EQUAL; break;
        case NOT_EQUAL: return N_NOT_EQUAL; break;
        case LESS: return N_LESS; break;
        case GREATER: return N_GREATER; break;
        case LESS_EQUAL: return N_LESS_EQUAL; break;
        case GREATER_EQUAL: return N_GREATER_EQUAL; break;

        default: printf("OPERATOR ERROR\n"); exit(1);

    }
}
/************************************************/



