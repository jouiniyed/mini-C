#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "read_characters.h"
#include "lexer.h"

#define KEY_WORD_SET_SIZE 3

const char* keywords[KEY_WORD_SET_SIZE] = {"if","else", "while"};
Token currentToken;
void add_char(char *s, char c);
CharacterType char_type(char c);
int is_separator(char c);
int is_digit(char c);
int is_symbol(char c);
int is_keyword(const char *token_string);
TokenType specify_keyword(Token t);
void recognize_token();

void start(char *file_name){
    start_char(file_name);
    advance();
}

void advance(){
    recognize_token();
}

Token current_token(){
    return currentToken;
}

int end_of_sequence(){
    return currentToken.type == END_OF_SEQUENCE;
}

void stop(){
    stop_char();
}

void recognize_token(){
    typedef enum {  INIT_STATE,
                    INTER_STATE_DIGIT,
                    INTER_STATE_ALPHA,
                    INTER_STATE_RELATIONAL_OPERATORS,
                    INTER_STATE_LOGICAL_OPERATORS,
                    FIN_STATE
    } Automaton_State;
    Automaton_State state = INIT_STATE;
    while(is_separator(current_char())){
        advance_char();
    }
    
    currentToken.string[0] = '\0';
    while(state != FIN_STATE){
        switch(state){
            case INIT_STATE:
                switch(char_type(current_char())){

                    case END_OF_SEQUENCE_CHAR:
                        currentToken.type = END_OF_SEQUENCE;
                        state = FIN_STATE;
                    break;

                    case DIGIT:
                        currentToken.type = INTEGER;
                        currentToken.ligne = current_ligne();
                        currentToken.column = current_column();
                        add_char(currentToken.string,current_char());
                        currentToken.value = current_char() - '0';
                        state = INTER_STATE_DIGIT;
                        advance_char();
                    break;

                    case ALPHA:
                        currentToken.type = IDENTIFIER;
                        currentToken.ligne = current_ligne();
                        currentToken.column = current_column();
                        add_char(currentToken.string,current_char());
                        state = INTER_STATE_ALPHA;
                        advance_char();
                    break;

                    case SYMBOL: 
                        currentToken.ligne = current_ligne();
                        currentToken.column = current_column();
                        add_char(currentToken.string,current_char());
                        switch(current_char()){
                            case '+':
                                currentToken.type = PLUS;
                                state = FIN_STATE;
                            break;

                            case '-':
                                currentToken.type = MINUS;
                                state = FIN_STATE;
                            break;

                            case '*':
                                currentToken.type = MUL;
                                state = FIN_STATE;
                            break;

                            case '/':
                                currentToken.type = DIV;
                                state = FIN_STATE;
                            break;

                            case '(':
                                currentToken.type = LPAR;
                                state = FIN_STATE;
                            break;

                            case ')':
                                currentToken.type = RPAR;
                                state = FIN_STATE;
                            break;

                            case '{':
                                currentToken.type = LCURL;
                                state = FIN_STATE;
                            break;
                            
                            case '}':
                                currentToken.type = RCURL;
                                state = FIN_STATE;
                            break;

                            case '=':
                                currentToken.type = ASSIGN;
                                state = INTER_STATE_RELATIONAL_OPERATORS;
                            break;

                            case ';': 
                                currentToken.type = SEMI;
                                state = FIN_STATE;
                            break;

                            case '<':
                                currentToken.type = LESS;
                                state = INTER_STATE_RELATIONAL_OPERATORS;
                            break;

                            case '>':
                                currentToken.type = GREATER;
                                state = INTER_STATE_RELATIONAL_OPERATORS;
                            break;

                            case '!':
                                currentToken.type = NEGATION;
                                state = INTER_STATE_RELATIONAL_OPERATORS;
                            break;

                            case '|':
                                currentToken.type = OR;
                                state = INTER_STATE_LOGICAL_OPERATORS;
                            break;

                            case '&':
                                currentToken.type = AND;
                                state = INTER_STATE_LOGICAL_OPERATORS;
                            break;

                            default:
                                printf("LEXICAL_ERROR (ligne %u, column %u)\n",currentToken.ligne,currentToken.column);
                                exit(0);

                        };

                        advance_char();
                    break;
                    
                    default :

                        printf("LEXICAL_ERROR (ligne %u, column %u)\n",currentToken.ligne,currentToken.column);
                        exit(0);
                    
                }
            break;

            
            case INTER_STATE_DIGIT:
                switch(char_type(current_char())){
                    case DIGIT:
                        add_char(currentToken.string,current_char());
                        currentToken.value = currentToken.value * 10 + (current_char() - '0');
                        state = INTER_STATE_DIGIT;
                        advance_char();
                    break;

                    default:
                        state = FIN_STATE;
                    
                }
            break;

            case INTER_STATE_ALPHA:
                switch(char_type(current_char())){
                    case DIGIT:
                        add_char(currentToken.string,current_char());
                        state = INTER_STATE_ALPHA;
                        advance_char();
                    break;

                    case ALPHA:
                        add_char(currentToken.string,current_char());
                        state = INTER_STATE_ALPHA;
                        advance_char();
                    break;

                    default:
                        if(is_keyword(currentToken.string) == 1){
                            currentToken.type = specify_keyword(currentToken);
                        }
                        state = FIN_STATE;
                }
            break;

            case INTER_STATE_RELATIONAL_OPERATORS:
                if(current_char() == '='){
                    switch(currentToken.type){
                        case ASSIGN:
                            add_char(currentToken.string,current_char());
                            currentToken.type = EQUAL;
                        break;

                        case LESS:
                            add_char(currentToken.string,current_char());
                            currentToken.type = LESS_EQUAL;
                        break;

                        case GREATER:
                            add_char(currentToken.string,current_char());
                            currentToken.type = GREATER_EQUAL;
                        break;
                            
                        case NEGATION:
                            add_char(currentToken.string,current_char());
                            currentToken.type = NOT_EQUAL;
                        break;

                        default:
                        break;
                    }
                    state = FIN_STATE;
                    advance_char();
                } else {
                    state = FIN_STATE;
                }
            break;

            case INTER_STATE_LOGICAL_OPERATORS:
                if((current_char() == '&' && currentToken.type == AND) || (current_char() == '|' && currentToken.type == OR)){
                    add_char(currentToken.string,current_char());
                } else {
                    printf("LEXICAL_ERROR (ligne %u, column %u)\n",currentToken.ligne,currentToken.column);
                    exit(0);
                }
                state = FIN_STATE;
                advance_char();
            break;
            
            case FIN_STATE:
            break;

            }
    } 
}




