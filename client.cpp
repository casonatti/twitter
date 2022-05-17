#include <csignal>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include "Notification.hpp"
#include "Packet.hpp"

using namespace std;

#define BUFFER_SIZE 512
#define CMD_SEND 1
#define CMD_FOLLOW 2
#define CMD_EXIT 3
#define IP_PROTOCOL 0
#define MAX_COMMAND_LENGTH 6
#define MESSAGE_LENGTH 128
#define PORT 8080
#define STRING_QUIT "QUITTING"
#define WHITESPACE 32

//global var
int sock = 0;
int global_notification_id = 0;

void signalHandler(int signum) {
    const char* exit_message = "EXIT SOMETHING GONE WRONG WITH ME... CYA!";
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
int getCommand(char user_input[]) {
    string command;
    stringstream ss (user_input);
    const char* cmd_exit ("EXIT");
    const char* cmd_follow ("FOLLOW");
    const char* cmd_send ("SEND");

    //it works like a split
    getline(ss, command, ' ');

    if(command.compare(cmd_send) == 0)
        return CMD_SEND; 
    if(command.compare(cmd_follow) == 0)
        return CMD_FOLLOW;
    if(command.compare(cmd_exit) == 0)
        return CMD_EXIT;

    return 0;
}

int main(int argc, char** argv) {
    bool flag_quit = false;
    char in[MESSAGE_LENGTH] = { 0 };
    const char* packet_serialized;
    const char* notification_serialized;
    int valread, command = -1;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = { 0 };
    string str_aux;
    string str_notification_serial ("");
    string str_packet_serial ("");
    Notification notification;
    Packet pack;
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
            //serialization strings reset
            str_notification_serial = "";
            str_packet_serial = "";

            //fill notification object
            notification.setId(global_notification_id);
            notification.incNotificationId(&global_notification_id);
            notification.setTimestamp(1);
            notification.setMessage(in);
            notification.setLength();

            //serializing notification
            notification.serialize(&str_notification_serial);

            //fill packet object
            pack.setType(1);
            pack.setSeqn(1);
            pack.setTimestamp(1);
            pack.setLength(static_cast<unsigned int>(sizeof(notification))); //casts size_t to uint
            pack.setPayload(str_notification_serial.c_str());
            
            //serialize packet objct
            pack.serialize(&str_packet_serial);

            //convert the string to const char*
            packet_serialized = str_packet_serial.c_str();

            //send the packet
            cout << "sending message to server: \"" << packet_serialized << "\"" << endl;
            send(sock, packet_serialized, strlen(packet_serialized), 0);
            
            //receive a packet
            valread = recv(sock, buffer, sizeof(buffer), 0);
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