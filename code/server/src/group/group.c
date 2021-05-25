#include "group.h"

group_t * new_group(char * name, char * ip) {
    group_t * group = (group_t *) malloc(sizeof(group_t));

    strcpy(group->name, name);
    strcpy(group->ip, ip);

    return group;
}