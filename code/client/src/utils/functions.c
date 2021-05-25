#include "functions.h"

void error(char * format, ...) {
    va_list args;
    va_start(args, format);

    printf("ERROR: ");
    vprintf(format, args);
    printf("\n");

    va_end(args);

    exit(-1);
}