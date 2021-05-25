#ifndef FUNCTIONS_HEADER
#define FUNCTIONS_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <arpa/inet.h>

extern void error(char * format, ...);
extern void add_timeout(int socket, int seconds);
extern int create_socket();
extern int starts_with(char * a, char * b);

#endif