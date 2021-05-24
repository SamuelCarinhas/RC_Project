#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h> 
#include <sys/wait.h>
#include "data_structures/avl_tree.h"
#include "utils/string.h"
#include "admin/admin_server.h"

void create_proccess(void ( *function)(void)) {
    if(fork() == 0) {
        function();
        exit(0);
    }
}

void wait_for_children() {
    while(wait(NULL) != -1);
}

void print_node(void * data) {
    printf("%s\n", (char *) data);
}

int data_cmp(void * a, void * b) {
    return strcmp((char *) a, (char *) b);
}

int main() {
    create_proccess(admin_server);
    
    wait_for_children();

    return 0;
}