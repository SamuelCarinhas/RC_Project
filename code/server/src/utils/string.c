#include "string.h"

int starts_with(char * a, char * b) {
    if(strlen(b) > strlen(a))
        return 0;

    for(size_t i = 0; i < strlen(b); i++) {
        if(a[i] != b[i])
            return 0;
    }

    return 1;
}