#include "utils.h"
#include "linked_list.h"

void
print_progress_bar(int width, float ratio) {
    int   c = ratio * width;

    printf("%3d%% [", (int)(ratio*100));
                // Show the load bar.
    int x;
    for(x=0; x<c; x++)
       printf("=");

    printf(">");

    for(x=c; x<width; x++)
       printf(" ");

    printf("]\r");
    fflush(stdout);
}

int
upload(int clientfd, const char from[PATH_MAX])
{
    int fd_from = open(from, O_RDONLY);
    int nread;
    char buf[4096];
    struct stat st;
    int size;
    int total= 0;

    stat(from, &st);
    size = st.st_size;

    printf("Uploading %s\n", from);

    while (nread = read(fd_from, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;
        total += nread;

        do {
            nwritten = write(clientfd, out_ptr, nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                printf("ERROR");
            }

            float ratio = (float)total/(float)size;
            print_progress_bar(45, ratio);

        } while (nread > 0);
    }
    printf("\n\n");
    close(fd_from);
    return 0;
}

int
download(int sockfd, const char to[PATH_MAX], int mtime, int mode, int size)
{
    int BUFFER = 4096;
    char buf[BUFFER];
    int nread;
    struct utimbuf new_times;
    int total = 0;

    printf("Downloading %s\n", to);

    if(S_ISDIR(mode)) {
            mode_t process_mask = umask(0);
            mkdir(to, mode);
            umask(process_mask);
            new_times.modtime = mtime;

            if(utime(to, &new_times) < 0) {
                perror(to);
                return 1;
            }
            return 0;
    }

    int fd_to = open(to, O_WRONLY | O_CREAT | O_TRUNC);
    if(fd_to < 0)
        perror("fd_to");

    if(size >0)
        while (nread = read(sockfd, buf, sizeof buf), nread > 0)
        {
            char *out_ptr = buf;
            ssize_t nwritten;
            total += nread;

            do {
                nwritten = write(fd_to, out_ptr, nread);

                if (nwritten >= 0)
                {
                    nread -= nwritten;
                    out_ptr += nwritten;
                }
                else if (errno != EINTR)
                {
                    perror("ERROR");
                }
            } while (nread > 0);

            float ratio = (float)total/(float)size;
            print_progress_bar(45, ratio);

            if(total == size)
                break;
        }
    printf("\n\n");
    close(fd_to);

    new_times.modtime = mtime;

    if(utime(to, &new_times) < 0) {
        perror(to);
        return 1;
    }

    if(chmod(to, mode) < 0) {
        perror(to);
        return 1;
    }

    return 0;
}

int
cp(char *from, char *to)
{
    int fd_to, fd_from;
    struct stat s;
    char buf[4096];
    ssize_t nread;
    int saved_errno;
    struct utimbuf new_times;

    if(check_dir_exist(from) == 0) {
        mkdir(to, 0755);

        if (stat(from, &s) < 0) {
            perror(from);
            return 1;
        }

        new_times.actime = s.st_atime;
        new_times.modtime = s.st_mtime;

        if(utime(to, &new_times) < 0) {
            perror(to);
            return 1;
        }

        return 0;
    }

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

        if (stat(from, &s) < 0) {
            perror(from);
            return 1;
        }

        new_times.actime = s.st_atime;
        new_times.modtime = s.st_mtime;
        if(utime(to, &new_times) < 0) {
            perror(to);
            return 1;
        }

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
        if(rel_path != NULL && strcmp(rel_path, "") !=0)
            snprintf (filename, PATH_MAX, "%s/%s", rel_path, entry->d_name);

        struct stat s;
        char test[PATH_MAX];
        snprintf (test, PATH_MAX, "%s/%s", directory, entry->d_name);

        stat(test, &s);

        insert(list, filename, s.st_mtime);

        if(entry->d_type == DT_DIR) {
            path_length = snprintf (path, PATH_MAX, "%s/%s", directory, entry->d_name);
            if(rel_path == NULL || strcmp(rel_path, "") == 0) {
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
