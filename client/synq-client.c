#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../common/protocol.h"
#include "../common/utils.h"
#include "../common/linked_list.h"

int local_sync(char dir1[PATH_MAX], char dir2[PATH_MAX]) {
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

    destroy(l1);
    destroy(l2);
    destroy(diff);

    return 0;
}

void send_data(int sock, TLV *tlv) {
    //char buffer[512];

    printf("version %d\n", tlv->value.tlv_connect.version);
    printf("magic %d\n", tlv->value.tlv_connect.magic);
    write(sock, tlv, sizeof(tlv));

    //on attend la reponse du serveur
    //read(sock, buffer, 255);
    //printf("message recu : %s", buffer);
}

int
tlv_connect(int sock) {
    TLV *tlv_c = malloc(sizeof(TLV));
    TLV *tlv_s = malloc(sizeof(TLV));
    init_tlv_connect(tlv_c);

    write(sock, tlv_c, sizeof(tlv_c));
    read(sock, tlv_s, sizeof(tlv_s));

    return 0;
}

int remote_sync(char dir[PATH_MAX], char *ip, uint16_t port)
{
    int sockfd;
    List *local_list = init();
    List *remote_list = init();
    struct sockaddr_in server;
    socklen_t serverlen;
    char buffer[255];

    explore_dir_rec(local_list, dir, NULL);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        perror("ERROR opening socket");

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;

    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(port);

    serverlen = sizeof(server);
    if( (connect(sockfd, (struct sockaddr *) &server, serverlen)) < 0 )
        perror("ERROR connect");

    inet_ntop(AF_INET, &(server.sin_addr), buffer, INET_ADDRSTRLEN);
    printf("Connected to : %s | Port: %d\n", buffer, ntohs(server.sin_port));

    tlv_connect(sockfd);

    TLV *tlv = malloc(sizeof(TLV));

    init_tlv_ask_files(tlv);
    write(sockfd, tlv, sizeof(tlv));
    read(sockfd, tlv, sizeof(tlv));
    printf("TYPE %d\n", tlv->tl.type);
    int entries = tlv->value.tlv_entries.entries;
    printf("ENTRIES %d\n", entries);

    int i =0;
    char test[PATH_MAX];

    for(i=0; i<entries; i++) {
        read(sockfd, tlv, sizeof(TLV));
        printf("%s\n", tlv->value.tlv_entry.filename);
        insert(remote_list, tlv->value.tlv_entry.filename, tlv->value.tlv_entry.mtime);
    }

    List *diff = compareLists(remote_list, local_list);
    printList(diff);

    File *current = diff->head;
    while (current != NULL)
    {
        char file[PATH_MAX];
        snprintf (file, PATH_MAX, "%s/%s", dir, current->path);

        init_tlv_ask_file(tlv, current->path);
        write(sockfd, tlv, sizeof(TLV));
        read(sockfd, tlv, sizeof(TLV));

        printf("DOWNLOADING %s\n", current->path);
        download(sockfd, file, tlv->value.tlv_meta_file.mtime,  tlv->value.tlv_meta_file.mode, tlv->value.tlv_meta_file.size);
        printf("DOWNLOAD FINISHED\n");
        current = current->next;
    }

    shutdown(sockfd, SHUT_RDWR);
    sleep(1);
    close(sockfd);

    return 0;
}

int
main(int argc, char **argv)
{
    int dry_run = 0;
    int c;
    char dir1[PATH_MAX];
    char dir2[PATH_MAX];

    if(argc < 3 || argc > 5) {
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

    size_t length = strlen(dir1);
    if(dir1[length-1] != '/') {
        strcat(dir1, "/");
    }

    if(check_dir_exist(dir2) == 1) {
        //printf("%s is not a directory\n", dir2);
        int rc = inet_addr(dir2);
        if(rc == INADDR_NONE) {
            printf("%s is not an INET address, exiting...\n", dir2);
            exit(EXIT_FAILURE);
        }
        if(argc < 4)  {
            printf("No port specified, exiting...\n");
            exit(EXIT_FAILURE);
        }
        int port = atoi(argv[optind + 2]);
        if(port == 0) {
            printf("Invalid port, exiting...\n");
            exit(EXIT_FAILURE);
        }

        printf("Syncing %s with %s:\n", dir1, dir2);
        remote_sync(dir1, dir2, port);
        exit(EXIT_FAILURE);
    }

    length = strlen(dir2);
    if(dir2[length-1] != '/') {
        strcat(dir2, "/");
    }

    printf("Syncing %s with %s\n", dir1, dir2);
    local_sync(dir1, dir2);

    return 0;
}
