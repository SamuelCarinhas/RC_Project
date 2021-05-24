#include "client.h"

int client_cmp(void * a, void * b) {
    client_t * client_a = (client_t *) a;
    client_t * client_b = (client_t *) b;

    return strcmp(client_a->username, client_b->username);
}

void client_print(void * client) {
    printf("%s\n", ((client_t *) client)->username);
}

void send_to_client(void * data, int socket) {
    char message[BUFFER_SIZE] = {0};
    snprintf(message, BUFFER_SIZE, "User: %s\n", ((client_t *) data)->username);
    write(socket, message, BUFFER_SIZE);
}

client_t * new_client(char * username, char * password, char * ip, int client_server, int p2p, int group) {
    client_t * new_client = (client_t *) malloc(sizeof(client_t));

    strcpy(new_client->username, username);
    strcpy(new_client->password, password);
    strcpy(new_client->ip, ip);

    new_client->client_server = client_server;
    new_client->p2p = p2p;
    new_client->group = group;

    return new_client;
}

void free_client(client_t * client) {
    free(client);
}