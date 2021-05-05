#include <stdio.h>
#include "network/server.h"
#include <signal.h>
#include <string.h>
#include <stdarg.h>

server_t * server;

void center_text(int width, char * format, ...) {
    char buffer[1000] = {0};
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 1000, format, args);
    va_end(args);
    int padlen = (width - strlen(buffer)) / 2;
    printf("%*s%s%*s\n", padlen, "", buffer, padlen, "");
}

void server_worker(server_t * server) {
    center_text(40, "---------------------------------------");
    center_text(40, "SERVER OPEN");
    center_text(40, "IP: %d", server->address.sin_addr.s_addr);
    center_text(40, "PORT: %d", server->port);
    center_text(40, "---------------------------------------");
    while(1) {
        int addr_len = sizeof(server->address);
        int new_socket = accept(server->socket, (struct sockaddr *) &server->address, (socklen_t *) &addr_len);

        char buffer[1000];
        int n = read(new_socket, buffer, 1000);
        buffer[n] = '\0';

        char response[] = "Hello there";
        write(new_socket, response, sizeof(response));

        printf("%s\n", buffer);
        close(new_socket);
    }
}

void cleanup() {
    close(server->socket);
    printf("\nProgram closed...\n");
    exit(0);
}

int main() {
    signal(SIGINT, cleanup);
    signal(SIGTSTP, SIG_IGN);

    server = new_server(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 7000, 10, server_worker);
    printf("Waiting...\n");

    server->worker(server);

    close(server->socket);
    free(server);
    
    return 0;
}