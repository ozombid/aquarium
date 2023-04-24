#include "fish.h"

static const struct shape no_shape = {-1,-1,-1,-1}; 
#define NO_SHAPE no_shape

static const struct move no_move = {"no move", NULL}; 
#define NO_MOVE no_move

static const struct fish mark_end = {"end of list", NOT_STARTED, NO_SHAPE, NO_MOVE, NULL}; 
#define SENTINEL ((struct fish *) &mark_end)


// we do not return the adress of a local variable
// we do not stock the adress of a local variable outside the function
struct fish * fish_empty() 
{
    struct fish * f = malloc(sizeof(struct fish));
    *f = (struct fish){"begin of list", NOT_STARTED, NO_SHAPE, NO_MOVE, SENTINEL};
    return f;
}

struct fish * fish_create(char* name, struct shape s, struct move m)
{
    struct fish * f = calloc(1,sizeof(struct fish));
    f->name = malloc(MAX_NAMES*sizeof(char));
    strcpy(f->name, name);
    f->shape = s;
    f->move = m;
    f->status = NOT_STARTED;
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

char* shape_show(struct shape s) // freed in show_fish
{
    char* result = (char*) malloc(40*sizeof(char)); // allocate memory for the string
    sprintf(result, "%d x %d + %d + %d", s.x, s.y, s.width, s.height); 
    return result; // return the formatted string
}

char* fish_show(struct fish f)
{
    char* result = (char*) malloc(50*sizeof(char));
    char* shape = shape_show(f.shape);
    char* status = f.status == STARTED ? "started" : "not started";
    sprintf(result, "%s : %s %s", f.name, shape, status); 
    free(shape);
    return result;
}

char* fishes_show(struct fish * f_list) 
{
    char* result = (char*) calloc(BUFFER_SIZE,sizeof(char)); 
    if (fish_is_empty(f_list)) {
        strcat(result, "empty list of fishes");
    }
    else {
        sprintf(result, "%ld fishes in aquarium", fish_size(f_list));
        struct fish * ptr = f_list->next;
        while(!is_fish_end(ptr)) {
            char* fish_str = fish_show(*ptr); 
            strcat(result, "\n");
            strcat(result, fish_str); 
            free(fish_str); 
            ptr = ptr->next;
        }
    }
    return result;
}

void fish_start(struct fish * f) {
    if (f->status != STARTED) f->status = STARTED;
}

struct shape shape_create(char* str, const char* delim)
{
    int ints[4];
    int count = 0;
    char* token = strtok(str, delim);
    while (token != NULL && count < 4) {
        ints[count] = atoi(token);
        count++;
        token = strtok(NULL, delim);
    }
    return (struct shape){ints[0],ints[1],ints[2],ints[3]};
}

