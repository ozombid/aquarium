#include "move.h"

#define STARTED true
#define NOT_STARTED false 

struct shape {
    int x;
    int y;
    int width;
    int height;
};

struct fish {
    char* name;
    bool status;
    struct shape shape;
    struct move move;
    struct fish * next;
};


struct fish * fish_empty();

struct fish * fish_find(char* name, struct fish * f_list);

struct fish * fish_create(char* name, struct shape s, struct move m);

size_t fish_size(struct fish * f_list);

bool fish_is_empty(struct fish * f_list);

bool is_last_fish(struct fish * f);

bool is_fish_end(struct fish * f);

char* shape_show(struct shape s); 

char* fish_show(struct fish f);

char* fishes_show(struct fish * f_list);

void fish_pop(char* name, struct shape s, struct move m, struct fish * f_list);

void fish_push(char* name, struct shape s, struct move m, struct fish * f_list);

void fish_remove(struct fish * f, struct fish * f_list);

void fish_free(struct fish * f_list);

bool fish_start(struct fish * f);

struct shape get_shape(char* str, const char* delim);

