#include "utils.h"
#include "linked_list.h"

List *
explore_dir_rec(char *directory) {
    List *files = init();

    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];
    int path_length;

    dir = opendir(directory);

    while( (entry = readdir(dir)) != NULL ) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, ".." ) == 0)
            continue;

        printf("%s\n", entry->d_name);
        insert(files, entry->d_name);

        if(entry->d_type == DT_DIR) {
            path_length = snprintf (path, PATH_MAX, "%s/%s", directory, entry->d_name);
            if(path_length >= PATH_MAX) {
                printf("Path too long");
                return NULL;
            }
            explore_dir_rec(path);
        }
    }
    closedir(dir);
    return files;
}

int
check_dir_exist(char *dir) {
    struct stat s;
    int rc;

    rc = stat(dir, &s);
    if(rc == -1)
        return 1;

    if(S_ISDIR(s.st_mode ) == 0)
        return 1;

    return 0;
}
