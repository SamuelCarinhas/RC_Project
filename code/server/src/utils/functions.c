#include "functions.h"

void send_udp_message(client_session_t * client, char * format, ...) {
    va_list args;
    va_start(args, format);

    char message[BUFFER_SIZE] = {0};
    vsnprintf(message, BUFFER_SIZE, format, args);
    
    sendto(client->socket, message, BUFFER_SIZE, 0, (struct sockaddr *) &client->sock, client->len);

    va_end(args);
}

void write_fd(int fd, char * format, ...) {
    va_list args;
    va_start(args, format);

    char message[BUFFER_SIZE] = {0};
    vsnprintf(message, BUFFER_SIZE, format, args);
    write(fd, message, BUFFER_SIZE);

    va_end(args);
}

void error(char * format, ...) {
    va_list args;
    va_start(args, format);

    printf("ERROR: ");
    vprintf(format, args);
    printf("\n");

    va_end(args);

    exit(-1);
}

void center_text(int width, char * format, ...) {
    char buffer[1000] = {0};
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 1000, format, args);
    va_end(args);
    int padlen = (width - strlen(buffer)) / 2;
    printf("%*s%s%*s\n", padlen, "", buffer, padlen, "");
}