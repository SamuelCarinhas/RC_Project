/**
 * Project: RC 2021
 * 
 * Authors:
 *  Samuel dos Santos Carinhas - 2019217199
 *  Carlos Eduardo da Costa Jordão - 2019221373
 * 
 */

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
#include "group/group.h"

pthread_t admin_thread, client_thread;

avl_tree_t * user_list;
avl_tree_t * user_session_list;
avl_tree_t * group_list;

char * groups[] = {"239.0.0.1", "239.0.0.2", "239.0.0.3", "239.0.0.4", "239.0.0.5", "239.0.0.6"};

int current_group;
int max_groups = sizeof(groups)/sizeof(groups[0]);

void print_node(void * data) {
    printf("%s\n", (char *) data);
}

void print_session(void * data) {
    client_session_t * session = (client_session_t *) data;
    printf("(%s:%d) %s\n", session->ip, session->port, session->client->username);
}

void print_group(void * data) {

}

int group_cmp(void * a, void * b) {
    group_t * group_a = (group_t *) a,
                * group_b = (group_t *) b;
    
    return strcmp(group_a->name, group_b->name);
}

int data_cmp(void * a, void * b) {
    return strcmp((char *) a, (char *) b);
}

int session_cmp(void * a, void * b) {
    client_session_t * session_a = (client_session_t *) a,
                        * session_b = (client_session_t *) b;
    
    int ip_cmp = strcmp(session_a->ip, session_b->ip);
    if(ip_cmp != 0)
        return ip_cmp;
    else
        return session_a->port - session_b->port;
}

void signal_sigint() {
    printf("Finish\n");
    exit(0);
}

void create_server_thread(void ( *function), int * port, pthread_t * thread) {
    pthread_create(thread, NULL, function, port);
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
    user_session_list = new_avl_tree(session_cmp, print_session, -1);
    group_list = new_avl_tree(group_cmp, print_group, 1);

    int n_read;
    client_t client;
    while(1) {
        n_read = read(records_fd, &client, sizeof(client_t)); 
        if(n_read <= 0)
            break;
        avl_add(user_list, &client, sizeof(client_t), 0, 0);
    }

    printf("Current multicast: %s\n", groups[current_group]);

    create_server_thread(admin_server, &config_port, &admin_thread);
    create_server_thread(client_server, &client_port, &client_thread);

    pthread_join(admin_thread, NULL);
    pthread_join(client_thread, NULL);

    return 0;
}