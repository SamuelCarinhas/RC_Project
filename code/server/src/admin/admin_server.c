#include "admin_server.h"

static void server_worker(server_t * server);
static void handle_admin(struct sockaddr_in * client, int client_socket);

static server_t * server;

static void server_worker(server_t * server) {
    center_text(40, "---------------------------------------");
    center_text(40, "ADMIN SERVER OPEN");
    center_text(40, "IP: %d", server->address.sin_addr.s_addr);
    center_text(40, "PORT: %d", server->port);
    center_text(40, "---------------------------------------");

    struct sockaddr_in client;
    int client_size;

    while(1) {
        int client_socket = accept(server->socket, (struct sockaddr *) &client, (socklen_t *) &client_size);

        if(client_socket > 0) {
            handle_admin(&client, client_socket);

            close(client_socket);
        }
    }
}

static void handle_admin(struct sockaddr_in * client, int client_socket) {
    printf("[ADMIN] User connected...\n");
    int n_read, command_found, exit;
    char buffer[BUFFER_SIZE];

    while(1) {
        n_read = read(client_socket, buffer, BUFFER_SIZE);

        if(n_read <= 0)
            break;

        command_found = cmd_function(client, buffer, &exit);
        if(!command_found)
            printf("[ADMIN] Command not found...\n");
        
        if(exit)
            break;
    }

    printf("[ADMIN] User disconnected...\n");

}

void admin_server() {
    server = new_server(AF_INET, SOCK_STREAM, 0, INADDR_ANY, ADMIN_PORT, 1, server_worker);
    server->worker(server);
}