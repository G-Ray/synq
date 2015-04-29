#include "utils.h"
#include "linked_list.h"

int
cp(char *from, char *to)
{
    int fd_to, fd_from;
    char buf[4096];
    ssize_t nread;
    int saved_errno;

    if(check_dir_exist(from) == 0) {
        mkdir(to, 0755);
        return 0;
    }

    printf("%s\n", from);
    fd_from = open(from, O_RDONLY);

    if (fd_from < 0)
        return -1;

    fd_to = open(to, O_WRONLY | O_CREAT, 0666);

    if (fd_to < 0)
        goto out_error;

    while (nread = read(fd_from, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;

        do {
            nwritten = write(fd_to, out_ptr, nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                goto out_error;
            }
        } while (nread > 0);
    }

    if (nread == 0)
    {
        if (close(fd_to) < 0)
        {
            fd_to = -1;
            goto out_error;
        }
        close(fd_from);

        /* Success! */
        return 0;
    }

  out_error:
    saved_errno = errno;

    close(fd_from);
    if (fd_to >= 0)
        close(fd_to);

    errno = saved_errno;
    return -1;
}

void
explore_dir_rec(List *list, char *directory, char *rel_path) {
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];
    int path_length;
    char rel[PATH_MAX];

    dir = opendir(directory);

    while( (entry = readdir(dir)) != NULL ) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, ".." ) == 0)
            continue;

        char filename[NAME_MAX];
        strncpy(filename, entry->d_name, NAME_MAX+1);
        if(rel_path != NULL)
            snprintf (filename, PATH_MAX, "%s/%s", rel_path, entry->d_name);
        insert(list, filename);

        if(entry->d_type == DT_DIR) {
            path_length = snprintf (path, PATH_MAX, "%s/%s", directory, entry->d_name);
            if(rel_path == NULL) {
                rel_path = "";
                snprintf(rel, PATH_MAX, "%s%s", rel_path, entry->d_name);
            }
            else snprintf(rel, PATH_MAX, "%s/%s", rel_path, entry->d_name);
            if(path_length >= PATH_MAX) {
                printf("Path too long");
                return;
            }
            explore_dir_rec(list, path, rel);
        }
    }
    closedir(dir);
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
