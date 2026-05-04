#ifndef LEXER_HH
#define LEXER_HH

typedef enum {
    ERROR_CHAR,
    DIGIT,
    SYMBOL,
    ALPHA,
    END_OF_SEQUENCE_CHAR //EOF
} CharacterType;

typedef enum {
    INTEGER,
    PLUS,
    MINUS,
    MUL,
    DIV,
    LPAR,
    RPAR,
    LCURL, // Recently added 
    RCURL, // Recently added 
    ASSIGN, 
    SEMI, 
    IDENTIFIER, 
    IF,
    ELSE,
    WHILE,
    EQUAL, // Recently added 
    NOT_EQUAL, // Recently added
    LESS, // Recently added 
    GREATER, // Recently added
    LESS_EQUAL, // Recently added
    GREATER_EQUAL, // Recently added
    NEGATION,
    AND,
    OR,
    END_OF_SEQUENCE,
    ERROR
} TokenType;

typedef struct {
    TokenType type;
    unsigned int ligne;
    unsigned int column;
    char string[256];
    int value;
} Token;

void display(Token t);

void start(char *file_name);

void advance();

Token current_token();

int end_of_sequence();

void stop();

char *Type_to_string(TokenType type);

#endif // LEXER_HH
