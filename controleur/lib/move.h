#include "utils.h"

struct move {
    char* name;
    struct move * next;
};


struct move * move_empty();

struct move * move_find(char* name, struct move * m_list);

struct move * move_create(char* name);

size_t move_size(struct move * m_list);

bool move_is_empty(struct move * m_list);

bool is_last_move(struct move * m);

bool is_move_end(struct move * m);

char* move_show(struct move m);

void move_pop(char* name, struct move * m_list);

void move_push(char* name, struct move * m_list);

void move_remove(struct move * m, struct move * m_list);

void move_free(struct move * m_list);
