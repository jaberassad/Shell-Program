#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

int mkdir_(char *argv[]) {
    if (!argv[1]) {
        fprintf(stderr, "mkdir: missing argument\n");
        return 1;
    }
    if (mkdir(argv[1], 0755) != 0) {
        perror("mkdir");
        return 1;
    }
    return 0;
}

int rmdir_(char *argv[]) {
    if (!argv[1]) {
        fprintf(stderr, "rmdir: missing argument\n");
        return 1;
    }
    if (rmdir(argv[1]) != 0) {
        perror("rmdir");
        return 1;
    }
    return 0;
}



int mv_(char *argv[]) {
    // Check argument count
    if (!argv[2]) {
        fprintf(stderr, "mv: missing file operand\n");
        fprintf(stderr, "Usage: mv SOURCE DEST\n");
        return 1;
    }
    
    if (argv[3]) {
        fprintf(stderr, "mv: too many arguments\n");
        fprintf(stderr, "Usage: mv SOURCE DEST\n");
        return 1;
    }
    
    char *source = argv[1];
    char *dest = argv[2];
    
    struct stat source_stat;
    if (stat(source, &source_stat) != 0) {
        fprintf(stderr, "mv: cannot stat '%s': %s\n", source, strerror(errno));
        return 1;
    }
    
    struct stat dest_stat;
    char final_dest[1024];
    
    if (stat(dest, &dest_stat) == 0 && S_ISDIR(dest_stat.st_mode)) {
        char *filename = strrchr(source, '/');
        if (filename == NULL) {
            filename = source;
        } else {
            filename++;
        }
        
        snprintf(final_dest, sizeof(final_dest), "%s/%s", dest, filename);
        dest = final_dest;
    }
    
    if (rename(source, dest) != 0) {
        fprintf(stderr, "mv: cannot move '%s' to '%s': %s\n", 
                source, dest, strerror(errno));
        return 1;
    }
    
    return 0;
}