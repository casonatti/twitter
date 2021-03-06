#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include "Notification.hpp"
#include "Packet.hpp"

using namespace std;

#define BUFFER_SIZE 512
#define IP_PROTOCOL 0
#define MAX_THREADS 2
#define PORT 8080

void *serverStuff(void* new_socket) {
    bool flag_quit = false;
    const char* server_ack_message = "ACK";
    const char* server_quit_message = "QUITTING";
    const char* cmd_exit = "EXIT";
    char buffer[BUFFER_SIZE] = { 0 };
    int client_socket;
    int* p = (int*) new_socket;
    int valread;
    string str_buffer;

    client_socket = (int)*p;

    cout << "Socket " << client_socket << " initialized." << endl;

    while(!flag_quit) {
        memset(buffer, ' ', strlen(buffer));
        valread = recv(client_socket, buffer, BUFFER_SIZE, 0);
        str_buffer = buffer;
        std::cout << "[Client (" << client_socket << ")] " << str_buffer << std::endl;
        
        //if client sent "exit", kill this thread
        if(str_buffer.compare(0,4,cmd_exit,0,4) == 0) {
            flag_quit = true;            
        } else {
            send(client_socket, server_ack_message, strlen(server_ack_message), 0);
        }
    }
    send(client_socket, server_quit_message, strlen(server_quit_message), 0);
    cout << "Closing socket " << client_socket << "..." << endl << endl;
    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    int server_fd, new_socket, valread, i = 0;
    char buffer[BUFFER_SIZE] = { 0 };
    struct sockaddr_in address;
    int address_len = sizeof(address);
    int opt = 1;
    pthread_t thread_array[MAX_THREADS];


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

    while(1) {
        //check if there's available space in thread_array
        if(i < MAX_THREADS) {
            //create a new socket
            if((new_socket = accept(server_fd, (struct sockaddr*) &address, (socklen_t*) &address_len)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            } else {
                //new_socket created!
                //create a new thread                
                if(pthread_create(&thread_array[i], NULL, serverStuff, &new_socket)) {
                    perror("thread create error");
                    exit(EXIT_FAILURE);
                }

                i++;    

                cout << i << endl;         
            }
        } //else -> while i => MAX_THREADS -> join()
    }

    return 0;
}