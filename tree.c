#define MAXARGS 10

enum Type{
    EXEC,
    REDIR,
    PIPE,
    LIST,
    BACK
};

struct cmd{
    enum Type type;
};

struct exec_cmd{
    enum Type type;
    int *argv[MAXARGS];
    int *eargv[MAXARGS];
}

struct pipe_cmd{
    enum Type type;
    struct cmd *left;
    struct cmd *right;
}

struct consec_cmd{
    enum Type type;
    struct cmd *left;
    struct cmd * right;
}

struct background_cmd{
    int type;
    struct cmd *command;
}

struct redirect_cmd{
    enum Type type;
    struct cmd *command;
    char file;
    char efile;
    int mode;
    int fd;
}

string[] tokenize(string s)
