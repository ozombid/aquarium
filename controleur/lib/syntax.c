#include "syntax.h"

bool node_traverse(struct node * n, char** words) 
{
    int length = get_array_length(words);
    bool rec(struct node * N, int i) {
        if (N->is_leaf && i == length) return true;
        if (N->is_leaf || i == length) return false;
        for (int j = 0; j < N->num_children; j++) 
        {
            bool (*func)(char*) = N->children[j]->syntax;
            if (func(words[i])) return rec(N->children[j], i+1);
        }
        return false;
    }
    return rec(n, 0);
}

struct node * create_leaf(bool (*syntax)(char*)) 
{
    struct node * n = malloc(sizeof(struct node));
    n->is_leaf = true;
    n->num_children = 0;
    n->children = NULL;
    n->syntax = syntax;
    return n;
}

struct node * create_node(int num_children, struct node** children, bool (*syntax)(char*)) 
{
    struct node * n = malloc(sizeof(struct node));
    n->is_leaf = false;
    n->num_children = num_children;
    n->children = children;
    n->syntax = syntax;
    return n;
}


bool control_server_syntax(char** words) 
{
    // syntaxes
        bool is_hello(char* s)  { return !strcmp(s,"hello"); }
        bool is_dot(char* s)  { return !strcmp(s,"."); }
        bool is_as(char* s)     { return !strcmp(s,"as"); }
        bool is_down(char* s)     { return !strcmp(s,"down"); }
        bool is_in(char* s)     { return !strcmp(s,"in"); }
        bool is_load(char* s)   { return !strcmp(s,"load"); }
        bool is_add(char* s)    { return !strcmp(s,"add"); }
        bool is_view(char* s)   { return !strcmp(s,"view"); }
        bool is_fish(char* s)   { return !strcmp(s,"fish"); }
        bool is_del(char* s)    { return !strcmp(s,"del"); }
        bool is_save(char* s)   { return !strcmp(s,"save"); }
        bool is_show(char* s)   { return !strcmp(s,"show"); }
        bool is_status(char* s) { return !strcmp(s,"status"); }
        bool is_start(char* s) { return !strcmp(s,"start"); }
        bool is_bye(char* s)    { return !strcmp(s,"bye"); }
        bool is_string(char* s) { (void) s; return true; }
        bool is_dim(char* s)    { (void) s; return true; }
    // leafs 
        struct node * string = create_leaf(is_string);
        struct node * dim = create_leaf(is_dim);
        struct node * dim_a[] = {dim};
        struct node * string_a[] = {string};
    // nodes
        // down
        struct node * down = create_leaf(is_down);
        // show
        struct node * show = create_leaf(is_show);
        // status
        struct node * status = create_leaf(is_status);
        // load
        struct node * load = create_node(1,string_a,is_load);
        // save
        struct node * save = create_node(1,string_a,is_save);
        // start
        struct node * start = create_node(1,string_a,is_start);
        // add
        struct node * add_2 = create_node(1,dim_a,is_string);
        struct node * add_2a[] = {add_2};
        struct node * add_4 = create_node(1,string_a,is_dim);
        struct node * add_4a[] = {add_4};
        struct node * add_3 = create_node(1,add_4a,is_string);
        struct node * add_3a[] = {add_3};
        struct node * add_11 = create_node(1,add_2a,is_view);
        struct node * add_12 = create_node(1,add_3a,is_fish);
        struct node * add_1a[] = {add_11, add_12};
        struct node * add = create_node(2,add_1a,is_add);
        // del
        struct node * del_11 = create_node(1,string_a,is_view);
        struct node * del_12 = create_node(1,string_a,is_fish);
        struct node * del_1a[] = {del_11, del_12};
        struct node * del = create_node(2,del_1a,is_del);
        // hello 
        struct node * in = create_node(1,string_a,is_in);
        struct node * in_a[] = {in};
        struct node * as = create_node(1,in_a,is_as);
        struct node * dot = create_leaf(is_dot);
        struct node * hello_a[] = {dot, as};
        struct node * hello = create_node(2,hello_a,is_hello);
        // bye
        struct node * close = create_leaf(is_bye);
        // cmds
        struct node * dollar_cmds[] = {load,save,show,status,add,del,start,close,hello,down};
        // dollar
        struct node * dollar = create_node(10,dollar_cmds,is_string);

    // traverse
        bool res = node_traverse(dollar,words);

    // free
        free(dim); free(string);
        free(show); free(status);
        free(save); free(start);
        free(load); free(down);
        free(hello); free(dot); free(as); free(in);
        free(add_4); free(add_3); free(add_2); free(add_11); free(add_12); free(add);
        free(del_11); free(del_12); free(del);
        free(dollar); free(close);

    return res;
}


/*
        // bye
        struct node * close = create_leaf(is_bye);
        // root
        struct node * root_cmds[] = {dollar, close};
        struct node * cmd = create_node(2,root_cmds,is_string);*/