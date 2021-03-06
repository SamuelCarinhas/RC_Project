/**
 * Project: RC 2021
 * 
 * Authors:
 *  Samuel dos Santos Carinhas - 2019217199
 *  Carlos Eduardo da Costa Jordão - 2019221373
 * 
 */

#ifndef CLIENT_HEADER
#define CLIENT_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client_struct.h"
#include "../data_structures/avl_tree.h"
#include "../config/config.h"
#include "../network/server.h"
#include "../memory/memory.h"

extern client_t * new_client(char * username, char * password, char * ip, int client_server, int p2p, int group);

extern void free_client(client_t * client);
extern void client_print(void * client);
extern void send_to_client(void * data, int socket);
extern client_session_t * init_client_session(struct sockaddr_in * sock, socklen_t len, int socket);

extern int client_cmp(void * a, void * b);

#endif