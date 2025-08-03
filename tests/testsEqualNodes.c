#include "../parse.c"
#include <assert.h>
#include <string.h>
#include "../nullTerminate.c"
#include "../cmd_utils.c"

// TESTS FOR EQUAL NODES()

// Exec Nodes

void testEqualNodesTwoExecsWithSameCommandNoArgs()
{
    char execWithNoArgs[] = "cmd1";

    struct exec_cmd *execNode1 = execCtor(execWithNoArgs, strlen(execWithNoArgs));
    struct exec_cmd *execNode2 = execCtor(execWithNoArgs, strlen(execWithNoArgs));

    nullTerminate((struct cmd *)execNode1);

    assert(equalNodes((struct cmd *)execNode1, (struct cmd *)execNode2));

    freeCmd((struct cmd *)execNode1);
    freeCmd((struct cmd *)execNode2);

    printf("testEqualNodesTwoExecsWithSameCommandNoArgs PASSED\n");
}

void testEqualNodesTwoExecsWithSameCommandOneArg()
{
    char execWithOneArgs[] = "cmd1 arg1";

    struct exec_cmd *execNode1 = execCtor(execWithOneArgs, strlen(execWithOneArgs));
    struct exec_cmd *execNode2 = execCtor(execWithOneArgs, strlen(execWithOneArgs));

    nullTerminate((struct cmd *)execNode1);

    assert(equalNodes((struct cmd *)execNode1, (struct cmd *)execNode2));

    freeCmd((struct cmd *)execNode1);
    freeCmd((struct cmd *)execNode2);

    printf("testEqualNodesTwoExecsWithSameCommandOneArg PASSED\n");
}

void testEqualNodesTwoExecsWithSameCommandTwoArgs()
{
    char execWithTwoArgs[] = "cmd1 arg1 arg2";

    struct exec_cmd *execNode1 = execCtor(execWithTwoArgs, strlen(execWithTwoArgs));
    struct exec_cmd *execNode2 = execCtor(execWithTwoArgs, strlen(execWithTwoArgs));

    nullTerminate((struct cmd *)execNode1);

    assert(equalNodes((struct cmd *)execNode1, (struct cmd *)execNode2));

    freeCmd((struct cmd *)execNode1);
    freeCmd((struct cmd *)execNode2);

    printf("testEqualNodesTwoExecsWithSameCommandTwoArgs PASSED\n");
}

void testEqualNodesTwoExecsWithCommandsDifferentSizeOfArgs()
{
    char execWithOneArgs[] = "cmd1 arg1";
    char execWithTwoArgs[] = "cmd1 arg1 arg2";

    struct exec_cmd *execNode1 = execCtor(execWithOneArgs, strlen(execWithOneArgs));
    struct exec_cmd *execNode2 = execCtor(execWithTwoArgs, strlen(execWithTwoArgs));

    nullTerminate((struct cmd *)execNode1);
    nullTerminate((struct cmd *)execNode2);

    assert(!equalNodes((struct cmd *)execNode1, (struct cmd *)execNode2));

    freeCmd((struct cmd *)execNode1);
    freeCmd((struct cmd *)execNode2);

    printf("testEqualNodesTwoExecsWithCommandsDifferentSizeOfArgs PASSED\n");
}

void testEqualNodesTwoExecsWithOneCommandNoArgsOneCommandWithArgs()
{
    char execWithNoArgs[] = "cmd1";
    char execWithOneArgs[] = "cmd1 arg1";

    struct exec_cmd *execNode1 = execCtor(execWithNoArgs, strlen(execWithNoArgs));
    struct exec_cmd *execNode2 = execCtor(execWithOneArgs, strlen(execWithOneArgs));

    nullTerminate((struct cmd *)execNode1);
    nullTerminate((struct cmd *)execNode2);

    assert(!equalNodes((struct cmd *)execNode1, (struct cmd *)execNode2));

    freeCmd((struct cmd *)execNode1);
    freeCmd((struct cmd *)execNode2);

    printf("testEqualNodesTwoExecsWithOneCommandNoArgsOneCommandWithArgs PASSED\n");
}

// Consec Nodes

