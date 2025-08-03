#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// structures
#define MAXARGS 1024

#ifndef PARSE
#define PARSE

enum Type
{
    EXEC,
    REDIR,
    PIPE,
    CONSEC,
    BACK
};

struct cmd
{
    enum Type type;
};

struct exec_cmd
{
    enum Type type;
    char *argv[MAXARGS];
    char *eargv[MAXARGS];
    int size;
};

struct pipe_cmd
{
    enum Type type;
    struct cmd *left;
    struct cmd *right;
};

struct consec_cmd
{
    enum Type type;
    struct cmd *left;
    struct cmd *right;
};

struct background_cmd
{
    int type;
    struct cmd *command;
};

struct redirect_cmd
{
    enum Type type;
    struct cmd *command;
    char *file;
    char *efile;
    int mode;
    int fd;
};

// constructors

struct consec_cmd *consecCtor(struct cmd *left, struct cmd *right)
{
    struct consec_cmd *node = malloc(sizeof(struct consec_cmd));
    node->type = CONSEC;
    node->left = left;
    node->right = right;

    return node;
}

struct pipe_cmd *pipeCtor(struct cmd *left, struct cmd *right)
{
    struct pipe_cmd *node = malloc(sizeof(struct pipe_cmd));
    node->type = PIPE;
    node->left = left;
    node->right = right;

    return node;
}

struct background_cmd *backgroundCtor(struct cmd *command)
{
    struct background_cmd *node = malloc(sizeof(struct background_cmd));
    node->type = BACK;
    node->command = command;

    return node;
}

struct exec_cmd *execDefaultCtor()
{
    struct exec_cmd *node = malloc(sizeof(struct exec_cmd));
    node->type = EXEC;
    node->size = 0;

    return node;
}

struct exec_cmd *execCtor(char *commandString, int size)
{
    struct exec_cmd *node = malloc(sizeof(struct exec_cmd));
    node->type = EXEC;
    node->size = 0;
    bool found = true;

    int start = 0;

    while (commandString[start] == ' ')
        start++;

    node->argv[0] = &commandString[start];
    node->size++;

    for (; start < size; start++)
    {
        if (found && commandString[start] == ' ')
        {
            (node->eargv)[node->size - 1] = &commandString[start];
            found = false;
        }
        else if (!found && commandString[start] != ' ')
        {
            (node->argv)[node->size] = &commandString[start];
            node->size++;
            found = true;
        }
    }

    if (!node->eargv[node->size - 1])
        node->eargv[node->size - 1] = &commandString[size];

    return node;
}

struct redirect_cmd *redirectCtor(struct cmd *command, char *file, char *efile, int mode, int fd)
{
    struct redirect_cmd *node = malloc(sizeof(struct redirect_cmd));
    node->type = REDIR;
    node->command = command;
    node->file = file;
    node->efile = efile;
    node->mode = mode;
    node->fd = fd;

    return node;
}

// verfies if the current character is one of the operators
bool isOperator(char *command, int index)
{
    return command[index] == '>' || command[index] == '<' || command[index] == '&' || command[index] == '|' || command[index] == ';';
}

// parses the command into a tree
struct cmd *parse(char *commandString, int start, int end)
{
    int currChar = start;

    int consecFirstOccurence = -1;
    int backgroundFirstOccurence = -1;
    int pipeFirstOccurence = -1;
    int redirectFirstOccurence = -1;

    // iterates in the buffer from int start to int end
    // keeps track of the first occurence of each operator
    while (currChar < end)
    {
        if (commandString[currChar] == ';')
        {
            consecFirstOccurence = currChar;
            break;
        }
        else if (commandString[currChar] == '&' && backgroundFirstOccurence == -1)
        {
            backgroundFirstOccurence = currChar;
        }
        else if (commandString[currChar] == '|' && pipeFirstOccurence == -1)
        {
            pipeFirstOccurence = currChar;
        }
        else if ((commandString[currChar] == '<' || commandString[currChar] == '>') && redirectFirstOccurence == -1)
        {
            redirectFirstOccurence = currChar;
        }

        currChar++;
    }

