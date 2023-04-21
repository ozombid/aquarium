#include "utils.h"

struct node {
    bool is_leaf;
    bool (*syntax)(char*);
    int num_children;
    struct node** children;
};

// tree implementation
struct node * create_leaf(bool (*syntax)(char*));
struct node * create_node(int num_children, struct node** children, bool (*syntax)(char*));
bool node_traverse(struct node * n, char** words);

// servers prompt syntax
bool control_server_syntax(char** words);
