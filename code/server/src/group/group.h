/**
 * Project: RC 2021
 * 
 * Authors:
 *  Samuel dos Santos Carinhas - 2019217199
 *  Carlos Eduardo da Costa Jordão - 2019221373
 * 
 */

#ifndef GROUP_HEADER
#define GROUP_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_GROUP_NAME 16

typedef struct group group_t;

struct group {
    char name[MAX_GROUP_NAME];
    char ip[INET_ADDRSTRLEN];
};

extern group_t * new_group(char * name, char * ip);

#endif