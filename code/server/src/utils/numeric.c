/**
 * Project: RC 2021
 * 
 * Authors:
 *  Samuel dos Santos Carinhas - 2019217199
 *  Carlos Eduardo da Costa Jordão - 2019221373
 * 
 */

#include "numeric.h"

int convert_int(char * string, int * store) {
    if(sscanf(string, "%d", store) != 1)
        return CONVERT_ERROR;

    return CONVERT_SUCCESS;
}
