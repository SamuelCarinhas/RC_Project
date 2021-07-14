/**
 * Project: RC 2021
 * 
 * Authors:
 *  Samuel dos Santos Carinhas - 2019217199
 *  Carlos Eduardo da Costa Jordão - 2019221373
 * 
 */

#include "functions.h"

void error(char * format, ...) {
    va_list args;
    va_start(args, format);

    printf("ERROR: ");
    vprintf(format, args);
    printf("\n");

    va_end(args);

    exit(-1);
}

int create_socket() {
    int new_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(new_socket < 0)
        error("Couldn't create socket\n");
    return new_socket;
}

int starts_with(char * a, char * b) {
    unsigned int len_b = strlen(b);
    if(len_b > strlen(a))
        return 0;

    for(unsigned int i = 0; i < len_b; i++)
        if(a[i] != b[i])
            return 0;

    return 1;
}

void add_timeout(int socket, int seconds) {
    struct timeval timeval;
    timeval.tv_sec = 0;
    timeval.tv_usec = seconds * 1000000;
    if (setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &timeval, sizeof(struct timeval)) < 0) {
        error("Error setting time");
    }
}