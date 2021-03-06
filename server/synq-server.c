#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>

//OpenSSL
#include "../common/ssl.h"

#include "../common/protocol.h"
#include "../common/utils.h"
#include "../common/linked_list.h"

char dir[PATH_MAX];

int tlv_connect(SSL *ssl) {
    TLV *tlv = malloc(sizeof(TLV));

    SSL_read(ssl, tlv, sizeof(TLV));

    if(tlv->tl.type != TLV_CONNECT_TYPE) {
        int clientfd = SSL_get_fd(ssl);
        shutdown(clientfd, SHUT_RDWR);
        SSL_free(ssl);
        sleep(1);
        close(clientfd);
        exit(0);
    }

    TLV *tlv_c = malloc(sizeof(TLV));
    init_tlv_connect(tlv_c);
    SSL_write(ssl, tlv_c, sizeof(tlv_c));

    printf("TLV_CONNECT OK\n");
    return 0;
}

int tlv_receive(SSL *ssl) {
    struct stat st;
    int rc;
    char filename[PATH_MAX];
    TLV *tlv = malloc(sizeof(TLV));
    SSL_read(ssl, tlv, sizeof(TLV));
    char file[PATH_MAX];

    switch(tlv->tl.type) {
        case 2:
            printf("TYPE 2\n");
            List *l1 = init();
            explore_dir_rec(l1, dir, NULL);
            int entries = countEntries(l1);
            printf("ENTRIES: %d\n", entries);
            init_tlv_entries(tlv, entries);
            SSL_write(ssl, tlv, sizeof(tlv));

            File *current = l1->head;
            while (current != NULL)
            {
                snprintf (filename, PATH_MAX, "%s/%s", dir, current->path);
                rc = stat(filename, &st);
                if(rc != 0) {
                    perror("Fichier inexistant");
                }
                init_tlv_entry(tlv, current->mtime, st.st_size, current->path);
                SSL_write(ssl, tlv, sizeof(TLV));
                printf("ENTRY SENT %s\n",  tlv->value.tlv_entry.filename);
                current = current->next;
            }
            break;

        case 3: printf("TYPE 3\n"); break;
        case 4: printf("TYPE 4\n"); break;
        case 5:
            printf("TYPE 5\n");
            //printf("requested file: %s\n", tlv->value.tlv_entry.filename);
            snprintf (filename, PATH_MAX, "%s/%s", dir, tlv->value.tlv_entry.filename);
            rc = stat(filename, &st);
            if(rc != 0) {
                perror("Fichier inexistant");
            }
            else {
                init_tlv_meta_file(tlv, st.st_mtime, st.st_size, st.st_mode,
                                        tlv->value.tlv_entry.filename);
                SSL_write(ssl, tlv, sizeof(TLV));
                upload(ssl, filename);
            }
            break;
        case 6:
            printf("TYPE 6\n");
            snprintf (file, PATH_MAX, "%s/%s", dir, tlv->value.tlv_meta_file.filename);
            download(ssl, file, tlv->value.tlv_meta_file.mtime,  tlv->value.tlv_meta_file.mode, tlv->value.tlv_meta_file.size);
            break;
        case 7:
            printf("TYPE 7\n");
            snprintf (file, PATH_MAX, "%s/%s", dir, tlv->value.tlv_entry.filename);
            if(unlink(file) < 0)
                perror("unlink");

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
    int port;
    SSL_CTX *ctx;

    if(argc != 3) {
        printf("Usage: %s <dir1> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    port = atoi(argv[2]);
    if(port == 0) {
        printf("Invalid port, exiting...\n");
        exit(EXIT_FAILURE);
    }
    strncpy(dir, argv[1], PATH_MAX);

    if(check_dir_exist(dir) == 1) {
        printf("%s is not a directory\n", dir);
        exit(EXIT_FAILURE);
    }

    /************************/
    SSL_library_init();
    ctx = InitServerCTX();
    LoadCertificates(ctx, "mycert.pem", "mycert.pem");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        perror("ERROR opening socket");

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if(bind(sockfd, (struct sockaddr *) &server, sizeof(server)) < 0)
        perror("ERROR on binding");

    listen(sockfd, 128);
    clilen = sizeof(client);

    while(1) {
        printf("Server is running...\n");
        SSL *ssl;

        if( (clientfd = accept(sockfd, (struct sockaddr *) &client, &clilen)) <0 )
            perror("ERROR accept");

        printf("Connection recue sur socket %d\n", sockfd);
        ssl = SSL_new(ctx);              /* get new SSL state with context */
        SSL_set_fd(ssl, clientfd);      /* set connection socket to SSL state */
        //Servlet(ssl);

        inet_ntop(AF_INET, &(client.sin_addr), buffer, INET_ADDRSTRLEN);
        printf("IP: %s | Port: %d\n", buffer, ntohs(client.sin_port));
        printf("\n Client %d\n", clientfd);

        if (!fork()) { //fils
            close(sockfd);

            if ( SSL_accept(ssl)< 0) {
                perror("SSL accept");
            }

            tlv_connect(ssl);

            while(tlv_receive(ssl) != 1);

            shutdown(clientfd, SHUT_RDWR);
            SSL_free(ssl);
            sleep(1);
            close(clientfd);
            printf("CONNECTION CLOSED");
            exit(0);
        }

        close(clientfd);
        SSL_CTX_free(ctx);
    }

    return 0;
}
