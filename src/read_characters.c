#include <stdio.h>
#include <string.h>

#include "read_characters.h"

char currentCharacter;
unsigned int currentLigne, currentColumn;
FILE *file;

void advance_char(){
    if(!feof(file)){
        currentCharacter = fgetc(file);
        if(currentCharacter != '\n'){
            currentColumn = currentColumn + 1;
        } else {
            currentColumn = 1;
            currentLigne = currentLigne +1;
        }
    } else {
        currentCharacter = EOF;
    }
}

void start_char(char *file_name){
    if(strlen(file_name) == 0){
        file = stdin;
    } else {
        file = fopen(file_name,"r");
    }
    currentLigne = 1; currentColumn = 0;
    advance_char();
}

char current_char(){
    return currentCharacter;
}

int end_of_char_sequence(){
    return currentCharacter == EOF;
}

unsigned int current_ligne(){
    return currentLigne;
}

unsigned int current_column(){
    return currentColumn;
}

void stop_char(){
    fclose(file);
}