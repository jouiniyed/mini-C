#include <stdio.h>
#include "parser.h"
#include <stdlib.h>





int main() {
    
    char* filename = "inputbis.txt";
    /*start(filename);
    while(current_token().type != END_OF_SEQUENCE){
        display(current_token());
        printf("\n");
        advance();
    }
    stop();*/

    AstNode *node = NULL;
    parse(filename, &node);
    print_ast(node);
    printf("\n");   
    /* int result = bool_evaluation(node);
    printf("result = %d\n",result);*/
    /*int result = evaluation(node);
    printf("the result is %d\n", result);
    return 0;*/
}
