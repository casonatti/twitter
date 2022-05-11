#include <csignal>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

using namespace std;

#define BUFFER_SIZE 512
#define IP_PROTOCOL 0
#define MAX_COMMAND_LENGTH 6
#define MESSAGE_LENGTH 128
#define PORT 8080
#define STRING_QUIT "QUITTING"
#define WHITESPACE 32

//global var
int sock = 0;


void signalHandler(int signum) {
    const char* exit_message = "quit SOMETHING GONE WRONG WITH ME... CYA!";
    char buffer[BUFFER_SIZE] = { 0 };

    memset(buffer, ' ', strlen(buffer));
        
    cout << "sending message to server: \"" << exit_message << "\"" << endl;
    send(sock, exit_message, strlen(exit_message), 0);
        
    recv(sock, buffer, BUFFER_SIZE, 0);
    cout << "received message from server: \"" << buffer << "\"" << endl << endl;

    close(sock);

    exit(signum);
}

//it's being implemented on the client side to don't overcharge server side with "bad commands"
int getCommand(char aux[]) {
    string command = "";
    const char* cmd_follow ("FOLLOW");
    const char* cmd_send ("SEND");
    for(int i = 0; i < MAX_COMMAND_LENGTH; i++) {
        if(aux[i] == WHITESPACE) //stop for loop.
            break;
        command += aux[i];
    }

    if((command.compare(cmd_send) == 0) || (command.compare(cmd_follow) == 0)) //is it the best way? xD
        return 1;

    return -1;
}

int main(int argc, char** argv) {
    bool flag_quit = false;
    char in[MESSAGE_LENGTH] = { 0 };
    int valread, command = -1;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = { 0 };
    string str_aux;
    signal(SIGINT, signalHandler);
    signal(SIGHUP, signalHandler);

    if((sock = socket(AF_INET, SOCK_STREAM, IP_PROTOCOL)) <  0) {
        printf("Socket creation error");
        return -1;
    }

    cout << sock << endl;

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
    
    while(!flag_quit) {
        //buffer reset
        memset(buffer, ' ', strlen(buffer));
        
        cout << "msg: ";
        cin.getline(in,MESSAGE_LENGTH);
        command = getCommand(in);

        if(command > 0) {
            cout << "sending message to server: \"" << in << "\"" << endl;
            send(sock, in, strlen(in), 0);
            
            valread = recv(sock, buffer, BUFFER_SIZE, 0);
            cout << "received message from server: \"" << buffer << "\"" << endl << endl;

            str_aux = buffer;

            if(str_aux.compare(0,4,STRING_QUIT,0,4) == 0) {
                flag_quit = true;
            }
        } else {
            cout << "Error: invalid command." << endl << endl;
        }
    }

    close(sock);

    cout << "Connection is closed." << endl << endl;

    return 0;
}