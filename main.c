#include <stdio.h>
#include "parse.c"
#include <string.h>
#include "nullTerminate.c"
#include "executing.c"
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "cmd_utils.c"

char *banner =
"   $$$$$\\           $$\\                         $$\\                $$$$$$\\  $$\\                 $$\\ $$\\ \n"
"   \\__$$ |          $$ |                        $  |              $$  __$$\\ $$ |                $$ |$$ |\n"
"      $$ | $$$$$$\\  $$$$$$$\\   $$$$$$\\   $$$$$$\\\\_/$$$$$$$\\       $$ /  \\__|$$$$$$$\\   $$$$$$\\  $$ |$$ |\n"
"      $$ | \\____$$\\ $$  __$$\\ $$  __$$\\ $$  __$$\\ $$  _____|      \\$$$$$$\\  $$  __$$\\ $$  __$$\\ $$ |$$ |\n"
"$$\\   $$ | $$$$$$$ |$$ |  $$ |$$$$$$$$ |$$ |  \\__|\\$$$$$$\\         \\____$$\\ $$ |  $$ |$$$$$$$$ |$$ |$$ |\n"
"$$ |  $$ |$$  __$$ |$$ |  $$ |$$   ____|$$ |       \\____$$\\       $$\\   $$ |$$ |  $$ |$$   ____|$$ |$$ |\n"
"\\$$$$$$  |\\$$$$$$$ |$$$$$$$  |\\$$$$$$$\\ $$ |      $$$$$$$  |      \\$$$$$$  |$$ |  $$ |\\$$$$$$$\\ $$ |$$ |\n"
" \\______/  \\_______|\\_______/  \\_______|\\__|      \\_______/        \\______/ \\__|  \\__| \\_______|\\__|\\__|\n";


                                                                                                                                                        
                                                                                                                                                        


bool isCdCommand(int start, int end, char *commandString){

    while(commandString[start]==' ') start++;
    while(commandString[end]==';' || commandString[end]==' ' || commandString[end]=='\n' || commandString[end]=='\0') end--;

    if(end-start+1<2 || commandString[start]!='c' || commandString[start+1]!='d' || (end-start+1>2 && commandString[start+2]!=' ')){
        return false;
    }

    if(end-start+1>2){
        start = start + 3;
        while(commandString[start]==' ') start++;
        commandString[end+1]='\0';
        chdir(&commandString[start]);
    }

    return true;
}

int main(void){
    printf("%s\n", banner);
    printf("Welcome to Jaber's Shell!\n");
    printf("Supported features:\n");
    printf("  - Command grouping with parentheses: ( )\n");
    printf("  - Redirections: <  >  >>\n");
    printf("  - Background execution: &\n");
    printf("  - Pipes: |\n");
    printf("  - Command chaining: ;\n");
    printf("Built-in commands:\n");
    printf("  cd  cat  grep  mkdir  rmdir  mv  ls  sleep  echo\n");
    printf("%c\n", ' ');
    printf("%c\n", ' ');
    printf("%c\n", ' ');

    char line[1024];

    char cwd[PATH_MAX];
    char prompt[PATH_MAX + 20];
    
    while(1){
        getcwd(cwd, sizeof(cwd));
        strcpy(prompt, cwd);
        strcat(prompt, " Jaber's Shell>");

        write(2, prompt, strlen(prompt));
        if(fgets(line, 1024, stdin)==NULL){
            break;
        }

        if(!checkSyntax(line)) continue;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        int openedParenthesis = 0;
        int currIndex = 0;
        int lastEncounter = -1;

        while(currIndex<len){
            if((openedParenthesis==0 && line[currIndex]==';') || (currIndex==len-1)){
                if(!isCdCommand(lastEncounter+1, currIndex, line)){
                    int end;

                    if(line[currIndex]==';') end = currIndex-1;
                    else end = currIndex;

                    struct cmd* command = parse(line, lastEncounter+1, end);
                    nullTerminate(command);
                    executeCmd(command, true);
                }
                lastEncounter = currIndex;
            }else if(line[currIndex]=='(') openedParenthesis++;
            else if(line[currIndex]==')') openedParenthesis--;
            currIndex++;
        }

        usleep(500000);
    }
    
    return 0;
}