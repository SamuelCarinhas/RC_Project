#include "client_commands.h"

static void write_client_log(client_session_t * client, char * format, ...);

char * client_commands[] = {
    "LOGIN",
    "MSG",
    "P2P",
    "CREATEMULTICAST",
};

void (* client_cmd_functions[])(client_session_t * client, char * args, int * exit) = {
    cmd_login,
    cmd_msg,
    cmd_p2p,
    cmd_createmulticast
};

int client_cmd_function(client_session_t * client, char * command, int * exit) {
    remove_end_line(command);

    char * args;
    strtok_r(command, " ", &args);

    if(client->client == NULL)
        write_client_log(client, "User executed: %s %s\n", command, args);
    else
        write_client_log(client, "%s executed: %s %s\n", client->client->username, command, args);

    for(size_t i = 0; i < sizeof(client_commands)/sizeof(client_commands[0]); i++) {
        if(parse_cmd(command, client_commands[i])) {
            client_cmd_functions[i](client, args, exit);
            return 1;
        }
    }
    
    return 0;
}

void cmd_login(client_session_t * client, char * args, int * exit) {
    UNUSED(exit);

    if(client->logged_in) {
        send_udp_message(client, "You are already logged in\n");
        return;
    }
    
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
            if(strcmp(client->ip, database_client->ip) != 0)
                send_udp_message(client, "You cannot login to this user account. (DIFFERENT IP)\n");
            else if(strcmp(database_client->password, password) == 0) {
                client->client = database_client;
                database_client->port = client->port;
                strcpy(database_client->ip, client->ip);
                client->logged_in = 1;
                send_udp_message(client, "Login successfully\n");

                char * answer[] = {"NO", "YES"};

                send_udp_message(client, "Current account permissions:\n\tClient-Server: %s\n\tP2P: %s\n\tGroup: %s\n",
                    answer[database_client->client_server], answer[database_client->p2p], answer[database_client->group]);
            } else
                send_udp_message(client, "Wrong password\n");
        }
    }
}

void cmd_msg(client_session_t * client, char * args, int * exit) {
    UNUSED(exit);

    if(!client->logged_in) {
        send_udp_message(client, "You need to be logged in to perform this command\n");
        return;
    }

    if(!client->client->client_server) {
        send_udp_message(client, "You do not have permission to use this command\n");
        return;
    }
    
    char username[USERNAME_SIZE], message[BUFFER_SIZE];

    int res = sscanf(args, "%s %[^\n]s", username, message);

    if(res != 2)
        send_udp_message(client, "Invalid usage! Use: MSG <USER-ID> <MSG>\n");
    else {
        client_t * client_arg = new_client(username, "", "", 0, 0, 0);
        client_t * database_client;
        database_client = (client_t *) avl_get(user_list, client_arg);
        free(client_arg);

        if(database_client == NULL)
            send_udp_message(client, "User not found\n");
        else {
            client_session_t client_temp;
            strcpy(client_temp.ip, database_client->ip);
            client_temp.port = database_client->port;

            client_session_t * client_session = (client_session_t *) avl_get(user_session_list, &client_temp);

            if(client_session == NULL)
                send_udp_message(client, "Couldn't find user session\n");
            else
                send_udp_message(client_session, "[%s] %s\n", client->client->username, message);
        }
    }
}

void cmd_p2p(client_session_t * client, char * args, int * exit) {
    UNUSED(exit);

    if(!client->logged_in) {
        send_udp_message(client, "You need to be logged in to perform this command\n");
        return;
    }

    if(!client->client->p2p) {
        send_udp_message(client, "You do not have permission to use this command\n");
        return;
    }
    
    char username[USERNAME_SIZE];

    int res = sscanf(args, "%s", username);

    if(res != 1)
        send_udp_message(client, "Invalid usage! Use: P2P <USER-ID>\n");
    else {
        client_t * client_arg = new_client(username, "", "", 0, 0, 0);
        client_t * database_client;
        database_client = (client_t *) avl_get(user_list, client_arg);
        free(client_arg);

        if(database_client == NULL)
            send_udp_message(client, "User not found\n");
        else {
            client_session_t client_temp;
            strcpy(client_temp.ip, database_client->ip);
            client_temp.port = database_client->port;

            client_session_t * client_session = (client_session_t *) avl_get(user_session_list, &client_temp);

            if(client_session == NULL)
                send_udp_message(client, "Couldn't find user session\n");
            else
                send_udp_message(client, "%s IP: %s, Port: %d\n", username, client_session->ip, client_session->port);
        }
    }
}

void cmd_createmulticast(client_session_t * client, char * args, int * exit) {
    UNUSED(exit);
    UNUSED(args);

    if(!client->logged_in) {
        send_udp_message(client, "You need to be logged in to perform this command\n");
        return;
    }

    if(!client->client->group) {
        send_udp_message(client, "You do not have permission to use this command\n");
        return;
    }

    send_udp_message(client, "Multicast IP created\n");
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