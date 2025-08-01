#include <stdio.h>
#include "parse.c"
#include <string.h>


int main(void){

    char line[1024];

    while(1){
        printf("myshell> ");
        if(fgets(line, 1024, stdin)==NULL){
            break;
        }
        parse(line, 0, strlen(line));
    }

    return 0;
}