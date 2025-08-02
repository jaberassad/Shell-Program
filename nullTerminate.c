#include "parse.c"


//travels the tree recrusively until reaching execute nodes
//then modifying the character at each address in eargv with a null terminator
void nullTerminate(struct cmd* node){

    switch(node->type){
        case CONSEC:
            struct consec_cmd* consecNode = (struct consec_cmd*) node;
            nullTerminate(consecNode->right);
            nullTerminate(consecNode->left);

        case PIPE:
            struct pipe_cmd* pipeNode = (struct pipe_cmd*) node;
            nullTerminate(pipeNode->right);
            nullTerminate(pipeNode->left);

        case BACK:
            struct background_cmd* backGroundNode = (struct background_cmd*) node;
            nullTerminate(backGroundNode->command);
        case REDIR:
            struct redirect_cmd* redirectNode = (struct redirect_cmd*) node;
            nullTerminate(redirectNode->command);

        case EXEC:
            struct exec_cmd* execNode = (struct exec_cmd*) node;

            for(int i=0; i<execNode->size; i++){
                *(execNode->eargv[i])='\0';
            }

            return true;
    }

}