void testEqualNodesSameConsec()
{
    char cmd1[] = "cmd1 arg1";
    char cmd2[] = "cmd2 arg2";

    struct exec_cmd *left = execCtor(cmd1, strlen(cmd1));
    struct exec_cmd *left2 = execCtor(cmd1, strlen(cmd1));
    nullTerminate((struct cmd *)left);

    struct exec_cmd *right = execCtor(cmd2, strlen(cmd2));
    struct exec_cmd *right2 = execCtor(cmd2, strlen(cmd2));
    nullTerminate((struct cmd *)right);

    struct consec_cmd *node1 = consecCtor((struct cmd *)left, (struct cmd *)right);
    struct consec_cmd *node2 = consecCtor((struct cmd *)left2, (struct cmd *)right2);

    assert(equalNodes((struct cmd *)node1, (struct cmd *)node2));

    freeCmd((struct cmd *)node1);
    freeCmd((struct cmd *)node2);

    printf("testEqualNodesSameConsec PASSED\n");
}

void testEqualNodesDifferentConsec()
{
    char cmd1[] = "cmd1 arg1";
    char cmd2[] = "cmd2 arg2";
    char cmd3[] = "cmd3 arg3";

    struct exec_cmd *left = execCtor(cmd1, strlen(cmd1));
    nullTerminate((struct cmd *)left);

    struct exec_cmd *right1 = execCtor(cmd2, strlen(cmd2));
    nullTerminate((struct cmd *)right1);

    struct exec_cmd *right2 = execCtor(cmd3, strlen(cmd3));
    nullTerminate((struct cmd *)right2);

    struct consec_cmd *node1 = consecCtor((struct cmd *)left, (struct cmd *)right1);
    struct consec_cmd *node2 = consecCtor((struct cmd *)left, (struct cmd *)right2);

    assert(!equalNodes((struct cmd *)node1, (struct cmd *)node2));

    printf("testEqualNodesDifferentConsec PASSED\n");

    freeCmd((struct cmd *)node1);
    freeCmd((struct cmd *)right2);
}

void testEqualNodesNestedConsec()
{
    char cmd1[] = "cmd1";
    char cmd2[] = "cmd2";
    char cmd3[] = "cmd3";

    struct exec_cmd *nodeA1 = execCtor(cmd1, strlen(cmd1));
    struct exec_cmd *nodeB1 = execCtor(cmd1, strlen(cmd1));
    nullTerminate((struct cmd *)nodeA1);

    struct exec_cmd *nodeA2 = execCtor(cmd2, strlen(cmd2));
    struct exec_cmd *nodeB2 = execCtor(cmd2, strlen(cmd2));
    nullTerminate((struct cmd *)nodeA2);

    struct exec_cmd *nodeA3 = execCtor(cmd3, strlen(cmd3));
    struct exec_cmd *nodeB3 = execCtor(cmd3, strlen(cmd3));
    nullTerminate((struct cmd *)nodeA3);

    struct consec_cmd *inner1 = consecCtor((struct cmd *)nodeA1, (struct cmd *)nodeA2);
    struct consec_cmd *outer1 = consecCtor((struct cmd *)inner1, (struct cmd *)nodeA3);

    struct consec_cmd *inner2 = consecCtor((struct cmd *)nodeB1, (struct cmd *)nodeB2);
    struct consec_cmd *outer2 = consecCtor((struct cmd *)inner2, (struct cmd *)nodeB3);

    assert(equalNodes((struct cmd *)outer1, (struct cmd *)outer2));

    freeCmd((struct cmd *)outer1);
    freeCmd((struct cmd *)outer2);

    printf("testEqualNodesNestedConsec PASSED\n");
}

// Pipe Nodes

void testEqualNodesSamePipe()
{
    char cmd1[] = "cmd1 arg1";
    char cmd2[] = "cmd2 arg2";

    struct exec_cmd *left = execCtor(cmd1, strlen(cmd1));
    struct exec_cmd *right = execCtor(cmd2, strlen(cmd2));

    struct pipe_cmd *node1 = pipeCtor((struct cmd *)left, (struct cmd *)right);
    struct pipe_cmd *node2 = pipeCtor((struct cmd *)execCtor(cmd1, strlen(cmd1)), (struct cmd *)execCtor(cmd2, strlen(cmd2)));

    nullTerminate((struct cmd *)left);
    nullTerminate((struct cmd *)right);

    assert(equalNodes((struct cmd *)node1, (struct cmd *)node2));

    printf("testEqualNodesSamePipe PASSED\n");

    freeCmd((struct cmd *)node1);
    freeCmd((struct cmd *)node2);
}

