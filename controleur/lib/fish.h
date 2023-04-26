#include "move.h"
#include "view.h"

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
    struct shape final_shape;
    struct move move;
    struct fish * next;
};

// init
struct fish * fish_empty();

struct fish * fish_create(char* name, struct shape s, struct move m);

// find
struct fish * fish_find(char* name, struct fish * f_list);

// size
size_t fish_size(struct fish * f_list);

// bools
bool fish_is_empty(struct fish * f_list);

bool is_last_fish(struct fish * f);

bool is_fish_end(struct fish * f);

// add / remove 
void fish_pop(char* name, struct shape s, struct move m, struct fish * f_list);

void fish_push(char* name, struct shape s, struct move m, struct fish * f_list);

void fish_remove(struct fish * f, struct fish * f_list);

// start / free
void fish_start(struct fish * f);

void fish_free(struct fish * f_list);

// show
bool fish_fit_view(struct fish * f, struct view * v); // shape - shape

char* fish_show(struct fish * f, struct view * v);

char* fishes_show(struct fish * f_list, struct view * v_list);

