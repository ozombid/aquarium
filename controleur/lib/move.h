#include "utils.h"

struct fish;
struct view;
struct aquarium;

struct move {
    char* name;
    int (*move)(struct fish *, struct view *, struct aquarium *); //! side effect on in final_shape and status
    struct move * next;
};

// init
struct move * move_empty();

struct move * move_create(char* name, int (*move)(struct fish *,  struct view *, struct aquarium *));

// find / size
struct move * move_find(char* name, struct move * m_list);

size_t move_size(struct move * m_list);

// bools
bool move_is_empty(struct move * m_list);

bool is_last_move(struct move * m);

bool is_move_end(struct move * m);

// add / remove / free
void move_pop(struct move * m, struct move * m_list);

void move_push(struct move * m, struct move * m_list);

void move_remove(struct move * m, struct move * m_list);

void move_free(struct move * m_list);

// show
char* move_show(struct move m);
