#include "string.h"

void remove_end_line(char * string) {
    char * current = string;

    while(*current) {
        if(*current == '\r' || *current == '\n') {
            *current = '\0';
            break;
        }
        current++;
    }
}

int parse_cmd(char * a, char * b) {
    if(a == NULL)
        return 0;
    return strcmp(a, b) == 0;
}

int starts_with(char * a, char * b) {
    if(strlen(b) > strlen(a))
        return 0;

    for(size_t i = 0; i < strlen(b); i++) {
        if(a[i] != b[i])
            return 0;
    }

    return 1;
}