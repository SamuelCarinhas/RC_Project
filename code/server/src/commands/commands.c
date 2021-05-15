#include "commands.h"

static void cmd_debug(char * format, ...);

char * commands[] = {
    "LIST",
    "ADD",
    "DEL",
    "QUIT"
};

void (* cmd_functions[])(struct sockaddr_in *, char * ) = {
    cmd_list,
    cmd_add,
    cmd_del,
    cmd_quit
};

int cmd_function(struct sockaddr_in * client, char * command) {
    for(size_t i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
        if(starts_with(command, commands[i])) {
            cmd_debug("Command parsed: %s\n", command);
            cmd_functions[i](client, command);
            return 1;
        }
    }
    return 0;
}

void cmd_list(struct sockaddr_in * client, char * command) {
}

void cmd_add(struct sockaddr_in * client, char * command) {
}

void cmd_del(struct sockaddr_in * client, char * command) {
}

void cmd_quit(struct sockaddr_in * client, char * command) {
}

static void cmd_debug(char * format, ...) {
    #ifdef CMD_DEBUG_FLAG
        va_list args;
        va_start(args, format);
        printf("[DEBUG] ");
        vprintf(format, args);
        va_end(args);
    #endif
}