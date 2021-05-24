#ifndef ADMIN_SERVER_HEADER
#define ADMIN_SERVER_HEADER

#include <stdio.h>
#include <stdlib.h>
#include "commands/commands.h"
#include "../network/server.h"
#include "../config/config.h"
#include "../utils/functions.h"
#include "admin.h"

extern void admin_server(int * port);

#endif