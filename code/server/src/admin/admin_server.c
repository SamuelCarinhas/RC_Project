#include "admin_server.h"

static void server_worker(server_t * server);
static void handle_admin(admin_t * admin);

server_t * server;

static void server_worker(server_t * server) {
    center_text(40, "---------------------------------------");
    center_text(40, "ADMIN SERVER OPEN");
    center_text(40, "IP: %d", server->address.sin_addr.s_addr);
    center_text(40, "PORT: %d", server->port);
    center_text(40, "---------------------------------------");

    struct sockaddr_in admin_sock;
    int admin_size;

    while(1) {
        int admin_socket = accept(server->socket, (struct sockaddr *) &admin_sock, (socklen_t *) &admin_size);
        admin_t * admin = new_admin(admin_socket, &admin_sock);

        if(admin_socket > 0) {
            handle_admin(admin);

            close(admin_socket);
        }
    }
}

static void handle_admin(admin_t * admin) {
    printf("[ADMIN] User connected...\n");
    int n_read, command_found, exit;
    char buffer[BUFFER_SIZE];

    while(1) {
        n_read = read(admin->socket, buffer, BUFFER_SIZE);

        if(n_read <= 0)
            break;

        command_found = cmd_function(admin, buffer, &exit);
        if(!command_found)
            write_fd(admin->socket, "Command not found\n");
        
        if(exit)
            break;
    }

    printf("[ADMIN] User disconnected...\n");

}

void admin_server(int * port) {
    server = new_server(AF_INET, SOCK_STREAM, 0, INADDR_ANY, *port, 1, server_worker);
    server->worker(server);
}