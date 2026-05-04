#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symbol_table.h"

void init_table(SymbolTable *table){
    table->head = NULL;
}


void insert_or_update(SymbolTable *table, char *idf, int value){
    Symbol *current = table->head;
    while(current){
        if(strcmp(current->idf,idf) == 0){
            current->value = value; // Update existing 
            current->bool_value = (value != 0);
            return;
        }
        current = current->next;
    }

    // Insert new symbol

    Symbol *new = malloc(sizeof(Symbol));

    assert(new && "Memory allocation failed\n");

    strncpy(new->idf, idf, MAX_IDF_LENGTH);
    new->idf[MAX_IDF_LENGTH - 1] = '\0';
    new->value = value;
    new->bool_value = (value != 0);
    new->next = table->head;
    table->head = new;

}


int lookup(SymbolTable *table, char *idf, int *value, int *bool_value){
    if(!table || !idf) return 0;

    Symbol *current = table->head;
    while(current){
        if(strcmp(current->idf,idf) == 0){
            if(value) *value = current->value;
            if(bool_value) *bool_value = (current->value != 0);
            return 1; // FOUND
        }
        current = current->next;
    }

    return 0; // NOT FOUND
}

void print_table(SymbolTable *table){
    Symbol *current = table->head;
    printf("Symbol table:\n");
    while(current){
        printf("    %s = %d (bool: %d)\n", current->idf, current->value, current->bool_value);
        current = current->next;
    }
}

void free_table(SymbolTable *table){
    Symbol *current = table->head;
    
    while(current){
        Symbol *next = current->next;
        free(current);
        current = next;
    }

    table->head = NULL;
}