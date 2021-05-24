#include "numeric.h"

int convert_int(char * string, int * store) {
    if(sscanf(string, "%d", store) != 1)
        return CONVERT_ERROR;

    return CONVERT_SUCCESS;
}