void testEqualNodesDifferentPipe()
{
    char cmd1[] = "cmd1 arg1";
    char cmd2[] = "cmd2 arg2";
    char cmd3[] = "cmd3 arg3";

    struct exec_cmd *left = execCtor(cmd1, strlen(cmd1));
    struct exec_cmd *right1 = execCtor(cmd2, strlen(cmd2));
    struct exec_cmd *right2 = execCtor(cmd3, strlen(cmd3));

    nullTerminate((struct cmd *)left);
    nullTerminate((struct cmd *)right1);
    nullTerminate((struct cmd *)right2);

    struct pipe_cmd *node1 = pipeCtor((struct cmd *)left, (struct cmd *)right1);
    struct pipe_cmd *node2 = pipeCtor((struct cmd *)left, (struct cmd *)right2);

    assert(!equalNodes((struct cmd *)node1, (struct cmd *)node2));

    printf("testEqualNodesDifferentPipe PASSED\n");

    freeCmd((struct cmd *)node1);
}

void testEqualNodesNestedPipe()
{
    char cmd1[] = "cmd1";
    char cmd2[] = "cmd2";
    char cmd3[] = "cmd3";

    struct exec_cmd *n1 = execCtor(cmd1, strlen(cmd1));
    struct exec_cmd *n2 = execCtor(cmd2, strlen(cmd2));
    struct exec_cmd *n3 = execCtor(cmd3, strlen(cmd3));

    nullTerminate((struct cmd *)n1);
    nullTerminate((struct cmd *)n2);
    nullTerminate((struct cmd *)n3);

    struct pipe_cmd *inner1 = pipeCtor((struct cmd *)n1, (struct cmd *)n2);
    struct pipe_cmd *outer1 = pipeCtor((struct cmd *)inner1, (struct cmd *)n3);

    struct pipe_cmd *inner2 = pipeCtor((struct cmd *)execCtor(cmd1, strlen(cmd1)), (struct cmd *)execCtor(cmd2, strlen(cmd2)));
    struct pipe_cmd *outer2 = pipeCtor((struct cmd *)inner2, (struct cmd *)execCtor(cmd3, strlen(cmd3)));

    assert(equalNodes((struct cmd *)outer1, (struct cmd *)outer2));

    printf("testEqualNodesNestedPipe PASSED\n");

    freeCmd((struct cmd *)outer1);
    freeCmd((struct cmd *)outer2);
}

// Background Nodes

void testEqualNodesSameBackground()
{
    char cmd[] = "cmd1 arg1";
    struct exec_cmd *child = execCtor(cmd, strlen(cmd));

    struct background_cmd *node1 = backgroundCtor((struct cmd *)child);
    struct background_cmd *node2 = backgroundCtor((struct cmd *)execCtor(cmd, strlen(cmd)));

    nullTerminate((struct cmd *)child);

    assert(equalNodes((struct cmd *)node1, (struct cmd *)node2));

    printf("testEqualNodesSameBackground PASSED\n");

    freeCmd((struct cmd *)node1);
    freeCmd((struct cmd *)node2);
}

void testEqualNodesDifferentBackground()
{
    char cmd1[] = "cmd1 arg1";
    char cmd2[] = "cmd2 arg2";

    struct exec_cmd *child1 = execCtor(cmd1, strlen(cmd1));
    struct exec_cmd *child2 = execCtor(cmd2, strlen(cmd2));

    nullTerminate((struct cmd *)child1);
    nullTerminate((struct cmd *)child2);

    struct background_cmd *node1 = backgroundCtor((struct cmd *)child1);
    struct background_cmd *node2 = backgroundCtor((struct cmd *)child2);

    assert(!equalNodes((struct cmd *)node1, (struct cmd *)node2));

    printf("testEqualNodesDifferentBackground PASSED\n");

    freeCmd((struct cmd *)node1);
    freeCmd((struct cmd *)node2);
}

