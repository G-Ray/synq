#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>

#include "../common/protocol.h"
#include "../common/utils.h"

void
check_dir_exist(char *dir) {
    struct stat s;
    int rc;

    rc = stat(dir, &s);
    if(rc == -1) {
        perror(dir);
        exit(EXIT_FAILURE);
    }

    if(S_ISDIR(s.st_mode ) == 0) {
        printf("%s is not a directory\n", dir);
        exit(EXIT_FAILURE);
    }
}

int
main(int argc, char **argv)
{
    int dry_run = 0;
    int c;
    char *dir1;
    char *dir2;

    if(argc < 3) {
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

    check_dir_exist(dir1);
    check_dir_exist(dir2);

    printf("Syncing %s and %s\n", dir1, dir2);

//    explore_dir_rec(argv[1]);
//    explore_dir_rec(argv[2]);

    return 0;
}
