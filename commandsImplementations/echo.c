#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int echo(char *strings[])
{    
    for (int i = 1; strings[i]; i++)
        printf("%s ", strings[i]);

    printf("%c\n", ' ');
    return 0;
}