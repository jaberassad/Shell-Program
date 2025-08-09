#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

void print_usage(const char *progname) {
    printf("Usage: %s [-lwc] [file]\n", progname);
    printf("  -l  count lines\n");
    printf("  -w  count words\n");
    printf("  -c  count characters\n");
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

int main(int argc, char *argv[]) {

    int count_lines = 0, count_words = 0, count_chars = 0;
    int option_l = 0, option_w = 0, option_c = 0;
    int arg_index = 1;


    while (arg_index < argc && argv[arg_index][0] == '-') {
        if (strcmp(argv[arg_index], "-l") == 0) {
            option_l = 1;
        } else if (strcmp(argv[arg_index], "-w") == 0) {
            option_w = 1;
        } else if (strcmp(argv[arg_index], "-c") == 0) {
            option_c = 1;
        } else if (strcmp(argv[arg_index], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[arg_index]);
            print_usage(argv[0]);
            return 1;
        }
        arg_index++;
    }


    if (!option_l && !option_w && !option_c) {
        option_l = option_w = option_c = 1;
    }

    FILE *file;



    if (arg_index < argc) {
        file = fopen(argv[arg_index], "r");
    }else{
        char *path = readStdin();
        argv[arg_index] = path;
        file = fopen(path, "r");
    }

    if (!file) {
        perror("fopen");
        return 1;
    }

    int c;
    int in_word = 0;

    while ((c = fgetc(file)) != EOF) {
        if (option_c) {
            count_chars++;
        }
        if (option_l && c == '\n') {
            count_lines++;
        }
        if (option_w) {
            if (isspace(c)) {
                if (in_word) {
                    count_words++;
                    in_word = 0;
                }
            } else {
                in_word = 1;
            }
        }
    }


    if (option_w && in_word) {
        count_words++;
    }

    if (file != stdin) {
        fclose(file);
    }


    if (option_l) {
        printf("%d ", count_lines+1);
    }
    if (option_w) {
        printf("%d ", count_words);
    }
    if (option_c) {
        printf("%d ", count_chars);
    }

    printf("%s\n", argv[arg_index]);

    return 0;
}