void testEqualNodesNestedBackground()
{
    char cmd1[] = "cmd1";
    char cmd2[] = "cmd2";

    struct exec_cmd *exec1 = execCtor(cmd1, strlen(cmd1));
    struct exec_cmd *exec2 = execCtor(cmd2, strlen(cmd2));

    nullTerminate((struct cmd *)exec1);
    nullTerminate((struct cmd *)exec2);

    struct pipe_cmd *pipe = pipeCtor((struct cmd *)exec1, (struct cmd *)exec2);

    struct background_cmd *bg1 = backgroundCtor((struct cmd *)pipe);
    struct background_cmd *bg2 = backgroundCtor(
        (struct cmd *)pipeCtor((struct cmd *)execCtor(cmd1, strlen(cmd1)), (struct cmd *)execCtor(cmd2, strlen(cmd2))));

    assert(equalNodes((struct cmd *)bg1, (struct cmd *)bg2));

    printf("testEqualNodesNestedBackground PASSED\n");

    freeCmd((struct cmd *)bg1);
    freeCmd((struct cmd *)bg2);
}

// Redirect Nodes

void testEqualNodesSameRedirect()
{
    char cmd[] = "cmd";
    char file[] = "output.txt";
    char *efile = file + strlen(file);

    struct exec_cmd *execNode = execCtor(cmd, strlen(cmd));
    struct redirect_cmd *redir1 = redirectCtor((struct cmd *)execNode, file, efile, 2, 0);
    struct redirect_cmd *redir2 = redirectCtor((struct cmd *)execCtor(cmd, strlen(cmd)), file, efile, 2, 0);

    assert(equalNodes((struct cmd *)redir1, (struct cmd *)redir2));

    printf("testEqualNodesSameRedirect PASSED\n");

    freeCmd((struct cmd *)redir1);
    freeCmd((struct cmd *)redir2);
}

void testEqualNodesDifferentFile()
{
    char cmd[] = "cmd";
    char file1[] = "output1.txt";
    char file2[] = "output2.txt";
    char *efile1 = file1 + strlen(file1);
    char *efile2 = file2 + strlen(file2);

    struct exec_cmd *execNode = execCtor(cmd, strlen(cmd));
    struct redirect_cmd *redir1 = redirectCtor((struct cmd *)execNode, file1, efile1, 2, 0);
    struct redirect_cmd *redir2 = redirectCtor((struct cmd *)execCtor(cmd, strlen(cmd)), file2, efile2, 2, 0);

    assert(!equalNodes((struct cmd *)redir1, (struct cmd *)redir2));

    printf("testEqualNodesDifferentFile PASSED\n");

    freeCmd((struct cmd *)redir1);
    freeCmd((struct cmd *)redir2);
}

void testEqualNodesDifferentMode()
{
    char cmd[] = "cmd";
    char file[] = "out.txt";
    char *efile = file + strlen(file);

    struct redirect_cmd *redir1 = redirectCtor((struct cmd *)execCtor(cmd, strlen(cmd)), file, efile, 1, 0);
    struct redirect_cmd *redir2 = redirectCtor((struct cmd *)execCtor(cmd, strlen(cmd)), file, efile, 2, 0);

    assert(!equalNodes((struct cmd *)redir1, (struct cmd *)redir2));

    printf("testEqualNodesDifferentMode PASSED\n");

    freeCmd((struct cmd *)redir1);
    freeCmd((struct cmd *)redir2);
}

void testEqualNodesDifferentFD()
{
    char cmd[] = "cmd";
    char file[] = "out.txt";
    char *efile = file + strlen(file);

    struct redirect_cmd *redir1 = redirectCtor((struct cmd *)execCtor(cmd, strlen(cmd)), file, efile, 2, 0); // stdout
    struct redirect_cmd *redir2 = redirectCtor((struct cmd *)execCtor(cmd, strlen(cmd)), file, efile, 2, 1); // stdin

    assert(!equalNodes((struct cmd *)redir1, (struct cmd *)redir2));

    printf("testEqualNodesDifferentFD PASSED\n");

    freeCmd((struct cmd *)redir1);
    freeCmd((struct cmd *)redir2);
}

// tests mixed AST Nodes

