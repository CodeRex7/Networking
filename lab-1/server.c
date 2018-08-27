#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 4444

int main() {
    int serverSocket, clientSocket, valRead;
    struct sockaddr_in address;
    int addrLen = sizeof(address);
    int opt = 1;
    char buffer[1024] = {'0'};

    // Creating socket file descriptor
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Error while creating socket");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 4444 - helps in reusing port 4444
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Error while reusing address and port");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    // Attaching socket to the port 4444
    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address))) {
        perror("Error while binding");
        exit(EXIT_FAILURE);
    }

    // Listen on the socket created
    if (listen(serverSocket, 3) < 0) {
        perror("Error while listening");
        exit(EXIT_FAILURE);
    }

    printf("Server started\n");

    // Accept client
    if ((clientSocket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t *)&addrLen)) < 0) {
        perror("Error while accepting");
        exit(EXIT_FAILURE);
    }

    // Receive message
    valRead = read(clientSocket, buffer, 1024);
    printf("Message from Client: %s\n", buffer);

    // Send message
    char *msg = "Hello I am the server";
    send(clientSocket, msg, strlen(msg), 0);
    printf("Message sent\n");

    return 0;
}