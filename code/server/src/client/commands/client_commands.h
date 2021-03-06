/**
 * Project: RC 2021
 * 
 * Authors:
 *  Samuel dos Santos Carinhas - 2019217199
 *  Carlos Eduardo da Costa Jordão - 2019221373
 * 
 */

#ifndef CLIENT_COMMANDS_HEADER
#define CLIENT_COMMANDS_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "../client_struct.h"
#include "../client.h"
#include "../../utils/string.h"
#include "../../utils/functions.h"
#include "../../memory/memory.h"
#include "../../data_structures/avl_tree.h"
#include "../../group/group.h"

#define CLIENT_CMD_DEBUG

int client_cmd_function(client_session_t * client, char * command, int * exit);

extern void cmd_login(client_session_t * client, char * command, int * exit);
extern void cmd_msg(client_session_t * client, char * command, int * exit);
extern void cmd_p2p(client_session_t * client, char * command, int * exit);
extern void cmd_createmulticast(client_session_t * client, char * args, int * exit);
extern void cmd_getmulticast(client_session_t * client, char * args, int * exit);

#endif