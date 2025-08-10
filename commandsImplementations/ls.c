#include <stdio.h>
#include <dirent.h>
#include <errno.h>

int ls(char *argv[]) {

    const char *path;

    if (!argv[1]) {
        path = ".";
    } else {
        path = argv[1];
    }

    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
    return 0;
}
