/**
 * Project: RC 2021
 * 
 * Authors:
 *  Samuel dos Santos Carinhas - 2019217199
 *  Carlos Eduardo da Costa Jordão - 2019221373
 * 
 */

#ifndef SERVER_HEADER
#define SERVER_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../utils/functions.h"
#include <unistd.h>

typedef struct server server_t;

struct server {
    int domain;
    int service;
    int protocol;
    u_long interface;
    int port;
    int backlog;
    int socket;
    struct sockaddr_in address;

    void (* worker) (server_t * server);
};

server_t * new_server(int domain, int service, int protocol, u_long interface, int port, int backlog, void (* worker) (server_t * server));

#endif