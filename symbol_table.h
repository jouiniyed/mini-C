#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_IDF_LENGTH 64

// Structure for a symbol (identifier + value)

typedef struct Symbol {
    char idf[MAX_IDF_LENGTH];
    int value;
    int bool_value;
    struct Symbol *next;
} Symbol;

// Symbol table as a linked list 

typedef struct {
    Symbol *head;
} SymbolTable;

void init_table(SymbolTable *table);
void insert_or_update(SymbolTable *table, char *idf, int value);
int lookup(SymbolTable *table, char *idf, int *value, int *bool_value);
void print_table(SymbolTable *table);
void free_table(SymbolTable *table);

#endif