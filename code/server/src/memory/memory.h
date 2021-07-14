/**
 * Project: RC 2021
 * 
 * Authors:
 *  Samuel dos Santos Carinhas - 2019217199
 *  Carlos Eduardo da Costa Jordão - 2019221373
 * 
 */

#ifndef MEMORY_HEADER
#define MEMORY_HEADER

#include "../data_structures/avl_tree.h"

extern avl_tree_t * user_list;
extern avl_tree_t * user_session_list;
extern avl_tree_t * group_list;
extern char * groups[];
extern int current_group;
extern int max_groups;

#endif