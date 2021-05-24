#include "admin.h"

admin_t * new_admin(int socket, struct sockaddr_in * admin) {
    admin_t * new_admin = (admin_t *) malloc(sizeof(admin_t));

    new_admin->socket = socket;
    new_admin->admin = admin;

    return new_admin;
}

extern void free_admin(admin_t * admin) {
    free(admin);
}