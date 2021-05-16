#include "commands.h"

static void write_admin_log(char * format, ...);

char * commands[] = {
    "LIST",
    "ADD",
    "DEL",
    "QUIT"
};

void (* cmd_functions[])(struct sockaddr_in *, char *, int *) = {
    cmd_list,
    cmd_add,
    cmd_del,
    cmd_quit
};

int cmd_function(struct sockaddr_in * client, char * command, int * exit) {
    remove_end_line(command);

    char * args;
    strtok_r(command, " ", &args);

    for(size_t i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
        if(parse_cmd(command, commands[i])) {
            write_admin_log("User executed: %s %s\n", command, args);
            cmd_functions[i](client, args, exit);
            return 1;
        }
    }
    return 0;
}

void cmd_list(struct sockaddr_in * client, char * args, int * exit) {
    *exit = 0;
}

void cmd_add(struct sockaddr_in * client, char * args, int * exit) {
    *exit = 0;
}

void cmd_del(struct sockaddr_in * client, char * args, int * exit) {
    *exit = 0;
}

void cmd_quit(struct sockaddr_in * client, char * args, int * exit) {
    *exit = 1;
}

static void write_admin_log(char * format, ...) {
    #ifdef CMD_DEBUG_FLAG
        va_list args;
        va_start(args, format);
        printf("[ADMIN] ");
        vprintf(format, args);
        va_end(args);
    #endif
}