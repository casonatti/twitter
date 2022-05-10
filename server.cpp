#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define IP_PROTOCOL 0
#define PORT 8080

int main() {
    int server_fd, new_socket, valread;
    char buffer[BUFFER_SIZE] = { 0 };
    struct sockaddr_in address;
    int address_len = sizeof(address);
    int opt = 1;
    const char* hello = "Hello from server!";

    if((server_fd = socket(AF_INET, SOCK_STREAM, IP_PROTOCOL)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //prevents "address already in use"
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*) &address, sizeof(address)) < 0) {
        perror("bind filed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 1) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if((new_socket = accept(server_fd, (struct sockaddr*) &address, (socklen_t*) &address_len)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while(1) {
        memset(buffer, ' ', strlen(buffer));
        valread = read(new_socket, buffer, BUFFER_SIZE);
        printf("%s\n", buffer);

        send(new_socket, hello, strlen(hello), 0);
    }

    return 0;
}