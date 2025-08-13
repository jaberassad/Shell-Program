#include "parse.c"
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#ifndef CMD_UTILS
#define CMD_UTILS 

// will be used in tests to see if the returned node is equal to the expected
bool equalNodes(struct cmd *node1, struct cmd *node2)
{
    if (node1->type != node2->type)
        return false;

    switch (node1->type)
    {
    case CONSEC:
    {
        struct consec_cmd *consecNode1 = (struct consec_cmd *)node1;
        struct consec_cmd *consecNode2 = (struct consec_cmd *)node2;

        return equalNodes(consecNode1->left, consecNode2->left) && equalNodes(consecNode1->right, consecNode2->right);
    }

    case PIPE:
    {
        struct pipe_cmd *pipeNode1 = (struct pipe_cmd *)node1;
        struct pipe_cmd *pipeNode2 = (struct pipe_cmd *)node2;
        return equalNodes(pipeNode1->left, pipeNode2->left) && equalNodes(pipeNode1->right, pipeNode2->right);
    }

    case BACK:
    {
        struct background_cmd *backGroundNode1 = (struct background_cmd *)node1;
        struct background_cmd *backGroundNode2 = (struct background_cmd *)node2;
        return equalNodes(backGroundNode1->command, backGroundNode2->command);
    }

    case REDIR:
    {
        struct redirect_cmd *redirectNode1 = (struct redirect_cmd *)node1;
        struct redirect_cmd *redirectNode2 = (struct redirect_cmd *)node2;
        return equalNodes(redirectNode1->command, redirectNode2->command) && strcmp(redirectNode1->file, redirectNode2->file) == 0 && redirectNode1->mode == redirectNode2->mode && redirectNode1->fd == redirectNode2->fd;
    }

    case BLOCK:{
        struct block_cmd * blockNode1 = (struct block_cmd *)node1;
        struct block_cmd * blockNode2 = (struct block_cmd *)node2;      
        
        return equalNodes(blockNode1->command, blockNode2->command);
    }

    case EXEC:
    {
        struct exec_cmd *execNode1 = (struct exec_cmd *)node1;
        struct exec_cmd *execNode2 = (struct exec_cmd *)node2;

        if (execNode1->size != execNode2->size)
            return false;

        int size = execNode1->size;

        for (int i = 0; i < size; i++)
        {
            if (strcmp(execNode1->argv[i], execNode2->argv[i]) != 0)
                return false;
        }

        return true;
    }
    }
}

void freeCmd(struct cmd *c)
{
    if (!c)
        return;


    switch (c->type)
    {
    case EXEC:
    {
        
        struct exec_cmd *exec = (struct exec_cmd *)c;
        free(exec);
        break;
    }
    case PIPE:
    {
        struct pipe_cmd *pipe = (struct pipe_cmd *)c;
        freeCmd(pipe->left);
        freeCmd(pipe->right);
        free(pipe);
        break;
    }
    case CONSEC:
    {
        struct consec_cmd *consec = (struct consec_cmd *)c;
        freeCmd(consec->left);
        freeCmd(consec->right);
        free(consec);
        break;
    }
    case BACK:
    {
        struct background_cmd *bg = (struct background_cmd *)c;
        freeCmd(bg->command);
        free(bg);
        break;
    }
    case BLOCK:{
        struct block_cmd *block = (struct block_cmd *)c;
        freeCmd(block->command);
        free(block);
        break;
    }
    case REDIR:
    {
        struct redirect_cmd *redir = (struct redirect_cmd *)c;
        freeCmd(redir->command);
        free(redir);
        break;
    }
    }
}

char *readStdin() {
    static char buf[1024];
    ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));

    int len = strlen(buf);
    while (len > 0 && isspace((unsigned char)buf[len - 1])) {
        buf[len - 1] = '\0';
        len--;
    }    

    return buf;
}


bool checkSyntax(const char *cmd) {
    int len = strlen(cmd);
    int parenDepth = 0;
    bool lastWasOp = true;
    bool lastWasParenOpen = false;

    for (int i = 0; i < len; i++) {
        char c = cmd[i];

        if (isspace(c)) continue;

        if (c == '(') {
            if (!lastWasOp) {
                fprintf(stderr, "syntax error: unexpected '('\n");
                return false;
            }
            parenDepth++;
            lastWasOp = true;
            lastWasParenOpen = true;
        }
        else if (c == ')') {
            if (parenDepth == 0) {
                fprintf(stderr, "syntax error: unexpected ')'\n");
                return false;
            }
            if (lastWasOp) {
                fprintf(stderr, "syntax error: empty command before ')'\n");
                return false;
            }
            parenDepth--;
            lastWasOp = false;
            lastWasParenOpen = false;
        }
        else if (c == '|' || c == ';' || c == '&') {
            if (lastWasOp) {
                fprintf(stderr, "syntax error: unexpected '%c'\n", c);
                return false;
            }
            lastWasOp = true;
        }
        else if (c == '<' || c == '>') {
            if (lastWasOp) {
                fprintf(stderr, "syntax error: unexpected redirection '%c'\n", c);
                return false;
            }
            lastWasOp = true;
        }
        else {
            lastWasOp = false;
            lastWasParenOpen = false;
            while (i + 1 < len && !isspace(cmd[i+1]) &&
                   cmd[i+1] != '(' && cmd[i+1] != ')' &&
                   cmd[i+1] != '|' && cmd[i+1] != ';' &&
                   cmd[i+1] != '&' && cmd[i+1] != '<' && cmd[i+1] != '>') {
                i++;
            }
        }
    }

    if (parenDepth > 0) {
        fprintf(stderr, "syntax error: '(' not closed\n");
        return false;
    }
    if (lastWasOp) {
        fprintf(stderr, "syntax error: command ends with operator\n");
        return false;
    }

    return true;
}


#endif