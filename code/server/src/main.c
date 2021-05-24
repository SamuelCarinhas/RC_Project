#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h> 
#include <sys/wait.h>
#include "data_structures/avl_tree.h"
#include "utils/string.h"
#include "utils/numeric.h"
#include "admin/admin_server.h"
#include "client/client_server.h"
#include "config/config.h"
#include <pthread.h>

pthread_t admin_thread, client_thread;

void create_server_thread(void ( *function), int * port, pthread_t * thread) {
    pthread_create(thread, NULL, function, port);
}

void print_node(void * data) {
    printf("%s\n", (char *) data);
}

int data_cmp(void * a, void * b) {
    return strcmp((char *) a, (char *) b);
}

int main(int arg_count, char * args[]) {
    if(arg_count != 4)
        error("Invalid usage: Use ./server <CLIENT_PORT> <CONFIG_PORT> <RECORDS_FILE>\n");

    char file[BUFFER_SIZE];
    int client_port, config_port;

    if(convert_int(args[1], &client_port) == CONVERT_ERROR || client_port <= 0)
        error("Error parsing client port\n");

    if(convert_int(args[2], &config_port) == CONVERT_ERROR || config_port <= 0)
        error("Error parsing config port\n");

    strcpy(file, args[3]);

    create_server_thread(admin_server, &config_port, &admin_thread);
    create_server_thread(client_server, &client_port, &client_thread);

    pthread_join(admin_thread, NULL);
    pthread_join(client_thread, NULL);

    return 0;
}