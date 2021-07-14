/**
 * Project: RC 2021
 * 
 * Authors:
 *  Samuel dos Santos Carinhas - 2019217199
 *  Carlos Eduardo da Costa Jordão - 2019221373
 * 
 */

#ifndef ADMIN_HEADER
#define ADMIN_HEADER

#include <stdlib.h>
#include "../client/client.h"

typedef struct admin admin_t;

struct admin {
    int socket;
    struct sockaddr_in * admin;
};

extern admin_t * new_admin(int socket, struct sockaddr_in * admin);

extern void free_admin(admin_t * admin);

#endif