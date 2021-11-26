#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        error("Error opening socket\n");
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Error, no such host\n");
        exit(0);
    }

    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR connecting");
    }

    // Receive key from server
    char key[2][256];
    n = read(sockfd, key, 512);

    printf("Key: ");
    for(int i=0;i<2;i++) {
        printf("%s ", key[i]);
    }
    printf("\n");

    // Send encrypted message to the server
    char encrypted_message[256] = "KACWNODLEMGETEN";
    n = write(sockfd, encrypted_message, 256);
    printf("Encrypted message: %s\n", encrypted_message);

    // Read decrypted message from the server
    char decrypted_message[256] = "";
    n = read(sockfd, decrypted_message, 256);
    printf("Decrypted message: %s\n", decrypted_message);

    return 0;
}
