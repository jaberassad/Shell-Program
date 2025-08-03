#include "../parse.c"
#include <assert.h>
#include <string.h>
#include "../nullTerminate.c"
#include "../cmd_utils.c"

void testParseOneExec()
{
    char commandString[] = "cmd1 arg1 arg2";

    struct cmd *node = parse(commandString, 0, strlen(commandString));
    struct cmd *testNode = (struct cmd *)execCtor(commandString, strlen(commandString));

    nullTerminate(node);

    assert(equalNodes(node, testNode));

    printf("testParseOneExec PASSED\n");

    freeCmd(node);
    freeCmd(testNode);
}

void testParseOneConsec()
{
    char commandString[] = "cmd1 arg1 ; cmd2 arg2";
    char leftString[] = "cmd1 arg1";
    char rightString[] = "cmd2 arg2";

    struct cmd *left = (struct cmd *)execCtor(leftString, strlen(leftString));
    struct cmd *right = (struct cmd *)execCtor(rightString , strlen(rightString));

    struct cmd *node = parse(commandString, 0, strlen(commandString));

    struct cmd *testNode = (struct cmd *)consecCtor(left, right);


    nullTerminate(node);
    nullTerminate(testNode);

    for(int i=0; i<2; i++){
        printf("%s\n", ((struct exec_cmd *)((struct consec_cmd *)node)->left)->argv[i]);
    }

    assert(equalNodes(node, testNode));

    printf("testParseOneConsec PASSED\n");

    freeCmd(node);
    freeCmd(testNode);
}

void testParseOnePipe()
{
    char commandString[] = "cmd1 arg1 | cmd2 arg2";

    struct cmd *node = parse(commandString, 0, strlen(commandString));
    struct cmd *testNode = (struct cmd *)pipeCtor((struct cmd *)execCtor("cmd1 arg1", 10), (struct cmd *)execCtor("cmd2 arg2", 10));

    assert(equalNodes(node, testNode));

    printf("testParseOnePipe PASSED\n");

    freeCmd(node);
    freeCmd(testNode);
}

void testParseOneBackground()
{
    char commandString[] = "cmd1 arg1 arg2 &";

    struct cmd *node = parse(commandString, 0, strlen(commandString));
    struct cmd *testNode = (struct cmd *)pipeCtor((struct cmd *)execCtor("cmd1 arg1", 10), (struct cmd *)execCtor("cmd2 arg2", 10));

    assert(equalNodes(node, testNode));

    printf("testParseOneBackground PASSED\n");

    freeCmd(node);
    freeCmd(testNode);
}

void testParseOneInputRedir()
{
}

void testParseOneOutputTruncateRedir()
{
}

void testParseOneOutputAppendRedir()
{
}

void testEmptyInput()
{
}

int main(void)
{

    testParseOneExec();
    testParseOneConsec();
    testParseOnePipe();
    testParseOneBackground();

    return 0;
}