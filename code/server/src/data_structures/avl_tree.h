#ifndef AVL_TREE_HEADER
#define AVL_TREE_HEADER

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
#include "../client/client_struct.h"

#define AVL_OK 1
#define AVL_DUPLICATED_KEY -1

typedef struct avl_node avl_node_t;
typedef struct avl_tree avl_tree_t;

struct avl_node {
    int height;
    void * data;
    avl_node_t * left;
    avl_node_t * right;
};

struct avl_tree {
    size_t size;
    avl_node_t * root;
    
    int fd;

    int (* data_cmp)(void *, void *);
    void (* print_data)(void *);
};

extern avl_tree_t * new_avl_tree(int (* data_cmp)(void *, void *), void (* print_data)(void *), int fd);
extern avl_node_t * new_avl_node(void * data, size_t data_size, int free_data);

extern int avl_add(avl_tree_t * tree, void * node, size_t data_size, int free_data, int write_data);
extern void avl_remove(avl_tree_t * tree, void * node);
extern void avl_print(avl_tree_t * tree);
extern void avl_print_client(avl_tree_t * tree, void (* send_to_client)(void *, int socket), int socket);

extern void * avl_get(avl_tree_t * tree, void * data);

#endif