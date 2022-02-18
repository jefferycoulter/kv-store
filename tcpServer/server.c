#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "utils.h"
//#include "commands.h"

int main(int argc, char** argv)
{
    /* check that arguments passed are appropriate */
    if (argc != 2) { KillErrorUser("usage error", "./server [<port number>]\n"); }

    /* initialize socket attributes */
    int sockfd, newsockfd, portno;
    char buffer[256]; /* for messages */

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len;
    pid_t pid; // for forking to create multiple clients

    /* create socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        KillErrorSyst("error opening socket.\n");
    }

    /* make sure server is cleared before using it */
    memset(&serv_addr, 0, sizeof(serv_addr));
    portno = atoi(argv[1]); /* port number is passed as second argument when running program */

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    /* bind socket file descriptor to socket address */
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        KillErrorSyst("binding failed.\n");
    }

    /* server can accept up to 5 clients */
    if (listen(sockfd, 5) != 0)
    {
        KillErrorSyst("listen failed.\n");
    }

    while(1) /* loop for as long as the server is accepting clients */
    {
        /* have use this apparently for accept calls (TCP/IP Sockets in C by Donahoo and Calvert) */
        cli_len = sizeof(cli_addr);

        if ((newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len)) < 0)
        {
            KillErrorSyst("error on accept.\n");
        }

        printf("new client: %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

        if ((pid = fork()) == 0) /* process for new client */
        {
            close(sockfd);

            while(1) /* loop for as long as the client is connected */
            {
                bzero(buffer, 256);
                recv(newsockfd, buffer, 256, 0);
                if (strncmp(":q", buffer, 2) == 0)
                { /* the client is done */
                    bzero(buffer, sizeof(buffer));
                    strcpy(buffer, "Good bye\r\n");
                    send(newsockfd, buffer, strlen(buffer), 0);
                    printf("client %s:%d disconnected\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
                    break;
                }

                else if (strncmp("set", buffer, 3) == 0)
                { /* the client wants to set a value */
                    char cmd[20] = { 0 }; // store set command
                    char key[20] = { 0 }; // store key
                    char val[20] = { 0 }; // store value
                    bool replace = false;

                    /* get the key-value pair */
                    sscanf(buffer, "%s %s %[a-zA-Z ]", cmd, key, val);

                    /* open old file */
                    FILE *f = fopen("kv-store.txt", "r");

                    /* open replacement file to updating values */
                    FILE *f_temp = fopen("temp.txt", "a");

                    /* make sure files are open */
                    if (f==NULL || f_temp == NULL) { KillErrorSyst("server failed to open file"); }
                    
                    /* write key and value to file */
                    char line[1024]; /* line of old file */
                    char new_line[1024]; /* line that will be added to file */
                    strcpy(new_line, key);
                    strcat(new_line, ": ");
                    strcat(new_line, val);
                    
                    /* check if the key is a new key or already exists */
                    int line_count = 0; /* line at which the key exists */
                    while ((fgets(line, sizeof(line), f)) != NULL)
                    {
                        char* token;
                        char line_copy[1024];
                        strcpy(line_copy, line);
                        token = strtok(line, ":"); /* this should be the key */
                        if (strcmp(token, key) == 0)
                        { /* key already exists, so update it it */
                            replace = true;
                            break;
                        }
                        line_count++;
                    }

                    /* reset the file pointer to the beginning */
                    rewind(f);

                    if (replace==true)
                    { /* the key-value pair is new, so create a new item */
                        int another_line_count = 0; /* when this equals line_count, replace the old value with the new one */
                        while ((fgets(line, sizeof(line), f)) != NULL)
                        {
                            if (line_count==another_line_count)
                            { /* value already exists, so replace it */
                                fputs(new_line, f_temp);
                                fputs("\n",f_temp);
                            }
                            else
                            { /* keep the other key-value pairs as the are */
                                fputs(line, f_temp);
                            }

                            another_line_count++;
                        }
                    }
                    else
                    { /* the key is new, so just add it to the kv-store */
                        fputs(new_line, f_temp);
                        fputs("\n",f_temp);
                        while ((fgets(line, sizeof(line), f)) != NULL)
                        {
                            fputs(line, f_temp);
                        }
                    }

                    /* close files, delete the old file, rename the new file to the old name */
                    fclose(f);
                    fclose(f_temp);
                    remove("kv-store.txt");
                    rename("temp.txt", "kv-store.txt");

                    /* tell the client that the key-value pair has been stored */
                    bzero(buffer, sizeof(buffer));
                    strcpy(buffer, "STORED\r\n");
                    send(newsockfd, buffer, strlen(buffer), 0);

                } /* END the client wants to set a value */

                else if (strncmp("get", buffer, 3) == 0)
                { /* the client wants to get a value */
                    char cmd[20] = { 0 }; // store set command
                    char key[20] = { 0 }; // store key
                    char val[20] = { 0 }; // store value

                    sscanf(buffer, "%s %s", cmd, key);

                    /* open file for reading */
                    FILE *f = fopen("kv-store.txt", "r");
                    if (f == NULL) { KillErrorSyst("server failed to open file"); }

                    char line[1024];
                    while (fgets(line, sizeof(line), f) != NULL)
                    {
                        char* token;
                        char line_copy[1024];
                        strcpy(line_copy, line);
                        token = strtok(line, ":"); /* this should be the key value */
                        if (strcmp(token, key) == 0)
                        {
                            strcpy(val, strtok(NULL, "\n"));
                            break;
                        }
                    }
                
                    /* close file */
                    fclose(f);

                    /* return the key-value pair to the client */
                    bzero(buffer, sizeof(buffer));
                    strcpy(buffer, key);
                    strcat(buffer, " ");
                    strcpy(buffer, val);
                    strcat(buffer, "\r\n");
                    send(newsockfd, buffer, strlen(buffer), 0);

                    /* tell client that all information has been delivered */
                    bzero(buffer, sizeof(buffer));
                    strcpy(buffer, "END\r\n");
                    send(newsockfd, buffer, strlen(buffer), 0);

                } /* END the client wants to get a value */

                else
                { /* the client needs to specify a command */
                    strcpy(buffer, "need to specify either a 'get' or a 'set' command\n usage:\n\t get [<key>]\n\t set [<key>] [<value size>] [<value>]\n");
                    send(newsockfd, buffer, strlen(buffer), 0);
                }

            } /* END loop for as long as the client is connected */

        } /* END process for new client */

    } /* END loop for as long as the server is accepting clients */

    close(newsockfd);
    //close(sockfd);

    return 0;
}
