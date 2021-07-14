/**
 * Project: RC 2021
 * 
 * Authors:
 *  Samuel dos Santos Carinhas - 2019217199
 *  Carlos Eduardo da Costa Jordão - 2019221373
 * 
 */

#ifndef CLIENT_SERVER_HEADER
#define CLIENT_SERVER_HEADER

#include <stdio.h>
#include <stdlib.h>
#include "client_struct.h"
#include "commands/client_commands.h"
#include "../network/server.h"
#include "../utils/string.h"
#include "client.h"
#include <arpa/inet.h>

extern server_t * udp_server;

extern void client_server(int * port);

#endif