#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


#define MAXARGS 1024

enum Type{
    EXEC,
    REDIR,
    PIPE,
    CONSEC,
    BACK
};

struct cmd{
    enum Type type;
};

struct exec_cmd{
    enum Type type;
    char *argv[MAXARGS];
    char *eargv[MAXARGS];
};

struct pipe_cmd {
    enum Type type;
    struct cmd *left;
    struct cmd *right;
};

struct consec_cmd{
    enum Type type;
    struct cmd *left;
    struct cmd * right;
};

struct background_cmd{
    int type;
    struct cmd *command;
};

struct redirect_cmd{
    enum Type type;
    struct cmd *command;
    char file;
    char efile;
    int mode;
    int fd;
};



bool isSymbol(char* command, int index){
    return  command[index]=='>' 
            || command[index]=='<' 
            || command[index]=='&'
            || command[index]=='|'
            || command[index]==';';
}

#ifndef PARSE
#define PARSE

struct cmd* parse(char* command, int start, int end){
    int currChar = start;

    int consecFirstOccurence = -1;
    int backgroundFirstOccurence = -1;
    int pipeFirstOccurence = -1;
    int redirectFirstOccurence = -1;

    while(currChar<end){
        if(command[currChar]==';'){
            consecFirstOccurence=currChar;
            break;
        }else if(command[currChar]=='&' && backgroundFirstOccurence==-1){
            backgroundFirstOccurence=currChar;
        }else if(command[currChar]=='|' && pipeFirstOccurence==-1){
            pipeFirstOccurence=currChar;
        }else if((command[currChar]=='<' || command[currChar]=='>') && redirectFirstOccurence==-1){
            redirectFirstOccurence=currChar;
        }

        currChar++;
    }

    
    if(consecFirstOccurence!=-1){
        struct consec_cmd *res = malloc(sizeof(struct consec_cmd));

        res->type = CONSEC;
        res->left = parse(command, start, consecFirstOccurence-1);
        res->right = parse(command, consecFirstOccurence+1, end);

        return (struct cmd *) res;
    }else if(backgroundFirstOccurence!=-1){
        struct background_cmd *res = malloc(sizeof(struct background_cmd));

        res->type = BACK;
        res->command = parse(command, start, backgroundFirstOccurence-1);

        return (struct cmd *) res;
    }else if(pipeFirstOccurence!=-1){
        struct pipe_cmd *res = malloc(sizeof(struct pipe_cmd));

        res->type = PIPE;
        res->left = parse(command, start, pipeFirstOccurence-1);
        res->right = parse(command, pipeFirstOccurence+1, end);

        return (struct cmd *) res;
    }else if(redirectFirstOccurence!=-1){
        struct redirect_cmd *res = malloc(sizeof(struct redirect_cmd));

        int startOfFile;
        int startOfCommand = redirectFirstOccurence;
        int endOfFile = redirectFirstOccurence;
        bool stdin;
        int mode;

        if(command[redirectFirstOccurence+1]=='>'){
            startOfFile=redirectFirstOccurence+2;
            stdin=false;
            mode = 1;
        }else if(command[redirectFirstOccurence]=='>'){
            startOfFile=redirectFirstOccurence+1;
            stdin=false;
            mode = 2;
        }else{
            startOfFile=redirectFirstOccurence+1;
            stdin=true;   
            mode = 3;
        }

        while(!isSymbol(command, startOfCommand)) startOfCommand--;
        while(!isSymbol(command, endOfFile)) endOfFile++;

        res->type=REDIR;
        res->command = parse(command, startOfCommand, redirectFirstOccurence-1);
        res->file = startOfFile;
        res->efile = endOfFile;
        res->mode = mode;
        res->fd = stdin;

        return (struct cmd *) res;
    }else{

        struct exec_cmd *res = malloc(sizeof(struct exec_cmd));
        res->type = EXEC;
        bool found = true;
        int size = 0;

        while(command[start]==' ') start++;
        (res->argv)[0]=&command[start];
        size++;

        for(; start<end; start++){
            if(found && command[start]==' '){
                ((res->eargv))[size]=&command[start];
                found = false;
            }else if(!found && command[start]!=' '){
                ((res->argv))[size]=&command[start];
                size++;
                found = true;
            }
        }

        return (struct cmd *) res;
    }
}

#endif