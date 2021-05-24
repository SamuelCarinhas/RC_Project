#ifndef CLIENT_STRUCT_HEADER
#define CLIENT_STRUCT_HEADER

#define USERNAME_SIZE 16
#define PASSWORD_SIZE 32

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct client client_t;
typedef struct client_session client_session_t;

struct client {
    char username[USERNAME_SIZE];
    char password[PASSWORD_SIZE];

    char ip[INET_ADDRSTRLEN];
    int port;

    int client_server;
    int p2p;
    int group;
};

struct client_session {
    client_t * client;

    struct sockaddr_in sock;

    socklen_t len;

    char ip[INET_ADDRSTRLEN];

    int port;
    int socket;
    int logged_in;
};


#endif