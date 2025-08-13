#include <stdio.h>
#include <string.h>

int grep(char *argv[])
{
    if (!argv[1])
    {
        fprintf(stderr, "grep: missing search pattern\n");
        return 1;
    }

    char *pattern = argv[1];


    if (!argv[2])
    {
        char line[1024];
        while (fgets(line, sizeof(line), stdin))
        {
            if (strstr(line, pattern) != NULL)
            {
                fputs(line, stdout);
            }
        }
        printf("%c\n", ' ');
        return 0;
    }

    for (int i = 2; argv[i]; i++)
    {
        FILE *fp = fopen(argv[i], "r");
        if (!fp)
        {
            perror(argv[i]);
            continue;
        }

        char line[1024];
        while (fgets(line, sizeof(line), fp))
        {
            if (strstr(line, pattern) != NULL)
            {
                fputs(line, stdout);
            }
        }
        printf("%c\n", ' ');

        fclose(fp);
    }

    return 0;
}
