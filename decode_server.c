#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
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

    // Send key to the client
    char key[2][256] = {"3","312"};
    n = write(newsockfd, key, 512);

    // Receive encrypted message from the client
    char encrypted_message[256];
    n = read(newsockfd, encrypted_message, 256);
    printf("Encrypted message: %s\n", encrypted_message);

    // Decryption logic
    char decrypted_message[256] = "";
    char temp[4] = "   ";
    for(int i=0;i<strlen(encrypted_message);) {
        for(int j=0;j<3;j++) {
            temp[key[1][j] - '0' - 1] = encrypted_message[i++];
        }
        strcat(decrypted_message, temp);
    }
    
    // Send decrypted message to client
    n = write(newsockfd, decrypted_message, 256);

    if (n < 0)
    {
        error("ERROR writing to socket");
    }
    return 0;
}
