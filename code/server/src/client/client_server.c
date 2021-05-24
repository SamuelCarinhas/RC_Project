#include "client_server.h"

static server_t * udp_server;

static void udp_server_worker(server_t * server) {
    center_text(40, "---------------------------------------");
    center_text(40, "CLIENT SERVER OPEN");
    center_text(40, "IP: %d", server->address.sin_addr.s_addr);
    center_text(40, "PORT: %d", server->port);
    center_text(40, "---------------------------------------");
    while(1) {
        socklen_t len = 0;

        char buffer[1000];
        int n = recvfrom(server->socket, buffer, 1000, MSG_WAITALL, 0, &len);
        buffer[n] = '\0';

        printf("%s\n", buffer);
    }
}

void client_server() {
    udp_server = new_server(AF_INET, SOCK_DGRAM, 0, INADDR_ANY, 7000, 10, udp_server_worker);
    udp_server->worker(udp_server);
}