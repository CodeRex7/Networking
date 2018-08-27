#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 4444

int main() {
    int clientSocket, valRead;
    struct sockaddr_in address, serverAddress;
    char buffer[1024] = {'0'};

    // Creating socket file descriptor
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error while creating socket");
        exit(EXIT_FAILURE);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    // Get server details
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // Establish connection
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Error while establishing connection");
        exit(EXIT_FAILURE);
    }

    // Sending message
    char *msg = "This is Aditya";
    send(clientSocket, msg, strlen(msg), 0);
    printf("Message sent\n");

    // Receiving message
    valRead = read(clientSocket, buffer, 1024);
    printf("Message from Server: %s\n", buffer);

    return 0;
}