#include <stdio.h>
#include "network/server.h"
#include "commands/commands.h"
#include "utils/string.h"
#include "admin/admin_server.h"
#include <signal.h>
#include <string.h>
#include <stdarg.h>
#include <sys/wait.h>

void create_proccess(void ( *function)(void)) {
    if(fork() == 0) {
        function();
        exit(0);
    }
}

void wait_for_children() {
    while(wait(NULL) != -1);
}

int main() {
    
    create_proccess(admin_server);

    wait_for_children();

    return 0;
}