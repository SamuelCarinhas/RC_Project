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