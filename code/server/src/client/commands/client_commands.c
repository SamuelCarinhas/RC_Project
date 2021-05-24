#include "client_commands.h"

static void write_client_log(client_session_t * client, char * format, ...);

char * client_commands[] = {
    "LOGIN",
};

void (* client_cmd_functions[])(client_session_t * client, char * args, int * exit) = {
    cmd_login,
};

int client_cmd_function(client_session_t * client, char * command, int * exit) {
    remove_end_line(command);

    char * args;
    strtok_r(command, " ", &args);

    write_client_log(client, "User executed: %s %s\n", command, args);
    for(size_t i = 0; i < sizeof(client_commands)/sizeof(client_commands[0]); i++) {
        if(parse_cmd(command, client_commands[i])) {
            client_cmd_functions[i](client, args, exit);
            return 1;
        }
    }
    
    return 0;
}

void cmd_login(client_session_t * client, char * args, int * exit) {
    char username[USERNAME_SIZE], password[PASSWORD_SIZE];

    int res = sscanf(args, "%s %s", username, password);

    if(res != 2)
        send_udp_message(client, "Invalid usage! Use: LOGIN <USER-ID> <PASSWORD>\n");
    else {
        client_t * client_arg = new_client(username, "", "", 0, 0, 0);
        client_t * database_client;
        database_client = (client_t *) avl_get(user_list, client_arg);
        free(client_arg);

        if(database_client == NULL)
            send_udp_message(client, "User not found\n");
        else {
            if(strcmp(database_client->password, password) == 0)
                send_udp_message(client, "Login successfully\n");
            else
                send_udp_message(client, "Wrong password\n");
        }
    }
}

static void write_client_log(client_session_t * client, char * format, ...) {
    #ifdef CLIENT_CMD_DEBUG
        va_list args;
        va_start(args, format);
        printf("[CLIENT %s:%d] ", client->ip, client->port);
        vprintf(format, args);
        va_end(args);
    #endif
}