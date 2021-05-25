#include "client_server.h"

server_t * udp_server;

static void udp_server_worker(server_t * server) {
    center_text(40, "---------------------------------------");
    center_text(40, "CLIENT SERVER OPEN");
    center_text(40, "IP: %d", server->address.sin_addr.s_addr);
    center_text(40, "PORT: %d", server->port);
    center_text(40, "---------------------------------------");

    struct sockaddr_in client_sock;

    client_session_t * client_session;

    int exit;
    while(1) {
        socklen_t len = sizeof(struct sockaddr_in);

        char buffer[BUFFER_SIZE];
        int n = recvfrom(server->socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &client_sock, &len);
        buffer[n] = '\0';
        remove_end_line(buffer);

        client_session = init_client_session(&client_sock, len, server->socket);

        if(!client_cmd_function(client_session, buffer, &exit))
            send_udp_message(client_session, "Command not found\n");
    }
}

void client_server(int * port) {
    udp_server = new_server(AF_INET, SOCK_DGRAM, 0, INADDR_ANY, *port, 16, udp_server_worker);
    udp_server->worker(udp_server);
}