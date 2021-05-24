#include "admin_commands.h"

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

    write_admin_log("User executed: %s %s\n", command, args);
    for(size_t i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
        if(parse_cmd(command, commands[i])) {
            cmd_functions[i](admin, args, exit);
            return 1;
        }
    }
    
    return 0;
}

void cmd_list(admin_t * admin, char * args, int * exit) {
    *exit = 0;
    UNUSED(args);
    avl_print_client(user_list, send_to_client, admin->socket);
}

void cmd_add(admin_t * admin, char * args, int * exit) {
    *exit = 0;

    char username[USERNAME_SIZE], password[PASSWORD_SIZE], ip[INET_ADDRSTRLEN];
    int client_server, p2p, group;

    int res = sscanf(args, "%s %s %s %d %d %d", username, ip, password, &client_server, &p2p, &group);


    if(res != 6) {
        write_fd(admin->socket, "Invalid usage! Use: ADD <USER-ID> <IP> <PASSWORD> <CLIENT-SERVER> <P2P> <GROUP>\n");
    } else {
        client_t * client = new_client(username, password, ip, client_server, p2p, group);
        res = avl_add(user_list, client, sizeof(client_t), 1, 1);
        if(res == AVL_DUPLICATED_KEY)
            write_fd(admin->socket, "User already exists\n");
        else
            write_fd(admin->socket, "User %s created.\n", username);
    }

}

void cmd_del(admin_t * admin, char * args, int * exit) {
    *exit = 0;

    char username[USERNAME_SIZE];
    int res = sscanf(args, "%s", username);

    if(res != 1) {
        write_fd(admin->socket, "Invalid usage! Use: DEL <USER-ID>\n");
    } else {
        client_t * client = new_client(username, "", "", 1, 1, 1);
        res = avl_remove(user_list, client, sizeof(client_t), 1, 1);
        if(res == AVL_KEY_NOT_FOUND)
            write_fd(admin->socket, "User not found\n");
        else
            write_fd(admin->socket, "User %s deleted.\n", username);
    }
}

void cmd_quit(admin_t * admin, char * args, int * exit) {
    UNUSED(admin);
    UNUSED(args);
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