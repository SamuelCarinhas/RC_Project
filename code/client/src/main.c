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
#include <arpa/inet.h>

char end_server[100];
int sock;
int server_socket, recv_len;
struct sockaddr_in addr, self_addr;
struct hostent * host_ptr;
socklen_t slen = sizeof(struct sockaddr_in);

void options();
void send_msg();
void send_p2p();
void multicast();

pthread_t thread;

int message_port = 9000;

void * read_msg();

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
    
    pthread_create(&thread, NULL, &read_msg, NULL);

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
            n = recvfrom(server_socket, message, BUFFER_SIZE, 0, (struct sockaddr *) &addr, &slen);
            if(n <= 0) {
                printf("Error contacting the server...\n");
                continue;
            }

            printf("%s", message);
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

void send_p2p(){
    char username[USERNAME_SIZE], message[BUFFER_SIZE], msg[BUFFER_SIZE];
    printf("Send message to: ");
    scanf("%s", username);
    username[strcspn(username, "\n")] = '\0';

    snprintf(msg, BUFFER_SIZE, "P2P %s", username);

    sendto(server_socket, msg, BUFFER_SIZE, 0, (struct sockaddr *) &addr, slen);

    char response[BUFFER_SIZE];
    int res = recvfrom(server_socket, response, BUFFER_SIZE, 0, (struct sockaddr *) &addr, &slen);
    if(res <= 0) {
        printf("Error contacting the server...\n");
    }

    printf("%s", response);

    char ip[INET_ADDRSTRLEN];
    int port;

    res = sscanf(response, "%s IP: %s Port: %d", username, ip, &port);

    if(res != 3) {
        printf("Something went wrong...\n");
        return;
    }

    while(fgetc(stdin) != '\n');
    
    printf("Message: ");
    scanf("%[^\n]s", message);

    int target_socket;
    struct sockaddr_in target_addr;
    struct hostent * target_host;
    socklen_t len = sizeof(struct sockaddr_in);

    if((target_host = gethostbyname(ip)) == 0) {
        printf("Invalid USER ip\n");
        return;
    }

    target_addr.sin_family = AF_INET;
    target_addr.sin_addr.s_addr = ((struct in_addr *)(host_ptr->h_addr))->s_addr;
    target_addr.sin_port = htons(message_port);

    if ((target_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Error opening USER socket\n");
        return;
    }

    if(connect(target_socket, (struct sockaddr*)&target_addr, len) < 0) {
        error("Error conecting to USER\n");
        return;
    }
    
    
    send(target_socket, message, BUFFER_SIZE, 0);

    printf("Message sent to %s:%d\n", ip, target_addr.sin_port);

}

void multicast(){

}

void options() {
    int option;
    do {
        printf("Menu:\n");
        printf("\t1 - Send message via Server\n");
        printf("\t2 - Send message via P2P\n");
        printf("\t3 - Send group message\n");
        printf("\t0 - Exit\n");
        printf("Choose an option:\n");
        scanf("%d", &option);
        switch(option){
            case 1:
                send_msg();
                break;
            case 2:
                send_p2p();
                break;
            case 3:
                multicast();
                break;
            case 0:
                printf("Leaving\n");
                close(server_socket);
                break;
        }
    } while (option != 0);
    
}

void * read_msg() {
    char buffer[BUFFER_SIZE];

    struct sockaddr_in message_addr;

    message_addr.sin_family = AF_INET;
    message_addr.sin_addr.s_addr = INADDR_ANY;
    message_addr.sin_port = htons(message_port);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

	if(sock < 0)
        error("Couldn't create socket\n");

    socklen_t len = sizeof(struct sockaddr_in);

	if (bind(sock, (struct sockaddr *) &message_addr, sizeof(struct sockaddr_in)))
		error("%s\n", "Couldn't bind message socket.\n");

    char ip[INET_ADDRSTRLEN];

    while(1) {
        int valread = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &message_addr, &len);
        inet_ntop(AF_INET, &message_addr.sin_addr, ip, INET_ADDRSTRLEN);
        if(valread <= 0) {
            printf("..\n");
            continue;
        }
        printf("Private message received from %s: %s\n", ip, buffer);
    }
}
