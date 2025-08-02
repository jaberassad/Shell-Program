#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


//structures
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
    int size;
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
    char *file;
    char *efile;
    int mode;
    int fd;
};

//constructors

struct consec_cmd *consecCtor(struct cmd* left, struct cmd* right){
    struct consec_cmd *node = malloc(sizeof(struct consec_cmd));
    node->type = CONSEC;
    node->left = left;
    node->right = right;

    return node;
}

struct pipe_cmd *pipeCtor(struct cmd* left, struct cmd* right){
    struct pipe_cmd *node = malloc(sizeof(struct pipe_cmd));
    node->type = PIPE;
    node->left = left;
    node->right = right;

    return node;
}

struct background_cmd *backgroundCtor(struct cmd* command){
    struct background_cmd *node = malloc(sizeof(struct background_cmd));
    node->type = BACK;
    node->command = command;

    return node;
}

struct exec_cmd *execCtor(){
        struct exec_cmd *node = malloc(sizeof(struct exec_cmd));
        node->type = EXEC;
        node->size = 0;

        return node;
}

struct redirect_cmd *redirectCtor(struct cmd *command, char *file, char *efile, int mode, int fd){
        struct redirect_cmd *node = malloc(sizeof(struct redirect_cmd));

        node->command = command;
        node->file = file;
        node->efile = efile;
        node->mode = mode;
        node->fd = fd;

        return node;
}


//verfies if the current character is one of the operators
bool isOperator(char* command, int index){
    return  command[index]=='>' 
            || command[index]=='<' 
            || command[index]=='&'
            || command[index]=='|'
            || command[index]==';';
}

#ifndef PARSE
#define PARSE


//parses the command into a tree
struct cmd* parse(char* command, int start, int end){
    int currChar = start;

    int consecFirstOccurence = -1;
    int backgroundFirstOccurence = -1;
    int pipeFirstOccurence = -1;
    int redirectFirstOccurence = -1;

    //iterates in the buffer from int start to int end
    //keeps track of the first occurence of each operator
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

    // Build the parse tree by checking for operators in order of precedence:
    // Lowest to highest: ';' → '&' → '|' → '<'/'>' → plain exec
    // At the first operator found, we split the command and recursively build subtrees.
    if(consecFirstOccurence!=-1){

        return (struct cmd *) consecCtor(parse(command, start, consecFirstOccurence-1), 
                                        parse(command, consecFirstOccurence+1, end));

    }else if(backgroundFirstOccurence!=-1){

        return (struct cmd *) backgroundCtor(malloc(sizeof(struct background_cmd)));

    }else if(pipeFirstOccurence!=-1){

        return (struct cmd *) pipeCtor(parse(command, start, pipeFirstOccurence-1), 
                                        parse(command, pipeFirstOccurence+1, end));

    }else if(redirectFirstOccurence!=-1){

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

        while(!isOperator(command, startOfCommand)) startOfCommand--;
        while(!isOperator(command, endOfFile)) endOfFile++;

        return (struct cmd *) redirectCtor(command, startOfFile, endOfFile, mode, stdin);

    }else{
        struct exec_cmd *res = execCtor();
        bool found = true;

        while(command[start]==' ') start++;
        (res->argv)[0]=&command[start];
        res->size++;

        for(; start<end; start++){
            if(found && command[start]==' '){
                ((res->eargv))[res->size]=&command[start];
                found = false;
            }else if(!found && command[start]!=' '){
                ((res->argv))[res->size]=&command[start];
                res->size++;
                found = true;
            }
        }

        return (struct cmd *) res;
    }
}

#endif