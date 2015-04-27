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
    char *dir1;
    char *dir2;

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

    dir1 = argv[optind];
    dir2 = argv[optind+1];

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

    printf("***Syncing %s and %s***\n", dir1, dir2);

    printf("========%s========\n", dir1);
    List *l = explore_dir_rec(dir1);

    //printList(l);
    printf("========%s========\n", dir2);
    List *l2 = explore_dir_rec(dir2);
    /*List *l = init();
    List *l2 = init();
    insert(l2, "test1");
    insert(l2, "test2");
    insert(l2, "test5");
    insert(l, "test3");
    insert(l, "test3");
    insert(l, "test3");
    insert(l, "test3");
    printList(l);*/
    printList(l2);
    printList(l);

    //compareLists(l);
    //printList(diff);

    //destroy(l);
    //destroy(l2);

    return 0;
}
