#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h> 
#include <pthread.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "data_structures/avl_tree.h"
#include "utils/string.h"
#include "utils/numeric.h"
#include "admin/admin_server.h"
#include "client/client_server.h"
#include "client/client_struct.h"
#include "config/config.h"
#include "memory/memory.h"

pthread_t admin_thread, client_thread;

avl_tree_t * user_list;

void create_server_thread(void ( *function), int * port, pthread_t * thread) {
    pthread_create(thread, NULL, function, port);
}

void print_node(void * data) {
    printf("%s\n", (char *) data);
}

int data_cmp(void * a, void * b) {
    return strcmp((char *) a, (char *) b);
}

void signal_sigint() {
    printf("Hello there\n");
    exit(0);
}

int main(int arg_count, char * args[]) {
    signal(SIGINT, signal_sigint);

    if(arg_count != 4)
        error("Invalid usage: Use ./server <CLIENT_PORT> <CONFIG_PORT> <RECORDS_FILE>\n");

    char file[BUFFER_SIZE];
    int client_port, config_port;

    if(convert_int(args[1], &client_port) == CONVERT_ERROR || client_port <= 0)
        error("Error parsing client port\n");

    if(convert_int(args[2], &config_port) == CONVERT_ERROR || config_port <= 0)
        error("Error parsing config port\n");

    strcpy(file, args[3]);

    int records_fd = open(file, O_CREAT | O_RDWR | O_APPEND, 0644);

    if(records_fd < 0)
        error("Couldn't open/create records file");

    user_list = new_avl_tree(data_cmp, print_node, records_fd);

    // Load data from records file
    int n_read;
    client_t * client = (client_t *) malloc(sizeof(client_t));
    while(1) {
        n_read = read(records_fd, client, sizeof(client_t)); 
        if(n_read <= 0)
            break;
        avl_add(user_list, client, sizeof(client_t), 0, 0);
    }
    free(client);

    create_server_thread(admin_server, &config_port, &admin_thread);
    create_server_thread(client_server, &client_port, &client_thread);

    pthread_join(admin_thread, NULL);
    pthread_join(client_thread, NULL);

    return 0;
}