#ifndef ADMIN_SERVER_HEADER
#define ADMIN_SERVER_HEADER

#include <stdio.h>
#include <stdlib.h>
#include "commands/commands.h"
#include "../network/server.h"
#include "../config/config.h"
#include "admin.h"

#define ADMIN_PORT 7000

extern void admin_server();

#endif