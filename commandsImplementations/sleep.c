#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int sleep_(char *argv[]){
    
    if(!argv[1]) printf("No Argument Given\n");
    sleep(atoi(argv[1]));

    return 0;
}