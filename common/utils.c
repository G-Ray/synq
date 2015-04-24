#include "utils.h"

void
explore_dir_rec(char *directory) {
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];
    int path_length;

    dir = opendir(directory);

    while( (entry = readdir(dir)) != NULL ) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, ".." ) == 0)
            continue;

        printf("%s\n", entry->d_name);

        if(entry->d_type == DT_DIR) {
            path_length = snprintf (path, PATH_MAX, "%s/%s", directory, entry->d_name);
            if(path_length >= PATH_MAX) {
                printf("Path too long");
                return;
            }
            explore_dir_rec(path);
        }
    }
    closedir(dir);
}
