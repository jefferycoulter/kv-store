#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <stdarg.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#include "utils.h"

int main(int argc, char** argv)
{
    /* check that arguments are appropriate */
    if ((argc < 3) || argc > 4) { KillErrorUser("usage error:", "./client [<host IP>] [<port number>] [<input file>]\n"); }

    /* initialize socket attributes */
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    portno = atoi(argv[2]);
    
    /* create socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) { KillErrorSyst("error opening socket.\n"); }

    /* check that server exists */
    if ((server = gethostbyname(argv[1])) == NULL) { KillErrorSyst("error no such host\n"); }

    /* make sure server is cleared before using it */
    //bzero((char*)&serv_addr, sizeof(serv_addr));
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    /* copy bytes from server to serv_addr */
    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(portno);

    /* connect to server */
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        KillErrorSyst("error connection failed\n");
    }

    if (argc == 4)
    { /* input file provided */

        FILE *f = fopen(argv[3], "r");
        if (f==NULL) { KillErrorSyst("server failed to open file"); }
        char line[1024]; /* line of old file */
        
        /* check if the key is a new key or already exists */
        while ((fgets(line, sizeof(line), f)) != NULL)
        {
            //printf("line %s\n", line);
            bzero(buffer, 256);
            strcpy(buffer, line);
            send(sockfd, buffer, strlen(buffer), 0);
            
            bzero(buffer, 256);
            fputs("server: ", stdout);
            recv(sockfd, buffer, 256, 0);
            fputs(buffer, stdout);
        }

        fclose(f);
    } /* END file input */
    else
    {
        while(1)
        {
            /* no input file provided */
            bzero(buffer, 256);
            fgets(buffer, 256, stdin);
            n = write(sockfd, buffer, strlen(buffer));
            if (n < 0) { KillErrorSyst("error on writing.\n"); }

            bzero(buffer, 256);
            n = read(sockfd, buffer, 256); /* strlen(buffer) is zero at this point so set size to 256 */
            if (n < 0) { KillErrorSyst("error on reading.\n"); }
            printf("server: %s\n", buffer);

            /* when server says bye, leave the loop */
            int i = strncmp("Good bye", buffer, 8);
            if (i == 0) { break; }
        } /* END while connected */
    } /* END no file input */

    close(sockfd);

    return 0;
}
