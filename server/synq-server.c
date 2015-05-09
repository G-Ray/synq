#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>

#include "../common/protocol.h"
#include "../common/utils.h"
#include "../common/linked_list.h"

#define PORT 8080

char * direct="/home/geoffrey/server/";

void echo(int clientfd) {
    TLV *tlv = malloc(sizeof(TLV));

    read(clientfd, tlv, sizeof(TLV));
    printf("Message recu de client %d : %d\n", clientfd, tlv->tl.type);
    printf("length %d\n",tlv->tl.length);
    printf("version %d\n", tlv->value.tlv_connect.version);
    printf("magic %d\n", tlv->value.tlv_connect.magic);
    //write(clientfd, buffer, 255);
}

int tlv_connect(int clientfd) {
    TLV *tlv = malloc(sizeof(TLV));

    read(clientfd, tlv, sizeof(TLV));

    if(tlv->tl.type != TLV_CONNECT_TYPE) {
        shutdown(clientfd, SHUT_RDWR);
        sleep(1);
        close(clientfd);
        exit(0);
    }

    TLV *tlv_c = malloc(sizeof(TLV));
    init_tlv_connect(tlv_c);
    write(clientfd, tlv_c, sizeof(tlv_c));

    printf("TLV_CONNECT OK\n");
    return 0;
}

int tlv_receive(int clientfd) {
    struct stat st;
    int rc;
    char filename[PATH_MAX];
    TLV *tlv = malloc(sizeof(TLV));
    read(clientfd, tlv, sizeof(TLV));

    switch(tlv->tl.type) {
        case 2:
            printf("TYPE 2");
            List *l1 = init();
            explore_dir_rec(l1, direct, NULL);
            int entries = countEntries(l1);
            printf("ENTRIES: %d\n", entries);
            init_tlv_entries(tlv, entries);
            write(clientfd, tlv, sizeof(tlv));

            File *current = l1->head;
            while (current != NULL)
            {
                init_tlv_entry(tlv, current->mtime, 50, current->path);
                write(clientfd, tlv, sizeof(TLV));
                printf("ENTRY SENT %s\n",  tlv->value.tlv_entry.filename);
                current = current->next;
            }
            break;

        case 3: printf("TYPE 3\n"); break;
        case 4: printf("TYPE 4\n"); break;
        case 5:
            printf("TYPE 5\n");
            printf("requested file: %s", tlv->value.tlv_entry.filename);
            snprintf (filename, PATH_MAX, "%s/%s", direct, tlv->value.tlv_entry.filename);
            rc = stat(filename, &st);
            if(rc != 0) {
                perror("Fichier inexistant");
            }
            else {
                init_tlv_meta_file(tlv, st.st_mtime, st.st_size, st.st_mode,
                                        tlv->value.tlv_entry.filename);
                write(clientfd, tlv, sizeof(TLV));
                int fd_from = open(filename, O_RDONLY);
                int nread;
                int nwritten;
                char buf[4096];

                while (nread = read(fd_from, buf, sizeof buf), nread > 0)
                {
                    char *out_ptr = buf;
                    ssize_t nwritten;

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
                    } while (nread > 0);
                    close(fd_from);
                }
            }
            break;
        case 6:
            printf("TYPE 6\n");
            break;

        default: return 1;
    }

    return 0;
}

int
main(int argc, char **argv)
{
    int sockfd;
    int clientfd;
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t clilen;
    char buffer[255];

    /************************/

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        perror("ERROR opening socket");

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if(bind(sockfd, (struct sockaddr *) &server, sizeof(server)) < 0)
        perror("ERROR on binding");

    listen(sockfd, 128);
    clilen = sizeof(client);

    while(1) {
        printf("Server is running...\n");
        if( (clientfd = accept(sockfd, (struct sockaddr *) &client, &clilen)) <0 )
            perror("ERROR accept");

        printf("Connection recue sur socket %d\n", sockfd);
        inet_ntop(AF_INET, &(client.sin_addr), buffer, INET_ADDRSTRLEN);
        printf("IP: %s | Port: %d\n", buffer, ntohs(client.sin_port));
        printf("\n Client %d\n", clientfd);

        if (!fork()) { //fils
            close(sockfd);

            tlv_connect(clientfd);
            tlv_receive(clientfd);

            tlv_receive(clientfd);

            //echo(clientfd);
            shutdown(clientfd, SHUT_RDWR);
            sleep(1);
            close(clientfd);
            exit(0);
        }
        //si erreur...

        close(clientfd);
    }

    return 0;
}
