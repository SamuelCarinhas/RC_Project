/**
 * Project: RC 2021
 * 
 * Authors:
 *  Samuel dos Santos Carinhas - 2019217199
 *  Carlos Eduardo da Costa Jordão - 2019221373
 * 
 */

#ifndef STRING_HEADER
#define STRING_HEADER

#include <string.h>
#include <stdio.h>

#define STRING_SIZE(str) sizeof(str)/sizeof(str[0])

extern int starts_with(char * a, char * b);
extern int parse_cmd(char * a, char * b);

extern void remove_end_line(char * string);

#endif