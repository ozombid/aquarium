#include "view.h"
#include "fish.h"

struct aquarium {
    char* name;
    int width;
    int height;
    struct view * views;
    struct fish * fishes;
    struct move * moves;
    struct aquarium * next;
};

// init
struct aquarium * aquarium_empty();

struct aquarium * aquarium_init();

// find
struct aquarium * aquarium_find(char* name, struct aquarium * a_list);

// bools
bool aquarium_is_empty(struct aquarium * a);

bool is_aquarium_end(struct aquarium * a);

bool is_last_aquarium(struct aquarium * a);

bool aquarium_fit_frame(struct aquarium * a, struct frame f);

bool aquarium_fit_shape(struct aquarium * a, struct shape s);

// show
char* aquarium_show(struct aquarium * a);

void aquarium_save(struct aquarium * a);

void aquarium_free(struct aquarium * a_list);

// not used here

// size
size_t aquarium_size(struct aquarium * a_list);

// add / remove
void aquarium_pop(struct aquarium * a, struct aquarium * a_list);

void aquarium_push(struct aquarium * a, struct aquarium * a_list);

void aquarium_remove(struct aquarium * a, struct aquarium * a_list);
