#ifndef FUNCTIONS_HEADER
#define FUNCTIONS_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h> 
#include <pthread.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../config/config.h"

extern void error(char *, ...);
extern void center_text(int width, char * format, ...);
extern void write_fd(int fd, char * format, ...);

#endif