void testEqualNodesConsecWithPipeAndExec()
{
    // (cmd1 arg1 | cmd2 arg2) ; cmd3 arg3
    char c1[] = "cmd1 arg1";
    char c2[] = "cmd2 arg2";
    char c3[] = "cmd3 arg3";

    struct pipe_cmd *pipeNode1 = pipeCtor((struct cmd *)execCtor(c1, strlen(c1)),
                                          (struct cmd *)execCtor(c2, strlen(c2)));
    struct consec_cmd *tree1 = consecCtor((struct cmd *)pipeNode1,
                                          (struct cmd *)execCtor(c3, strlen(c3)));

    struct pipe_cmd *pipeNode2 = pipeCtor((struct cmd *)execCtor(c1, strlen(c1)),
                                          (struct cmd *)execCtor(c2, strlen(c2)));
    struct consec_cmd *tree2 = consecCtor((struct cmd *)pipeNode2,
                                          (struct cmd *)execCtor(c3, strlen(c3)));

    assert(equalNodes((struct cmd *)tree1, (struct cmd *)tree2));
    printf("testEqualNodesConsecWithPipeAndExec PASSED\n");

    freeCmd((struct cmd *)tree1);
    freeCmd((struct cmd *)tree2);
}

void testEqualNodesBackgroundWithRedirect()
{
    // (cmd1 arg1 > out.txt) &
    char cmd[] = "cmd1 arg1";
    char file[] = "out.txt";

    struct redirect_cmd *redir1 = redirectCtor((struct cmd *)execCtor(cmd, strlen(cmd)),
                                               file, file + strlen(file), 2, 1);
    struct background_cmd *bg1 = backgroundCtor((struct cmd *)redir1);

    struct redirect_cmd *redir2 = redirectCtor((struct cmd *)execCtor(cmd, strlen(cmd)),
                                               file, file + strlen(file), 2, 1);
    struct background_cmd *bg2 = backgroundCtor((struct cmd *)redir2);

    assert(equalNodes((struct cmd *)bg1, (struct cmd *)bg2));
    printf("testEqualNodesBackgroundWithRedirect PASSED\n");

    freeCmd((struct cmd *)bg1);
    freeCmd((struct cmd *)bg2);
}

void testEqualNodesPipeWithRedirectAndExec()
{
    // (cmd1 arg1 > out.txt) | cmd2 arg2
    char c1[] = "cmd1 arg1";
    char c2[] = "cmd2 arg2";
    char file[] = "out.txt";

    struct redirect_cmd *redir = redirectCtor((struct cmd *)execCtor(c1, strlen(c1)),
                                              file, file + strlen(file), 2, 1);
    struct pipe_cmd *pipe1 = pipeCtor((struct cmd *)redir,
                                      (struct cmd *)execCtor(c2, strlen(c2)));

    struct redirect_cmd *redir2 = redirectCtor((struct cmd *)execCtor(c1, strlen(c1)),
                                               file, file + strlen(file), 2, 1);
    struct pipe_cmd *pipe2 = pipeCtor((struct cmd *)redir2,
                                      (struct cmd *)execCtor(c2, strlen(c2)));

    assert(equalNodes((struct cmd *)pipe1, (struct cmd *)pipe2));
    printf("testEqualNodesPipeWithRedirectAndExec PASSED\n");

    freeCmd((struct cmd *)pipe1);
    freeCmd((struct cmd *)pipe2);
}

void testEqualNodesDeepMixedTree()
{
    // (((cmd1 arg1 | cmd2 arg2) ; cmd3 arg3) > out.txt) &
    char c1[] = "cmd1 arg1";
    char c2[] = "cmd2 arg2";
    char c3[] = "cmd3 arg3";
    char file[] = "out.txt";

    struct pipe_cmd *pipe = pipeCtor((struct cmd *)execCtor(c1, strlen(c1)),
                                     (struct cmd *)execCtor(c2, strlen(c2)));
    struct consec_cmd *consec = consecCtor((struct cmd *)pipe,
                                           (struct cmd *)execCtor(c3, strlen(c3)));
    struct redirect_cmd *redir = redirectCtor((struct cmd *)consec,
                                              file, file + strlen(file), 2, 1);
    struct background_cmd *bg1 = backgroundCtor((struct cmd *)redir);

    // Build again
    struct pipe_cmd *pipe2 = pipeCtor((struct cmd *)execCtor(c1, strlen(c1)),
                                      (struct cmd *)execCtor(c2, strlen(c2)));
    struct consec_cmd *consec2 = consecCtor((struct cmd *)pipe2,
                                            (struct cmd *)execCtor(c3, strlen(c3)));
    struct redirect_cmd *redir2 = redirectCtor((struct cmd *)consec2,
                                               file, file + strlen(file), 2, 1);
    struct background_cmd *bg2 = backgroundCtor((struct cmd *)redir2);

    assert(equalNodes((struct cmd *)bg1, (struct cmd *)bg2));
    printf("testEqualNodesDeepMixedTree PASSED\n");

    freeCmd((struct cmd *)bg1);
    freeCmd((struct cmd *)bg2);
}

