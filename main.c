#include <stdio.h>
#include "parse.c"
#include <string.h>
#include "nullTerminate.c"
#include "executing.c"
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>


int main(void){
    char line[1024];
    
    fflush(stdout);
    
    char prompt[] = "myshell>";
    
    while(1){
        write(2, prompt, strlen(prompt));
        if(fgets(line, 1024, stdin)==NULL){
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        struct cmd* command = parse(line, 0, strlen(line));
        nullTerminate(command);
        executeCmd(command);
    }
    
    return 0;
}