/**
 * Project: RC 2021
 * 
 * Authors:
 *  Samuel dos Santos Carinhas - 2019217199
 *  Carlos Eduardo da Costa Jordão - 2019221373
 * 
 */

#ifndef ADMIN_SERVER_HEADER
#define ADMIN_SERVER_HEADER

#include <stdio.h>
#include <stdlib.h>
#include "commands/admin_commands.h"
#include "../network/server.h"
#include "../config/config.h"
#include "../utils/functions.h"
#include "admin.h"

extern void admin_server(int * port);

#endif