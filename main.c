#include <stdio.h>
#include "parse.c"
#include <string.h>
#include "nullTerminate.c"

int main(void){

    char line[1024];

    while(1){
        printf("myshell> ");
        if(fgets(line, 1024, stdin)==NULL){
            break;
        }
        struct cmd* command = parse(line, 0, strlen(line));
        nullTerminate(command);
    }

    return 0;
}