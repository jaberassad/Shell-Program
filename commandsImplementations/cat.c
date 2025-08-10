#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "../cmd_utils.c"

#define MAXARGS 1024

void helper(char *fileName)
{

    int fileDescriptor = open(fileName, O_RDONLY);
    if (fileDescriptor < 0)
    {
        fprintf(stderr, "cat: can't open %s: %s\n", fileName, strerror(errno));
        return;
    }

    char buffer[MAXARGS];
    ssize_t bytesRead;

    while ((bytesRead = read(fileDescriptor, buffer, MAXARGS)) > 0)
    {
        write(1, buffer, bytesRead);
    }

    if (bytesRead == -1)
    {
        perror("read");
    }

    printf("%c\n", ' ');

    close(fileDescriptor);
}

int cat(char *argv[])
{
    if (!argv[1])
    {
        printf("%s\n", readStdin());
    }

    for (int i = 1; argv[i]; i++)
    {
        helper(argv[i]);
        printf("%c\n", ' ');
    }
    return 0;
}
