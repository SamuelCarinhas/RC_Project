#ifndef COMMANDS_HEADER
#define COMMANDS_HEADER

#include <stdio.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../../utils/string.h"

#define CMD_DEBUG_FLAG

extern char * commands[];
extern void (* cmd_functions[])(struct sockaddr_in *, char *, int *);

extern int cmd_function(struct sockaddr_in * client, char * command, int * exit);

extern void cmd_list(struct sockaddr_in * client, char * command, int * exit);
extern void cmd_add(struct sockaddr_in * client, char * command, int * exit);
extern void cmd_del(struct sockaddr_in * client, char * command, int * exit);
extern void cmd_quit(struct sockaddr_in * client, char * command, int * exit);

#endif