#include <stdio.h>


int main(void){

    char line[1024];

    while(1){
        printf("myshell> ");
        if(fgets(line, 1024, stdin)==NULL){
            break;
        }

        
    }

    return 0;
}