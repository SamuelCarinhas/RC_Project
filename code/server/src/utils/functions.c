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

void center_text(int width, char * format, ...) {
    char buffer[1000] = {0};
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 1000, format, args);
    va_end(args);
    int padlen = (width - strlen(buffer)) / 2;
    printf("%*s%s%*s\n", padlen, "", buffer, padlen, "");
}