/* A simple server in the internet domain using TCP the port number is passed as an argument */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

char key[5][5];

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    char ch = 'A';
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (ch == 'J')
            {
                ch = 'K';
            }
            key[i][j] = ch++;
            printf("%c", key[i][j]);
        }
        printf("\n");
    }

    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }

    //   bzero((char * ) & serv_addr, sizeof(serv_addr));

    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR on binding");
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0)
    {
        error("ERROR on accept");
    }

    //   bzero(buffer, 256);

    n = read(newsockfd, buffer, 255);

    if (n < 0)
    {
        error("ERROR reading from socket");
    }

    printf("From client: %s\n", buffer);

    for (int k = 0; k < strlen(buffer) - 1; k += 2)
    {
        int row = buffer[k] - '0' - 1;
        int col = buffer[k + 1] - '0' - 1;
        printf("%c", key[row][col]);
    }
    printf("%d", strlen(buffer));

    printf("Type to client: ");

    fgets(buffer, 255, stdin);

    n = write(newsockfd, buffer, strlen(buffer));

    if (n < 0)
    {
        error("ERROR writing to socket");
    }
    return 0;
}
