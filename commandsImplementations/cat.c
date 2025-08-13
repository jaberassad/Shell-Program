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

    char path[1024];
    int ret = fcntl(STDIN_FILENO, F_GETPATH, path);
    if (ret != -1) {
        printf("stdin points to file: %s\n", path);
    } else {
        perror("F_GETPATH");
    }

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

    printf("%c\n", ' ');

    if (bytesRead == -1)
    {
        perror("read");
    }

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
    }
    
    return 0;
}
