#include "server.h"

server_t * new_server(int domain, int service, int protocol, u_long interface, int port, int backlog, void (* worker) (server_t * server)) {
    server_t * server = (server_t *) malloc(sizeof(server_t));

    server->domain = domain;
    server->service = service;
    server->protocol = protocol;
    server->interface = interface;
    server->port = port;
    server->backlog = backlog;

    server->address.sin_family = domain;
    server->address.sin_port = htons(port);
    server->address.sin_addr.s_addr = htonl(interface);

    server->socket = socket(domain, service, protocol);

    if(server->socket == 0) {
        free(server);
        error("Couldn't create server socket");
    }

    if(bind(server->socket, (struct sockaddr *) &server->address, sizeof(server->address)) < 0) {
        free(server);
        error("Couldn't bind the server socket");
    }

    if(listen(server->socket, server->backlog) < 0) {
        free(server);
        error("Couldn't listen the server socket");
    }

    server->worker = worker;

    return server;
}