void add_char(char *s, char c){
    int l ;
	l = strlen(s) ;
	s[l] = c ;
	s[l+1] = '\0' ;
}

CharacterType char_type(char c){
    if(is_digit(c)) return DIGIT;
    if(is_symbol(c)) return SYMBOL;
    if(isalpha(c)) return ALPHA;
    if(end_of_char_sequence(c)) return END_OF_SEQUENCE_CHAR;
    return ERROR_CHAR;
}

int is_separator(char c){
    return c == ' ' || c == '\t' || c == '\n' ;
}

int is_digit(char c){
    return c >= '0' && c <= '9' ;
}

int is_symbol(char c){
    switch (c) {
        case '+':  
	 	case '-':  
	 	case '*':
	 	case '/':
	 	case '(':
	 	case ')':
        case '{':
        case '}':
        case '=':
        case ';':
        case '<':
        case '>':
        case '!':
        case '|':
        case '&':
            return 1;

        default:
            return 0;
    } 
}

int is_keyword(const char *token_string){
    for(int i = 0; i < KEY_WORD_SET_SIZE; i++){
        if(strcmp(token_string, keywords[i]) == 0){
            return 1;
        }
    }

    return 0;
}

TokenType specify_keyword(Token t){
    if(strcmp(t.string,"if") == 0){
        return IF;
    } else if(strcmp(t.string,"else") == 0){
        return ELSE;
    } else if(strcmp(t.string, "while") == 0){
        return WHILE;
    } else {
        printf("ERROR: Can't specify keyword\n");
        exit(1);
    }
        
}

char *Type_to_string(TokenType type){
    switch(type){
        case INTEGER: return "INTEGER";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case MUL: return "MUL";
        case DIV: return "DIV";
        case LPAR: return "LPAR";
        case RPAR: return "RPAR";
        case LCURL: return "LCURL";
        case RCURL: return "RCURL";
        case ASSIGN: return "ASSIGN";
        case SEMI: return "SEMI";
        case IDENTIFIER: return "IDENTIFIER";
        case IF: return "IF";
        case ELSE: return "ELSE";
        case WHILE: return "WHILE";
        case EQUAL: return "EQUAL";
        case NOT_EQUAL: return "NOT EQUAL";
        case LESS: return "LESS";
        case GREATER: return "GREATER";
        case LESS_EQUAL: return "LESS_OR_EQUAL";
        case GREATER_EQUAL: return "GREATER_OR_EQUAL";
        case NEGATION: return "NEGATION";
        case AND: return "AND";
        case OR: return "OR";
        case END_OF_SEQUENCE: return "END OF SEQUENCE";
        default: return "ERROR";
    }
}




void display(Token t){
    switch (t.type) {
         case END_OF_SEQUENCE: 
            break;
         default: 
            printf("(ligne %d, ", t.ligne);
            printf("column %d) : ",t.column);
	    printf("[") ;
            printf("type : %s", Type_to_string(t.type)) ;
            printf(", lexem : %s, ", t.string) ;
            switch(t.type) {
                 case INTEGER:
                      printf("value : %d", t.value);
                 default:
                      break;
            } ;
	    printf("]") ;
	} ;
}
