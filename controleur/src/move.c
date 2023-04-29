#include "move.h"

static const struct move mark_end = {"end of list", NULL, NULL}; 
#define SENTINEL ((struct move *) &mark_end)


struct move * move_empty() 
{
    struct move * m = malloc(sizeof(struct move));
    *m = (struct move){"begin of list", NULL, SENTINEL};
    return m;
}

struct move * move_create(char* name, int (*move)(struct fish *,  struct view *, struct aquarium *))
{
    struct move * m = calloc(1,sizeof(struct move));
    m->name = malloc(MAX_NAMES*sizeof(char));
    strcpy(m->name, name);
    m->move = move;
    m->next = NULL;
    return m;
}

bool is_move_end(struct move * m)
{
    return m == SENTINEL;
}


bool is_last_move(struct move * m)
{
    return is_move_end(m->next);
}

bool move_is_empty(struct move * m)
{
    return is_last_move(m);
}

void move_pop(struct move * m, struct move * m_list) 
{
    m->next = SENTINEL;
    struct move * ptr = m_list;
    while (!is_last_move(ptr)) ptr = ptr->next;
    ptr->next = m;
}

void move_push(struct move * m, struct move * m_list) 
{
    m->next = m_list->next;;
    m_list->next = m;
}

void move_remove(struct move * m, struct move * m_list) 
{
    struct move * ptr = m_list;
    while (ptr->next != m) ptr = ptr->next;
    struct move * tmp = ptr->next->next;
    ptr->next = tmp;
    free(m->name);
    free(m);
}

struct move * move_find(char* name, struct move * m_list) 
{
    if (move_is_empty(m_list)) return NULL;
    struct move * ptr = m_list;
    while (!is_move_end(ptr)) {
        if (!strcmp(ptr->name,name)) return ptr;
        ptr = ptr->next;
    }
    return NULL;
}

size_t move_size(struct move * m_list)
{
    size_t count = 0;
    struct move * ptr = m_list;
    while (!is_last_move(ptr)) {
        count++;
        ptr = ptr->next;
    }
    return count;
}

void move_free(struct move * m_list)
{
    struct move * ptr = m_list->next;
    while (!is_move_end(ptr)) {
        struct move * tmp = ptr->next;
        free(ptr->name);
        free(ptr);
        ptr = tmp;
    }
    free(m_list);
}

char* move_show(struct move m) // freed in show_aquarium
{
    char* result = (char*) malloc(50*sizeof(char));
    sprintf(result, "%s \n", m.name); 
    return result;
}
