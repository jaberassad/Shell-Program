bool nullTerminate(struct cmd *node)
{
    switch (node->type)
    {
    case CONSEC:
    {
        struct consec_cmd *consecNode = (struct consec_cmd *)node;
        nullTerminate(consecNode->right);
        nullTerminate(consecNode->left);
        break;
    }

    case PIPE:
    {
        struct pipe_cmd *pipeNode = (struct pipe_cmd *)node;
        nullTerminate(pipeNode->right);
        nullTerminate(pipeNode->left);
        break;
    }

    case BACK:
    {
        struct background_cmd *backGroundNode = (struct background_cmd *)node;
        nullTerminate(backGroundNode->command);
        break;
    }

    case REDIR:
    {
        struct redirect_cmd *redirectNode = (struct redirect_cmd *)node;
        *(redirectNode->efile) = '\0';
        nullTerminate(redirectNode->command);
        break;
    }

    case EXEC:
    {
        struct exec_cmd *execNode = (struct exec_cmd *)node;

        printf("%d\n", execNode->size);

        for (int i = 0; i < execNode->size; i++)
        {
            if (execNode->eargv[i] == NULL)
                continue;
            *(execNode->eargv[i]) = '\0';
            printf("%s\n", execNode->argv[i]);
        }

        break;
    }
    }

    return true;
}
