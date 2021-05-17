#include "avl_tree.h"

static int get_height(avl_node_t * node);
static int get_balance(avl_node_t * node);

static avl_node_t * right_rotation(avl_node_t * node);
static avl_node_t * left_rotation(avl_node_t * node);

static avl_node_t * private_avl_add(avl_tree_t * tree, avl_node_t * new_node, avl_node_t * current_node);
static avl_node_t * private_avl_remove(void * data);
static void private_avl_print(avl_tree_t * tree, avl_node_t * node);
static void * private_avl_get(avl_node_t * node);
static void private_avl_print_client(avl_tree_t * tree, avl_node_t * node, void (* send_to_client)(void *, client_t * client), client_t * client);

avl_tree_t * new_avl_tree(int (* data_cmp)(void *, void *), void (* print_data)(void *)) {
    avl_tree_t * tree = (avl_tree_t *) malloc(sizeof(avl_tree_t));

    tree->size = 0;
    tree->root = NULL;
    tree->data_cmp = data_cmp;
    tree->print_data = print_data;

    return tree;
}

avl_node_t * new_avl_node(void * data, size_t data_size) {
    avl_node_t * node = (avl_node_t *) malloc(sizeof(avl_node_t));

    node->height = 1;
    node->data = malloc(data_size);
    memcpy(node->data, data, data_size);
    node->left = NULL;
    node->right = NULL;

    return node;
}

void avl_add(avl_tree_t * tree, void * data, size_t data_size) {
    avl_node_t * node = new_avl_node(data, data_size);
    tree->root = private_avl_add(tree, node, tree->root);
    tree->size++;
}

void avl_remove(avl_tree_t * tree, void * data) {
    tree->root = private_avl_remove(data);
}

void * avl_get(avl_tree_t * tree, void * data) {
    return private_avl_get(tree->root);
}

void avl_print(avl_tree_t * tree) {
    private_avl_print(tree, tree->root);
}

void avl_print_client(avl_tree_t * tree, void (* send_to_client)(void *, client_t * client), client_t * client) {
    private_avl_print_client(tree, tree->root, send_to_client, client);
}

static avl_node_t * private_avl_add(avl_tree_t * tree, avl_node_t * new_node, avl_node_t * current_node) {
    if(current_node == NULL)
        return new_node;

    int res = tree->data_cmp(new_node->data, current_node->data);
    if(res < 0)
        current_node->left = private_avl_add(tree, new_node, current_node->left);
    else if(res > 0)
        current_node->right = private_avl_add(tree, new_node, current_node->right);
    else
        return current_node;
    
    current_node->height = 1 + (get_height(current_node->left) > get_height(current_node->right)) ? get_height(current_node->left) : get_height(current_node->right);

    int balance = get_balance(current_node);

    if(balance > 1) {
        res = tree->data_cmp(new_node->data, current_node->left->data);
        if(res > 0)
            current_node->left = left_rotation(current_node);
        return right_rotation(current_node);
    }

    if(balance < -1) {
        res = tree->data_cmp(new_node->data, current_node->right->data);
        if(res < 0)
            current_node->right = right_rotation(current_node);
        return left_rotation(current_node);
    }

    return current_node;
}

static avl_node_t * private_avl_remove(void * data) {
    return NULL;
}

static void * private_avl_get(avl_node_t * node) {
    return NULL;
}

static void private_avl_print(avl_tree_t * tree, avl_node_t * node) {
    if(node == NULL)
        return;
    private_avl_print(tree, node->left);
    tree->print_data(node->data);
    private_avl_print(tree, node->right);
}

static void private_avl_print_client(avl_tree_t * tree, avl_node_t * node, void (* send_to_client)(void *, client_t * client), client_t * client) {
    if(node == NULL)
        return;
    private_avl_print_client(tree, node->left, send_to_client, client);
    send_to_client(node->data, client);
    private_avl_print_client(tree, node->right, send_to_client, client);
}

static int get_height(avl_node_t * node) {
    return node == NULL ? 0 : node->height;
}

static int get_balance(avl_node_t * node) {
    return node == NULL ? 0 : get_height(node->left) - get_height(node->right);
}

static avl_node_t * right_rotation(avl_node_t * node) {
    avl_node_t * child = node->left;
    avl_node_t * save = child->right;

    child->right = node;
    node->left = save;

    node->height = 1 + (get_height(node->left) > get_height(node->right)) ? get_height(node->left) : get_height(node->right);
    child->height = 1 + (get_height(save->left) > get_height(save->right)) ? get_height(save->left) : get_height(save->right);

    return child;
}

static avl_node_t * left_rotation(avl_node_t * node) {
    avl_node_t * child = node->right;
    avl_node_t * save = child->left;

    child->left = node;
    node->right = save;

    node->height = 1 + (get_height(node->left) > get_height(node->right)) ? get_height(node->left) : get_height(node->right);
    child->height = 1 + (get_height(save->left) > get_height(save->right)) ? get_height(save->left) : get_height(save->right);

    return child;
}

