#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../common/protocol.h"
#include "../common/utils.h"
#include "../common/linked_list.h"

int
main(int argc, char **argv)
{
    int dry_run = 0;
    int c;
    char dir1[PATH_MAX];
    char dir2[PATH_MAX];

    if(argc < 3 || argc > 4) {
        printf("Usage: %s [options] <dir1> <dir2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    while ((c = getopt (argc, argv, "d")) != -1)
        switch (c)
            {
                case 'd':
                    dry_run = 1;
                    break;
                case '?':
                    if (isprint (optopt))
                        fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                    else
                        fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                  return 1;
                default:
                    abort ();
            }

    printf ("dry_run = %d\n", dry_run);

    //dir1 = argv[optind];
    //dir2 = argv[optind+1];
    strncpy(dir1, argv[optind], PATH_MAX);
    strncpy(dir2, argv[optind + 1] , PATH_MAX);

    if(dir1 == NULL || dir2 == NULL) {
        printf("Usage: %s [options] <dir1> <dir2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if(check_dir_exist(dir1) == 1) {
        printf("%s is not a directory\n", dir1);
        exit(EXIT_FAILURE);
    }

    if(check_dir_exist(dir2) == 1) {
        printf("%s is not a directory\n", dir2);
        exit(EXIT_FAILURE);
    }

    size_t length = strlen(dir1);
    if(dir1[length-1] != '/') {
        strcat(dir1, "/");
    }
    length = strlen(dir2);
    if(dir2[length-1] != '/') {
        strcat(dir2, "/");
    }

    printf("Syncing %s with %s\n", dir1, dir2);

    List *l1 = init();
    List *l2 = init();

    explore_dir_rec(l1, dir1, NULL);
    explore_dir_rec(l2, dir2, NULL);

    printf("========Content of %s========\n", dir1);
    printList(l1);
    printf("========Content of %s========\n", dir2);
    printList(l2);

    List * diff = compareLists(l1, l2);
    printf("Files to copy from %s to %s\n", dir1, dir2);
    printList(diff);

    File *current = diff->head;

    while (current != NULL)
    {
        char from[PATH_MAX];
        char to[PATH_MAX];
        snprintf(from, PATH_MAX, "%s/%s", dir1, current->path);
        snprintf(to, PATH_MAX, "%s%s", dir2, current->path);
        cp(from, to);
        printf("COPYING %s -> %s\n", from, to);
        perror("status");
        current = current->next;
    }

    printf("Files to copy from %s to %s\n", dir2, dir1);
    destroy(diff);
    diff = compareLists(l2, l1);
    printList(diff);

    current = diff->head;

    while (current != NULL)
    {
        char from[PATH_MAX];
        char to[PATH_MAX];
        snprintf (from, PATH_MAX, "%s/%s", dir2, current->path);
        snprintf (to, PATH_MAX, "%s%s", dir1, current->path);
        cp(from, to);
        printf("COPYING %s -> %s\n", from, to);
        perror("status");
        current = current->next;
    }

    //destroy(l);
    //destroy(l2);

    return 0;
}
