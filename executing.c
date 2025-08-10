#include "parse.c"
#include <unistd.h>
#include <fcntl.h>
#include "commandsImplementations/echo.c"
#include "commandsImplementations/cat.c"
#include "commandsImplementations/ls.c"
#include "commandsImplementations/sleep.c"

void exec(char *argv[])
{
    char *cmdName = argv[0];

    if (strcmp(cmdName, "echo")==0)
        echo(argv);
    else if (strcmp(cmdName, "cat")==0)
        cat(argv);
    else if (strcmp(cmdName, "ls")==0)
        ls(argv);
    else if (strcmp(cmdName, "sleep")==0)
        sleep_(argv);
}

void executeCmd(struct cmd *node)
{

    switch (node->type)
    {
    case CONSEC:
    {
        struct consec_cmd *consecNode = (struct consec_cmd *)node;
        executeCmd(consecNode->left);
        executeCmd(consecNode->right);
        break;
    }
    case PIPE:
    {
        struct pipe_cmd *pipeNode = (struct pipe_cmd *)node;
        int pipefd[2];

        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            exit(1);
        }

        pid_t pid1 = fork();
        if (pid1 == -1)
        {
            perror("fork");
            exit(1);
        }

        if (pid1 == 0)
        {
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);
            executeCmd(pipeNode->left);
            exit(0);
        }

        pid_t pid2 = fork();

        if (pid2 == -1)
        {
            perror("fork");
            exit(1);
        }

        if (pid2 == 0)
        {
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);
            executeCmd(pipeNode->right);
            exit(0);
        }

        close(pipefd[0]);
        close(pipefd[1]);

        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);

        break;
    }
    case BACK:
    {
        struct background_cmd *backgroundNode = (struct background_cmd *)node;

        pid_t pid = fork();

        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }
        else if (pid == 0)
        {
            executeCmd(backgroundNode->command);
            exit(0);
        }
        else
        {
            printf("[Background pid %d]\n", pid);
        }

        break;
    }
    case REDIR:
    {
        struct redirect_cmd *redirectNode = (struct redirect_cmd *)node;

        pid_t pid = fork();

        while (*(redirectNode->file) == ' ')
            redirectNode->file++;

        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }
        else if (pid == 0)
        {
            int fileDescriptor;
            if (redirectNode->fd == 1)
            {
                if (redirectNode->mode == 1)
                {
                    fileDescriptor = open(redirectNode->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                }
                else
                {
                    fileDescriptor = open(redirectNode->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                }
                dup2(fileDescriptor, STDOUT_FILENO);
            }
            else
            {
                printf("%s\n", ((struct exec_cmd *)redirectNode->command)->argv[0]);
                fileDescriptor = open(redirectNode->file, O_RDONLY);
                dup2(fileDescriptor, STDIN_FILENO);
            }

            close(fileDescriptor);
            executeCmd(redirectNode->command);
            exit(0);
        }
        break;
    }
    case BLOCK:
    {
        struct block_cmd *blockNode = (struct block_cmd *)node;
        executeCmd(blockNode->command);

        break;
    }
    case EXEC:
    {
        struct exec_cmd *execNode = (struct exec_cmd *)node;

        pid_t pid = fork();

        if (pid == 0)
        {
            exec(execNode->argv);
            exit(0);
        }

        wait(&pid);
    }
    }
}