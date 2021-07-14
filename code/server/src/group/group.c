/**
 * Project: RC 2021
 * 
 * Authors:
 *  Samuel dos Santos Carinhas - 2019217199
 *  Carlos Eduardo da Costa Jordão - 2019221373
 * 
 */

#include "group.h"

group_t * new_group(char * name, char * ip) {
    group_t * group = (group_t *) malloc(sizeof(group_t));

    strcpy(group->name, name);
    strcpy(group->ip, ip);

    return group;
}