void testEqualNodesNestedConsecPipeBackground()
{
    // ((cmd1 arg1 ; (cmd2 arg2 | cmd3 arg3)) &) ; cmd4 arg4
    char c1[] = "cmd1 arg1";
    char c2[] = "cmd2 arg2";
    char c3[] = "cmd3 arg3";
    char c4[] = "cmd4 arg4";

    struct pipe_cmd *pipe = pipeCtor((struct cmd *)execCtor(c2, strlen(c2)),
                                     (struct cmd *)execCtor(c3, strlen(c3)));
    struct consec_cmd *innerConsec = consecCtor((struct cmd *)execCtor(c1, strlen(c1)),
                                                (struct cmd *)pipe);
    struct background_cmd *bg = backgroundCtor((struct cmd *)innerConsec);
    struct consec_cmd *tree1 = consecCtor((struct cmd *)bg,
                                          (struct cmd *)execCtor(c4, strlen(c4)));

    // Clone for comparison
    struct pipe_cmd *pipe2 = pipeCtor((struct cmd *)execCtor(c2, strlen(c2)),
                                      (struct cmd *)execCtor(c3, strlen(c3)));
    struct consec_cmd *innerConsec2 = consecCtor((struct cmd *)execCtor(c1, strlen(c1)),
                                                 (struct cmd *)pipe2);
    struct background_cmd *bg2 = backgroundCtor((struct cmd *)innerConsec2);
    struct consec_cmd *tree2 = consecCtor((struct cmd *)bg2,
                                          (struct cmd *)execCtor(c4, strlen(c4)));

    assert(equalNodes((struct cmd *)tree1, (struct cmd *)tree2));
    printf("testEqualNodesNestedConsecPipeBackground PASSED\n");

    freeCmd((struct cmd *)tree1);
    freeCmd((struct cmd *)tree2);
}

int main()
{

    // TESTS FOR EQUALNODE FUNCTION

    // exec nodes
    testEqualNodesTwoExecsWithSameCommandNoArgs();
    testEqualNodesTwoExecsWithSameCommandOneArg();
    testEqualNodesTwoExecsWithSameCommandTwoArgs();
    testEqualNodesTwoExecsWithCommandsDifferentSizeOfArgs();
    testEqualNodesTwoExecsWithOneCommandNoArgsOneCommandWithArgs();

    printf("%c", '\n');

    // consec nodes
    testEqualNodesSameConsec();
    testEqualNodesDifferentConsec();
    testEqualNodesNestedConsec();

    printf("%c", '\n');

    // pipe nodes
    testEqualNodesSamePipe();
    testEqualNodesDifferentPipe();
    testEqualNodesNestedPipe();

    printf("%c", '\n');

    // background nodes
    testEqualNodesSameBackground();
    testEqualNodesDifferentBackground();
    testEqualNodesNestedBackground();

    printf("%c", '\n');

    // redirect nodes
    testEqualNodesSameRedirect();
    testEqualNodesDifferentFile();
    testEqualNodesDifferentMode();
    testEqualNodesDifferentFD();

    printf("%c", '\n');

    // mixed AST nodes
    testEqualNodesConsecWithPipeAndExec();
    testEqualNodesBackgroundWithRedirect();
    testEqualNodesPipeWithRedirectAndExec();
    testEqualNodesDeepMixedTree();
    testEqualNodesNestedConsecPipeBackground();

    return 0;
}
