#ifndef COMMANDS_HEADER
#define COMMANDS_HEADER

#include <stdio.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../../memory/memory.h"
#include "../../utils/string.h"
#include "../../config/config.h"
#include "../../utils/functions.h"
#include "../admin.h"

#define CMD_DEBUG_FLAG

extern char * commands[];
extern void (* cmd_functions[])(admin_t * admin, char * command, int * exit);

extern int cmd_function(admin_t * admin, char * command, int * exit);

extern void cmd_list(admin_t * admin, char * command, int * exit);
extern void cmd_add(admin_t * admin, char * command, int * exit);
extern void cmd_del(admin_t * admin, char * command, int * exit);
extern void cmd_quit(admin_t * admin, char * command, int * exit);

#endif