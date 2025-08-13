#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    long lines;
    long words;
    long chars;
    long bytes;
} wc_counts;

wc_counts count_from_file(FILE *fp) {
    wc_counts counts = {0, 0, 0, 0};
    int c;
    int in_word = 0;
    
    while ((c = fgetc(fp)) != EOF) {
        counts.bytes++;
        counts.chars++;
        
        if (c == '\n') {
            counts.lines++;
        }
        
        if (isspace(c)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            counts.words++;
        }
    }
    
    return counts;
}

void print_counts(wc_counts counts, const char *filename) {
    printf("%8ld %8ld %8ld", counts.lines, counts.words, counts.chars);
    if (filename) {
        printf(" %s", filename);
    }
    printf("\n");
}

int wc(char *argv[]) {
    int argc = 0;
    while (argv[argc] != NULL) {
        argc++;
    }
    
    if (argc == 1) {
        wc_counts counts = count_from_file(stdin);
        print_counts(counts, NULL);
        return 0;
    }
    
    wc_counts total = {0, 0, 0, 0};
    int file_count = 0;
    int error_occurred = 0;
    
    for (int i = 1; i < argc; i++) {
        FILE *fp;
        wc_counts counts;
        
        if (strcmp(argv[i], "-") == 0) {
            fp = stdin;
            counts = count_from_file(fp);
            print_counts(counts, NULL);
        } else {
            fp = fopen(argv[i], "r");
            if (fp == NULL) {
                fprintf(stderr, "wc: %s: No such file or directory\n", argv[i]);
                error_occurred = 1;
                continue;
            }
            
            counts = count_from_file(fp);
            print_counts(counts, argv[i]);
            fclose(fp);
        }
        
        total.lines += counts.lines;
        total.words += counts.words;
        total.chars += counts.chars;
        total.bytes += counts.bytes;
        file_count++;
    }
    
    if (file_count > 1) {
        print_counts(total, "total");
    }
    
    return error_occurred ? 1 : 0;
}