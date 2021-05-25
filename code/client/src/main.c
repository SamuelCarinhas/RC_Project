#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include "config/config.h"
#include "utils/functions.h"

char end_server[100];
int server_socket, recv_len;
struct sockaddr_in addr;
struct hostent * host_ptr;
socklen_t slen = sizeof(struct sockaddr_in);

void options();
void send_msg();
void send_2p2();
void multicast();

int main(int argc, char *argv[]) {
    
    if(argc != 3)
        error("Invalid usage: Use ./client <HOST> <PORT>\n");

    strcpy(end_server, argv[1]);

    if((host_ptr = gethostbyname(end_server)) == 0)
        error("Could not obtain host\n");
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ((struct in_addr *)(host_ptr->h_addr))->s_addr;
    addr.sin_port = htons((short) atoi(argv[2]));

    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        error("Error opening socket\n");

    if(connect(server_socket, (struct sockaddr*)&addr, slen) == -1)
        error("Error conecting to server\n");


    char username[USERNAME_SIZE], password[PASSWORD_SIZE];

    while(1) {
        printf("Login menu:\n");
        printf("Username: ");
        scanf("%s", username);
        printf("Password: ");
        scanf("%s", password);

        char message[BUFFER_SIZE];
        snprintf(message, BUFFER_SIZE, "LOGIN %s %s", username, password);

        sendto(server_socket, message, BUFFER_SIZE, 0, (struct sockaddr *) &addr, (socklen_t) slen);

        int n = recvfrom(server_socket, message, BUFFER_SIZE, 0, (struct sockaddr *) &addr, &slen);
        if(n <= 0) {
            printf("Error contacting the server...\n");
            continue;
        }
        printf("%s", message);

        if(strcmp(message, "Login successfully\n") == 0) {
            break;
        }
    }

    options();

    close(server_socket);

    return 0;
}

void send_msg(){
    char username[USERNAME_SIZE], message[100], msg[BUFFER_SIZE];
    printf("Send message to: ");
    scanf("%s", username);
    username[strcspn(username, "\n")] = '\0';
    
    while(fgetc(stdin) != '\n');

    printf("Message: ");
    scanf("%[^\n]s", message);
    snprintf(msg, BUFFER_SIZE, "MSG %s %s", username, message);

    sendto(server_socket, msg, BUFFER_SIZE, 0, (struct sockaddr *) &addr, slen);

    char response[BUFFER_SIZE];
    int n = recvfrom(server_socket, response, BUFFER_SIZE, 0, (struct sockaddr *) &addr, &slen);
    if(n <= 0) {
        printf("Error contacting the server...\n");
    }

    printf("%s", response);
}

void send_2p2(){
    char username[BUFFER_SIZE], message[100], msg[BUFFER_SIZE];
    printf("Send message to: ");
    scanf("%s", username);
    username[strcspn(username, "\n")] = '\0';

    printf("Message: ");
    scanf("%s", message);
    snprintf(msg, BUFFER_SIZE, "MSG %s %s", username, message);

    char response[BUFFER_SIZE];
    int n = recvfrom(server_socket, response, BUFFER_SIZE, 0, (struct sockaddr *) &addr, &slen);
    if(n <= 0) {
        printf("Error contacting the server...\n");
    }
}

void multicast(){

}

void options() {
    int option;

    printf("1 - Send message via Server\n");
    printf("2 - Send message via 2P2\n");
    printf("3 - Send group message\n");
    printf("4 - Exit\n");

    do{
        scanf("%d", &option);
    } while (1 > option || option > 4);
    
    switch(option){
        case 1:
            send_msg();
            break;
        case 2:
            send_2p2();
            break;
        case 3:
            multicast();
            break;
        case 4:
            printf("Leaving\n");
            close(server_socket);
            break;
    }
}
