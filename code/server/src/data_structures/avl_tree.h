#ifndef AVL_TREE_HEADER
#define AVL_TREE_HEADER

#include <stdlib.h>
#include <string.h>

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

    int (* data_cmp)(void *, void *);
    void (* print_data)(void *);
};

extern avl_tree_t * new_avl_tree(int (* data_cmp)(void *, void *), void (* print_data)(void *));
extern avl_node_t * new_avl_node(void * data, size_t data_size);

extern void avl_add(avl_tree_t * tree, void * node, size_t data_size);
extern void avl_remove(avl_tree_t * tree, void * node);
extern void avl_print(avl_tree_t * tree);

extern void * avl_get(avl_tree_t * tree, void * data);

#endif