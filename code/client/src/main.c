/**
 * Project: RC 2021
 * 
 * Authors:
 *  Samuel dos Santos Carinhas - 2019217199
 *  Carlos Eduardo da Costa Jordão - 2019221373
 * 
 */

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
#include <netinet/in.h>
#include <netinet/ip.h>

char end_server[100], current_group[GROUP_SIZE];
struct sockaddr_in server_addr, multicast_addr;
socklen_t slen = sizeof(struct sockaddr_in);

void options();
void send_msg();
void send_p2p();
void create_group_multicast();
void connect_multicast();
void send_multicast();
void authenticate();
pthread_t message_thread, multicast_thread;

int message_port;

int server_socket, multicast_write_socket, multicast_read_socket, p2p_socket;

unsigned long multicast_ip = INVALID_GROUP;

void * read_msg();
void * read_multicast();

char username[USERNAME_SIZE], password[PASSWORD_SIZE];

int main(int argc, char *argv[]) {
    
    if(argc != 3)
        error("Invalid usage: Use ./client <HOST> <PORT>\n");

    // START Server

    strcpy(end_server, argv[1]);

    struct hostent * host_ptr;
    if((host_ptr = gethostbyname(end_server)) == 0)
        error("Could not obtain host\n");
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = ((struct in_addr *)(host_ptr->h_addr))->s_addr;
    server_addr.sin_port = htons((short) atoi(argv[2]));

    server_socket = create_socket();

    // END Server

    // P2P Start

    p2p_socket = create_socket();

    // P2P End

    // START multicast
    multicast_write_socket = create_socket();
    multicast_read_socket = create_socket();
    
    int false = 0;
    int res = setsockopt(multicast_write_socket, IPPROTO_IP, IP_MULTICAST_LOOP, &false, sizeof(int));
    if(res < 0)
        error("Couldn't update multicast writing socket. (LOOP)\n");
    
    int ttl = 255;
    res = setsockopt(multicast_write_socket, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(int));
    if(res < 0)
        error("Couldn't update multicast writing socket. (TTL)\n");

    // END multicast

    pthread_create(&multicast_thread, NULL, read_multicast, NULL);

    authenticate();

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

    sendto(server_socket, msg, BUFFER_SIZE, 0, (struct sockaddr *) &server_addr, slen);

    int res = recvfrom(server_socket, message, BUFFER_SIZE, 0, (struct sockaddr *) &server_addr, &slen);
    if(res <= 0)
        error("Lost connection with the server...\n");

    printf("%s", message);
}

void send_p2p(){
    char username[USERNAME_SIZE], message[BUFFER_SIZE], msg[BUFFER_SIZE];
    printf("Send message to: ");
    scanf("%s", username);
    username[strcspn(username, "\n")] = '\0';

    snprintf(msg, BUFFER_SIZE, "P2P %s", username);

    sendto(server_socket, msg, BUFFER_SIZE, 0, (struct sockaddr *) &server_addr, slen);

    char response[BUFFER_SIZE];
    int res = recvfrom(server_socket, response, BUFFER_SIZE, 0, (struct sockaddr *) &server_addr, &slen);
    if(res <= 0) {
        printf("Error contacting the server...\n");
    }

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

    struct sockaddr_in target_addr;
    socklen_t len = sizeof(struct sockaddr_in);

    printf("Send to %s:%d\n", ip, port);

    target_addr.sin_family = AF_INET;
    target_addr.sin_addr.s_addr = inet_addr(ip);
    target_addr.sin_port = htons(port);

    
    sendto(p2p_socket, message, BUFFER_SIZE, 0, (struct sockaddr *) &target_addr, (socklen_t) len);
}


// CREATEMULTICAST GROUP
void create_group_multicast() {
    char group_name[16], message[BUFFER_SIZE + 17];

    printf("Create group name: ");
    while(fgetc(stdin) != '\n');
    
    scanf("%[^\n]s", group_name);

    snprintf(message, BUFFER_SIZE + 17, "CREATEMULTICAST %s", group_name);

    sendto(server_socket, message, BUFFER_SIZE, 0, (struct sockaddr *) &server_addr, (socklen_t) slen);

    int res = recvfrom(server_socket, message, BUFFER_SIZE, 0, (struct sockaddr *) &server_addr, &slen);
    if(res <= 0) {
        printf("Error contacting the server...\n");
        return;
    }

    printf("%s", message);
}

