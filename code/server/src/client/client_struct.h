#ifndef CLIENT_STRUCT_HEADER
#define CLIENT_STRUCT_HEADER

#define USERNAME_SIZE 16
#define PASSWORD_SIZE 32
#define IP_SIZE 20

typedef struct client client_t;

struct client {
    char username[USERNAME_SIZE];
    char password[PASSWORD_SIZE];
    char ip[IP_SIZE];

    int client_socket;
    int client_server;
    int p2p;
    int group;
};

#endif