#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

using namespace std;

#define BUFFER_SIZE 1024
#define IP_PROTOCOL 0
#define MESSAGE_LENGTH 128
#define PORT 8080

int main() {
    char in[MESSAGE_LENGTH] = { 0 };
    int sock = 0, valread;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = { 0 };

    if((sock = socket(AF_INET, SOCK_STREAM, IP_PROTOCOL)) <  0) {
        printf("Socket creation error");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        printf("INET conversion error");
        return -1;
    }

    if(connect(sock, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        printf("Connection failed");
        return -1;
    }
    
    while(1) {
        //buffer reset
        memset(buffer, ' ', strlen(buffer));
        
        cout << "msg: ";
        cin.getline(in,MESSAGE_LENGTH);
        cout << "sending message to server: \"" << in << "\"" << endl;
        send(sock, in, strlen(in), 0);
        
        valread = read(sock, buffer, BUFFER_SIZE);
        cout << "received message from server: \"" << buffer << "\"" << endl << endl;
    }

    return 0;
}