// GETMULTICAST GROUP
void connect_multicast() {
    char group_name[GROUP_SIZE], message[BUFFER_SIZE + GROUP_SIZE + 1];

    printf("Connect to group: ");
    while(fgetc(stdin) != '\n');
    
    scanf("%[^\n]s", group_name);

    snprintf(message, BUFFER_SIZE + 17, "GETMULTICAST %s", group_name);

    sendto(server_socket, message, BUFFER_SIZE, 0, (struct sockaddr *) &server_addr, (socklen_t) slen);

    int res = recvfrom(server_socket, message, BUFFER_SIZE, 0, (struct sockaddr *) &server_addr, &slen);
    if(res <= 0) {
        printf("Error contacting the server...\n");
        return;
    }

    if(!starts_with(message, "Multicast IP")) {
        printf("%s", message);
        return;
    }

    char ip[INET_ADDRSTRLEN];
    res = sscanf(message, "Multicast IP %s", ip);
    if(res != 1) {
        printf("Something went wrong...\n");
        return;
    }

    inet_pton(AF_INET, ip, &multicast_ip);

    struct ip_mreqn multicast;
    multicast.imr_multiaddr.s_addr = multicast_ip;
    multicast.imr_address.s_addr = htonl(INADDR_ANY);
    multicast.imr_ifindex = 0;

    res = setsockopt(multicast_read_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicast, sizeof(multicast));
    if(res < 0) {
        printf("Couldn't add membership to the multicast socket\n");
        return;
    }

    strcpy(current_group, group_name);
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_addr.s_addr = multicast_ip;
    multicast_addr.sin_port = htons(MULTICAST_PORT);

    printf("Multicast IP: %s\n", ip);
}

void send_multicast() {
    if(multicast_ip == INVALID_GROUP) {
        printf("You are not in a group\n");
        return;
    } else {
        char message[BUFFER_SIZE];

        printf("Write message [%s]: ", current_group);
        
        while(fgetc(stdin) != '\n');
        scanf("%[^\n]s", message);

        char send[BUFFER_SIZE + 100];
        snprintf(send, BUFFER_SIZE + 100, "[GROUP %s] %s : %s", current_group, username, message);

        sendto(multicast_write_socket, send, BUFFER_SIZE, 0, (struct sockaddr *) &multicast_addr, sizeof(multicast_addr));
    }
}

void options() {
    int option;
    do {
        printf("Menu:\n");
        printf("\t1 - Send message via Server\n");
        printf("\t2 - Send message via P2P\n");
        printf("\t3 - Create multicast group\n");
        printf("\t4 - Connect to multicast group\n");
        printf("\t5 - Send message to multicast group\n");
        printf("\t0 - Exit\n");
        printf("Choose an option:\n");
        while(fgetc(stdin) != '\n');
        scanf("%d", &option);
        switch(option){
            case 1:
                send_msg();
                break;
            case 2:
                send_p2p();
                break;
            case 3:
                create_group_multicast();
                break;
            case 4:
                connect_multicast();
                break;
            case 5:
                send_multicast();
                break;
            case 0:
                printf("Leaving\n");
                close(server_socket);
                break;
            default:
                printf("Option not found\n");
                break;
        }
    } while (option != 0);
    
}

void * read_msg() {
    char buffer[BUFFER_SIZE];

    struct sockaddr_in message_addr;

    message_addr.sin_family = AF_INET;
    message_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    message_addr.sin_port = htons(message_port);

    int sock = create_socket();

    socklen_t len = sizeof(struct sockaddr_in);

	if (bind(sock, (struct sockaddr *) &message_addr, sizeof(struct sockaddr_in)) < 0)
		error("%s\n", "Couldn't bind message socket.\n");

    char ip[INET_ADDRSTRLEN];

    while(1) {
        int valread = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &message_addr, &len);
        inet_ntop(AF_INET, &message_addr.sin_addr, ip, INET_ADDRSTRLEN);
        if(valread <= 0) {
            printf("Message read error\n");
            continue;
        }
        printf("[Message received from %s] %s\n", ip, buffer);
    }
}

void * read_multicast() {
    char buffer[BUFFER_SIZE];

    struct sockaddr_in multicast_addr;

    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_addr.s_addr = INADDR_ANY;
    multicast_addr.sin_port = htons(MULTICAST_PORT);

    socklen_t len = sizeof(struct sockaddr_in);

	if (bind(multicast_read_socket, (struct sockaddr *) &multicast_addr, sizeof(struct sockaddr_in)))
		error("%s\n", "Couldn't bind message socket.\n");


    while(1) {
        int valread = recvfrom(multicast_read_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &multicast_addr, &len);
        if(valread <= 0) {
            printf("Multicast read error\n");
            break;
        }
        printf("[Multicast] %s\n", buffer);
    }

    return NULL;
}

void authenticate() {

    printf("Login menu:\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    char message[BUFFER_SIZE];
    snprintf(message, BUFFER_SIZE, "LOGIN %s %s", username, password);

    sendto(server_socket, message, BUFFER_SIZE, 0, (struct sockaddr *) &server_addr, (socklen_t) slen);

    int n = recvfrom(server_socket, message, BUFFER_SIZE, 0, (struct sockaddr *) &server_addr, &slen);
    if(n <= 0)
        error("Error contacting the server...\n");

    printf("%s", message);

    if(strcmp(message, "Login successfully\n") == 0) {
        n = recvfrom(server_socket, message, BUFFER_SIZE, 0, (struct sockaddr *) &server_addr, &slen);
        if(n <= 0)
            error("Error contacting the server...\n");

        int res = sscanf(message, "%d", &message_port);

        pthread_create(&message_thread, NULL, &read_msg, NULL);

        if(res != 1)
            error("Something went wrong...\n");

        n = recvfrom(server_socket, message, BUFFER_SIZE, 0, (struct sockaddr *) &server_addr, &slen);
        if(n <= 0)
            error("Error contacting the server...\n");

        printf("%s", message);

        options();

        close(server_socket);
    }
}
