#ifndef STRING_HEADER
#define STRING_HEADER

#include <string.h>
#include <stdio.h>

#define STRING_SIZE(str) sizeof(str)/sizeof(str[0])

extern int starts_with(char * a, char * b);
extern int parse_cmd(char * a, char * b);

extern void remove_end_line(char * string);

#endif