#include "view.h"

static const struct frame no_frame = {-1,-1,-1,-1}; 
#define NO_FRAME no_frame

static const struct view mark_end = {"end of list", AVAILABLE, NO_FRAME, NULL}; 
#define SENTINEL ((struct view *) &mark_end)

// we do not return the adress of a local variable
// we do not stock the adress of a local variable outside the function
struct view * view_empty() 
{
    struct view * v = malloc(sizeof(struct view));
    *v = (struct view){"begin of list", -1, NO_FRAME, SENTINEL};
    return v;
}

struct view * view_create(char* name, struct frame f) 
{
    struct view * v = calloc(1,sizeof(struct view));
    v->name = malloc(MAX_NAMES*sizeof(char));
    strcpy(v->name, name);
    v->frame = f;
    v->status = AVAILABLE;
    v->next = NULL;
    return v;
}

bool is_view_end(struct view * v)
{
    return v == SENTINEL;
}

bool is_last_view(struct view * v)
{
    return is_view_end(v->next);
}

bool view_is_empty(struct view * v)
{
    return is_last_view(v);
}

bool view_is_taken(struct view * v)
{
    return v->status == TAKEN;
}

void view_pop(char* name, struct frame f, struct view * v_list) 
{
    struct view * v = view_create(name, f);
    v->next = SENTINEL;
    struct view * ptr = v_list;
    while (!is_last_view(ptr)) ptr = ptr->next;
    ptr->next = v;
}

void view_push(char* name, struct frame f, struct view * v_list) 
{
    struct view * v = view_create(name,f);
    v->next = v_list->next;;
    v_list->next = v;
}

void view_remove(struct view * v, struct view * v_list) 
{
    struct view * ptr = v_list;
    while (ptr->next != v) ptr = ptr->next;
    struct view * tmp = ptr->next->next;
    ptr->next = tmp;
    free(v->name);
    free(v);
}

struct view * view_find(char* name, struct view * v_list) 
{
    if (view_is_empty(v_list)) return NULL;
    struct view * ptr = v_list;
    while (!is_view_end(ptr)) {
        if (!strcmp(ptr->name,name)) return ptr;
        ptr = ptr->next;
    }
    return NULL;
}

struct view * view_find_available(struct view * v_list)
{
    if (view_is_empty(v_list)) return NULL;
    struct view * ptr = v_list;
    while (!is_view_end(ptr)) {
        if (!view_is_taken(ptr)) return ptr;
        ptr = ptr->next;
    }
    return NULL;
}

size_t view_size(struct view * v_list)
{
    size_t count = 0;
    struct view * ptr = v_list;
    while (!is_last_view(ptr)) {
        count++;
        ptr = ptr->next;
    }
    return count;
}

void view_free(struct view * v_list)
{
    struct view * ptr = v_list->next;
    while (!is_view_end(ptr)) {
        struct view * tmp = ptr->next;
        free(ptr->name);
        free(ptr);
        ptr = tmp;
    }
    free(v_list);
}

void view_take(struct view * v)
{
    v->status = TAKEN;
}

void view_release(struct view * v)
{
    v->status = AVAILABLE;
}

char* frame_show(struct frame f) // freed in show_view
{
    char* result = (char*) malloc(40*sizeof(char)); // allocate memory for the string
    sprintf(result, "%d x %d + %d + %d", f.x, f.y, f.width, f.height); 
    return result; // return the formatted string
}

char* view_show(struct view v) 
{
    char* result = (char*) malloc(50*sizeof(char));
    char* frame = frame_show(v.frame);
    char* status = v.status == AVAILABLE ? "available" : "taken";
    sprintf(result, "%s : %s : %s", v.name, frame, status); 
    free(frame);
    return result;
}

char* views_show(struct view * v_list) 
{
    char* result = (char*) calloc(BUFFER_SIZE,sizeof(char));
    if (view_is_empty(v_list)) {
        strcat(result, "empty list of views");
    }
    else {
        sprintf(result, "%ld views of aquarium", view_size(v_list));
        struct view * ptr = v_list->next;
        while(!is_view_end(ptr)) {
            char* view_str = view_show(*ptr);
            strcat(result, "\n");
            strcat(result, view_str);
            free(view_str);
            ptr = ptr->next;
        }
    }
    return result;
}

struct frame frame_create(char* str, const char* delim)
{
    int ints[4];
    int count = 0;
    char* token = strtok(str, delim);
    while (token != NULL && count < 4) {
        ints[count] = atoi(token);
        count++;
        token = strtok(NULL, delim);
    }
    return (struct frame){ints[0],ints[1],ints[2],ints[3]};
}



