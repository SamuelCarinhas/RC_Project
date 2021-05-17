#include "commands.h"

static void write_admin_log(char * format, ...);

char * commands[] = {
    "LIST",
    "ADD",
    "DEL",
    "QUIT"
};

void (* cmd_functions[])(admin_t * admin, char * command, int * exit) = {
    cmd_list,
    cmd_add,
    cmd_del,
    cmd_quit
};

int cmd_function(admin_t * admin, char * command, int * exit) {
    remove_end_line(command);

    char * args;
    strtok_r(command, " ", &args);

    for(size_t i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
        if(parse_cmd(command, commands[i])) {
            write_admin_log("User executed: %s %s\n", command, args);
            cmd_functions[i](admin, args, exit);
            return 1;
        }
    }
    return 0;
}

void cmd_list(admin_t * admin, char * args, int * exit) {
    *exit = 0;

    avl_print_client(client_list, send_to_client, admin->socket);
}

void cmd_add(admin_t * admin, char * args, int * exit) {
    *exit = 0;

    char username[16], password[30], ip[20];
    int client_server, p2p, group;

    int res = sscanf(args, "%s %s %s %d %d %d", username, ip, password, &client_server, &p2p, &group);


    if(res != 6) {
        char message[BUFFER_SIZE] = {0};
        snprintf(message, BUFFER_SIZE, "Invalid usage! Use: /ADD <USER-ID> <IP> <PASSWORD> <CLIENT-SERVER> <P2P> <GROUP>\n");
        write(admin->socket, message, BUFFER_SIZE);
    } else {
        client_t * client = new_client(username, password, ip, client_server, p2p, group);
        avl_add(client_list, client, sizeof(client_t));
        char message[BUFFER_SIZE] = {0};
        snprintf(message, BUFFER_SIZE, "User %s created.\n", username);
        write(admin->socket, message, BUFFER_SIZE);
    }

}

void cmd_del(admin_t * admin, char * args, int * exit) {
    *exit = 0;
}

void cmd_quit(admin_t * admin, char * args, int * exit) {
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