    // Build the parse tree by checking for operators in order of precedence:
    // Lowest to highest: ';' → '&' → '|' → '<'/'>' → plain exec
    // At the first operator found, we split the command and recursively build subtrees.
    if (consecFirstOccurence != -1)
    {
        printf("consec\n");
        printf("%d\n", start);
        printf("%d\n", end);
        printf("%c\n", ' ');

        return (struct cmd *)consecCtor(parse(commandString, start, consecFirstOccurence - 1),
                                        parse(commandString, consecFirstOccurence + 1, end));
    }
    else if (backgroundFirstOccurence != -1)
    {
        // printf("background\n");
        // printf("%d\n", start);
        // printf("%d\n", end);
        // printf("%c\n", ' ');

        int startOfCommand = backgroundFirstOccurence-1;

        while (!isOperator(commandString, startOfCommand) && startOfCommand>0)
            startOfCommand--;

        if(isOperator(commandString, startOfCommand)) startOfCommand++;

        return (struct cmd *)backgroundCtor(parse(commandString, startOfCommand, backgroundFirstOccurence-1));
    }
    else if (pipeFirstOccurence != -1)
    {
        // printf("pipe\n");
        // printf("%d\n", start);
        // printf("%d\n", end);
        // printf("%c\n", ' ');
        return (struct cmd *)pipeCtor(parse(commandString, start, pipeFirstOccurence - 1),
                                      parse(commandString, pipeFirstOccurence + 1, end));
    }
    else if (redirectFirstOccurence != -1)
    {
        printf("redirect\n");
        printf("%d\n", start);
        printf("%d\n", end);
        printf("%c\n", ' ');

        int startOfFile;
        int startOfCommand = redirectFirstOccurence-1;
        int endOfFile = redirectFirstOccurence;
        bool input;
        int mode;

        if (commandString[redirectFirstOccurence + 1] == '>')
        {
            startOfFile = redirectFirstOccurence + 2;
            input = false;
            mode = 1;
        }
        else if (commandString[redirectFirstOccurence] == '>')
        {
            startOfFile = redirectFirstOccurence + 1;
            input = false;
            mode = 2;
        }
        else
        {
            startOfFile = redirectFirstOccurence + 1;
            input = true;
            mode = 3;
        }

        while (!isOperator(commandString, startOfCommand) && startOfCommand>0)
            startOfCommand--;
        while (!isOperator(commandString, endOfFile))
            endOfFile++;

        if(isOperator(commandString, startOfCommand)) startOfCommand++;

        int fd = 1;

        if (input)
            fd = 0;

        struct cmd *command = parse(commandString, startOfCommand, redirectFirstOccurence - 1);
        return (struct cmd *)redirectCtor(command, &commandString[startOfFile], &commandString[endOfFile], mode, fd);
    }
    else
    {
        printf("exec\n");
        printf("%d\n", start);
        printf("%d\n", end);
        printf("%c\n", ' ');
        struct exec_cmd *execNode = execDefaultCtor();
        bool found = true;

        while (commandString[start] == ' ')
            start++;

        execNode->argv[0] = &commandString[start];
        execNode->size++;

        for (; start < end; start++)
        {
            if (found && (commandString[start] == ' ' || isOperator(commandString, start)))
            {
                found = false;
                ((execNode->eargv))[execNode->size - 1] = &commandString[start];
            }
            else if (!found && commandString[start] != ' ')
            {
                ((execNode->argv))[execNode->size] = &commandString[start];
                found = true;
                execNode->size++;
            }
        }

        if (!execNode->eargv[execNode->size - 1])
            execNode->eargv[execNode->size - 1] = &commandString[end];
        return (struct cmd *)execNode;
    }
}

#endif