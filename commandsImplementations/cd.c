#include <stdio.h>
#include <unistd.h>

int cd_(char *argv[]) {
    if (!argv[1]) {
        fprintf(stderr, "cd: missing operand\n");
        return 1;
    }

    if (chdir(argv[1]) != 0) {
        perror("cd");
        return 1;
    }

    char cw[1024];

    getcwd(cw, 1023);

    return 0;
}
