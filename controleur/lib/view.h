#include "utils.h"

#define TAKEN true
#define AVAILABLE false 

struct frame {
    int x;
    int y;
    int width;
    int height;
};

struct view {
    char* name;
    bool status;
    struct frame frame;
    struct view * next;
};

// init
struct view * view_empty();

struct frame frame_create(char* str, const char* delim);

struct view * view_create(char* name, struct frame f);

// find
struct view * view_find(char* name, struct view * v_list);

struct view * view_find_available(struct view * v_list);

// bools
bool view_is_empty(struct view * v_list);

bool is_last_view(struct view * v);

bool is_view_end(struct view * v);

bool view_is_taken(struct view * v);

// show
char* frame_show(struct frame f); 

char* view_show(struct view v);

char* views_show(struct view * v_list);

// add / remove 
void view_pop(char* name, struct frame f, struct view * v_list);

void view_push(char* name, struct frame f, struct view * v_list);

void view_remove(struct view * v, struct view * v_list);

// take / release / free
void view_take(struct view * v);

void view_release(struct view * v);

void view_free(struct view * v_list);

// size
size_t view_size(struct view * v_list);





