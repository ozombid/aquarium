#include "fish.h"

static const struct shape no_shape = {-1,-1,-1,-1}; 
#define NO_SHAPE no_shape

static const struct move no_move = {"no move", NULL, NULL}; 
#define NO_MOVE no_move

static const struct fish mark_end = {"end of list", NOT_STARTED, NO_SHAPE, NO_SHAPE, NO_MOVE, NULL}; 
#define SENTINEL ((struct fish *) &mark_end)


// we do not return the adress of a local variable
// we do not stock the adress of a local variable outside the function
struct fish * fish_empty() 
{
    struct fish * f = malloc(sizeof(struct fish));
    *f = (struct fish){"begin of list", NOT_STARTED, NO_SHAPE, NO_SHAPE, NO_MOVE, SENTINEL};
    return f;
}

struct fish * fish_create(char* name, struct shape s, struct move m)
{
    struct fish * f = calloc(1,sizeof(struct fish));
    f->name = malloc(MAX_NAMES*sizeof(char));
    strcpy(f->name, name);
    f->shape = s;
    f->final_shape = s;
    f->move = m;
    f->status = STARTED; //! change here 
    f->next = NULL;
    return f;
}

bool is_fish_end(struct fish * f)
{
    return f == SENTINEL;
}


bool is_last_fish(struct fish * f)
{
    return is_fish_end(f->next);
}

bool fish_is_empty(struct fish * f)
{
    return is_last_fish(f);
}

void fish_pop(char* name, struct shape s, struct move m, struct fish * f_list) 
{
    struct fish * f = fish_create(name, s, m);
    f->next = SENTINEL;
    struct fish * ptr = f_list;
    while (!is_last_fish(ptr)) ptr = ptr->next;
    ptr->next = f;
}

void fish_push(char* name, struct shape s, struct move m, struct fish * f_list) 
{
    struct fish * f = fish_create(name, s, m);
    f->next = f_list->next;;
    f_list->next = f;
}

void fish_remove(struct fish * f, struct fish * f_list) 
{
    struct fish * ptr = f_list;
    while (ptr->next != f) ptr = ptr->next;
    struct fish * tmp = ptr->next->next;
    ptr->next = tmp;
    free(f->name);
    free(f);
}

struct fish * fish_find(char* name, struct fish * f_list) 
{
    if (fish_is_empty(f_list)) return NULL;
    struct fish * ptr = f_list;
    while (!is_fish_end(ptr)) {
        if (!strcmp(ptr->name,name)) return ptr;
        ptr = ptr->next;
    }
    return NULL;
}

size_t fish_size(struct fish * f_list)
{
    size_t count = 0;
    struct fish * ptr = f_list;
    while (!is_last_fish(ptr)) {
        count++;
        ptr = ptr->next;
    }
    return count;
}

void fish_free(struct fish * f_list)
{
    struct fish * ptr = f_list->next;
    while (!is_fish_end(ptr)) {
        struct fish * tmp = ptr->next;
        free(ptr->name);
        free(ptr);
        ptr = tmp;
    }
    free(f_list);
}

bool fish_fit_view(struct fish * f, struct view * v) // shape - shape
{
    bool is_inside(int x, int y) { 
        return x >= v->frame.x && x <= v->frame.x + v->frame.width &&
            y >= v->frame.y && y <= v->frame.y + v->frame.height;
    }
    return is_inside(f->shape.x,f->shape.y) &&
        is_inside(f->shape.x+f->shape.width, f->shape.y+f->shape.height);
}

char* fish_show(struct fish * f, struct view * v)
{
    char* result = (char*) malloc(50*sizeof(char));
    char* status = f->status == STARTED ? "started" : "not started";
    int x = to_percent(f->shape.x, v->frame.x, v->frame.width);
    int y = to_percent(f->shape.y, v->frame.y, v->frame.height);
    sprintf(result, "%s : %s : %d x %d + %d + %d %s", f->name, v->name, 
        x, y, f->shape.width, f->shape.height, status);
    return result;
}

char* fishes_show(struct fish * f_list, struct view * v_list) 
{
    char* result = (char*) calloc(BUFFER_SIZE,sizeof(char)); 
    if (fish_is_empty(f_list)) strcat(result, "empty list of fishes");
    else {
        sprintf(result, "%ld fishes in aquarium", fish_size(f_list));
        struct fish * f_ptr = f_list->next;
        while (!is_fish_end(f_ptr)) {
            struct view * v_ptr = v_list->next;
            while (!is_view_end(v_ptr)) {
                if (fish_fit_view(f_ptr, v_ptr)) {
                    char* line = fish_show(f_ptr, v_ptr);
                    strcat(result, "\n");
                    strcat(result, line);
                    free(line); 
                    // break;
                }
                v_ptr = v_ptr->next;
            }
            f_ptr = f_ptr->next;
        }
    }
    return result;
}

void fish_start(struct fish * f) 
{
    if (f->status == NOT_STARTED) {
        f->shape.x = f->final_shape.x;
        f->shape.y = f->final_shape.y;
        f->status = STARTED;
    }
    //else assert (f->final_shape == f->shape);
}


