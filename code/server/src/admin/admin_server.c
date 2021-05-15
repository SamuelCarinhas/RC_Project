#include "admin_server.h"

static void server_worker(server_t * server);

static server_t * server;

static void server_worker(server_t * server) {
    center_text(40, "---------------------------------------");
    center_text(40, "ADMIN SERVER OPEN");
    center_text(40, "IP: %d", server->address.sin_addr.s_addr);
    center_text(40, "PORT: %d", server->port);
    center_text(40, "---------------------------------------");
    while(1) {
        int addr_len = sizeof(server->address);
        int new_socket = accept(server->socket, (struct sockaddr *) &server->address, (socklen_t *) &addr_len);

        

        close(new_socket);
    }
}

void admin_server() {
    server = new_server(AF_INET, SOCK_STREAM, 0, INADDR_ANY, ADMIN_PORT, 1, server_worker);
    server->worker(server);


}