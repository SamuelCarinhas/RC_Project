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