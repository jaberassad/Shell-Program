#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char *string[])
{

    for (int i = 1; string[i]; i++)
        printf("%s ", string[i]);
    printf("%c\n", ' ');

    return 0;
}