#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int sleep_(char *argv[]){
    
    if(!argv[1]) fprintf(stderr, "No Arguments given\n");
    sleep(atoi(argv[1]));

    return 0;
}