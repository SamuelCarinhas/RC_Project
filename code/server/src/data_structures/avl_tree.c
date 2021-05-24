#include "avl_tree.h"

static int get_height(avl_node_t * node);
static int get_balance(avl_node_t * node);

static avl_node_t * left_rotation(avl_node_t * node);
static avl_node_t * right_rotation(avl_node_t * node);

static avl_node_t * private_avl_get(avl_tree_t * tree, avl_node_t * current, void * data);
static avl_node_t * private_avl_remove(avl_tree_t * tree, avl_node_t * current, void * data);
static avl_node_t * private_avl_add(avl_tree_t * tree, avl_node_t * new_node, avl_node_t * current_node);

static void private_avl_print(avl_tree_t * tree, avl_node_t * node);
static void private_avl_rewrite(avl_tree_t * tree, avl_node_t * current, size_t data_size);
static void private_avl_print_client(avl_tree_t * tree, avl_node_t * node, void (* send_to_client)(void *, int socket), int socket);

avl_tree_t * new_avl_tree(int (* data_cmp)(void *, void *), void (* print_data)(void *), int fd) {
    avl_tree_t * tree = (avl_tree_t *) malloc(sizeof(avl_tree_t));

    tree->fd = fd;
    tree->size = 0;
    tree->root = NULL;
    tree->data_cmp = data_cmp;
    tree->print_data = print_data;

    return tree;
}

avl_node_t * new_avl_node(void * data, size_t data_size, int free_data) {
    avl_node_t * node = (avl_node_t *) malloc(sizeof(avl_node_t));

    node->height = 1;
    node->data = data;
    node->data = malloc(data_size);
    memcpy(node->data, data, data_size);
    node->left = NULL;
    node->right = NULL;

    if(free_data)
        free(data);

    return node;
}

int avl_add(avl_tree_t * tree, void * data, size_t data_size, int free_data, int write_data) {
    if(private_avl_get(tree, tree->root, data) != NULL)
        return AVL_DUPLICATED_KEY;
    
    if(write_data)
        write(tree->fd, data, data_size);

    avl_node_t * node = new_avl_node(data, data_size, free_data);
    tree->root = private_avl_add(tree, node, tree->root);
    tree->size++;

    return AVL_OK;
}

int avl_remove(avl_tree_t * tree, void * data, size_t data_size, int free_data, int write_data) {
    if(private_avl_get(tree, tree->root, data) == NULL)
        return AVL_KEY_NOT_FOUND;

    tree->root = private_avl_remove(tree, tree->root, data);

    if(free_data)
        free(data);
    
    if(write_data) {
        ftruncate(tree->fd, 0);
        private_avl_rewrite(tree, tree->root, data_size);
    }

    return AVL_OK;
}

void * avl_get(avl_tree_t * tree, void * data) {
    return private_avl_get(tree, tree->root, data);
}

void avl_print(avl_tree_t * tree) {
    private_avl_print(tree, tree->root);
}

void avl_print_client(avl_tree_t * tree, void (* send_to_client)(void *, int socket), int socket) {
    private_avl_print_client(tree, tree->root, send_to_client, socket);
}

static void private_avl_rewrite(avl_tree_t * tree, avl_node_t * current, size_t data_size) {
    if(current == NULL)
        return;
    private_avl_rewrite(tree, current->left, data_size);
    write(tree->fd, current->data, data_size);
    private_avl_rewrite(tree, current->right, data_size);
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

static avl_node_t * private_avl_remove(avl_tree_t * tree, avl_node_t * current, void * data) {
    if(current == NULL)
        return current;

    int res = tree->data_cmp(data, current->data);

    if(res < 0)
        current->left = private_avl_remove(tree, current->left, data);
    else if(res > 0)
        current->right = private_avl_remove(tree, current->right, data);
    else {
        if(current->left == NULL || current->right == NULL) {
            avl_node_t * temp = (current->left != NULL) ? current->left : current->right;

            if(temp == NULL) {
                temp = current;
                current = NULL;
            } else
                *current = *temp;

            free(temp);
        }
    }

    if(current == NULL)
        return current;
    
    current->height = 1 + (get_height(current->left) > get_height(current->right)) ? get_height(current->left) : get_height(current->right);

    int balance = get_balance(current);

    if(balance > 1) {
        if(get_balance(current->left) < 0)
            current->left = left_rotation(current->left);
        return right_rotation(current);
    } else if(balance < -1) {
        if(get_balance(current->right) > 0)
            current->right = right_rotation(current->right);
        return left_rotation(current);
    }

    return current;
}

static avl_node_t * private_avl_get(avl_tree_t * tree, avl_node_t * current, void * data) {
    if(current == NULL)
        return NULL;
    
    int res = tree->data_cmp(data, current->data);

    if(res < 0)
        return private_avl_get(tree, current->left, data);
    else if(res > 0)
        return private_avl_get(tree, current->right, data);

    return current;
}

static void private_avl_print(avl_tree_t * tree, avl_node_t * node) {
    if(node == NULL)
        return;
    private_avl_print(tree, node->left);
    tree->print_data(node->data);
    private_avl_print(tree, node->right);
}

static void private_avl_print_client(avl_tree_t * tree, avl_node_t * node, void (* send_to_client)(void *, int socket), int socket) {
    if(node == NULL)
        return;
    private_avl_print_client(tree, node->left, send_to_client, socket);
    send_to_client(node->data, socket);
    private_avl_print_client(tree, node->right, send_to_client, socket);
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

