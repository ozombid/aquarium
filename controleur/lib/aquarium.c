#include "aquarium.h"

static const struct aquarium mark_end = {"end of list", -1, -1, NULL, NULL, NULL, NULL}; 
#define SENTINEL ((struct aquarium *) &mark_end)

// moves
int random_move(struct fish * f, struct view * v, struct aquarium * a) // v not used
{
    f->status = NOT_STARTED;
    f->final_shape.x = 0 + (rand() % (a->width + 1));
    f->final_shape.y = 0 + (rand() % (a->height + 1));
    // if out of view to c1 view > delFish + addFish (not add fish) with move st dt = 0 start fish > c1
    // or adapt server : change the move to 2 moves ??
    return 5;
}
int random_inside_move(struct fish * f, struct view * v, struct aquarium * a) // a not used
{
    f->status = NOT_STARTED;
    f->final_shape.x = v->frame.x + (rand() % (v->frame.width + 1));
    f->final_shape.y = v->frame.y + (rand() % (v->frame.height + 1));
    return 5;
}

// shape
struct shape shape_create(char* str, const char* delim, struct view * v)
{
    int ints[4];
    int count = 0;
    char* token = strtok(str, delim);
    while (token != NULL && count < 4) {
        ints[count] = atoi(token);
        count++;
        token = strtok(NULL, delim);
    }
    int x = from_percent(ints[0], v->frame.x, v->frame.width);
    int y = from_percent(ints[1], v->frame.y, v->frame.height);
    return (struct shape){x, y,ints[2],ints[3]};
}


// init
struct aquarium * aquarium_empty() 
{
    struct aquarium * a = malloc(sizeof(struct aquarium));
    *a = (struct aquarium){"begin of list",- 1, -1, NULL, NULL, NULL, SENTINEL};
    return a;
}
struct aquarium * aquarium_init() 
{
    struct view * v = view_empty();
    view_pop("N1",(struct frame){0,0,500,500},v);
    view_pop("N2",(struct frame){500,0,500,500},v);
    view_pop("N3",(struct frame){0,500,500,500},v);
    view_pop("N4",(struct frame){500,500,500,500},v);

    struct fish * f = fish_empty();

    struct move * m = move_empty();
    struct move * m1 = move_create("random", random_move);
    move_pop(m1, m);

    struct aquarium * a = calloc(1,sizeof(struct aquarium));
    //memset(&a, 0, sizeof(a));
    a->name = malloc(MAX_NAMES*sizeof(char));
    strcpy(a->name,"aq1");
    a->width = 1000;
    a->height = 1000;
    a->views = v;
    a->fishes = f;
    a->moves = m;
    a->next = NULL;

    return a;
}

void aquarium_save(struct aquarium * a)
{
    // export
    FILE * fp = fopen("aquarium.txt", "a"); // open the file for writing
    if (fp == NULL)  error("Error : cannot open file\n");
    char* result = aquarium_show(a);
    int n = write(fileno(fp), result, strlen(result));
    if (n < 0) error("Error on printing in .txt");
    free(result);
    fclose(fp);
}

// bools
bool aquarium_fit_frame(struct aquarium * a, struct frame f) 
{
    bool is_inside(int x, int y) { 
        return x >= 0 && x <= a->width && y >= 0 && y <= a->height;
    }
    return is_inside(f.x,f.y) && is_inside(f.x+f.width, f.y+f.height);
}
bool aquarium_fit_shape(struct aquarium * a, struct shape s) 
{
    bool is_inside(int x, int y) { 
        return x >= 0 && x <= a->width && y >= 0 && y <= a->height;
    }
    return is_inside(s.x,s.y) && is_inside(s.x+s.width, s.y+s.height);
}
bool view_fit_fish(struct view * v, struct fish * f)
{
    bool is_inside(int x, int y) { 
        return x >= v->frame.x && x <= v->frame.x + v->frame.width &&
            y >= v->frame.y && y <= v->frame.y + v->frame.height;
    }
    return is_inside(f->shape.x,f->shape.y) &&
        is_inside(f->shape.x+f->shape.width, f->shape.y+f->shape.height);
}
bool is_aquarium_end(struct aquarium * a)
{
    return a == SENTINEL;
}
bool is_last_aquarium(struct aquarium * a)
{
    return is_aquarium_end(a->next);
}
bool aquarium_is_empty(struct aquarium * a)
{
    return is_last_aquarium(a);
}

// add / remove / free
void aquarium_pop(struct aquarium * a, struct aquarium * a_list) 
{
    a->next = SENTINEL;
    struct aquarium * ptr = a_list;
    while (!is_last_aquarium(ptr)) ptr = ptr->next;
    ptr->next = a;
}
void aquarium_push(struct aquarium * a, struct aquarium * a_list) 
{
    a->next = a_list->next;;
    a_list->next = a;
}
void aquarium_remove(struct aquarium * a, struct aquarium * a_list) 
{
    struct aquarium * ptr = a_list;
    while (ptr->next != a) ptr = ptr->next;
    struct aquarium * tmp = ptr->next->next;
    ptr->next = tmp;
    move_free(a->moves);
    view_free(a->views);
    fish_free(a->fishes);
    free(a->name);
    free(a);
}
void aquarium_free(struct aquarium * a_list)
{
    struct aquarium * ptr = a_list->next;
    while (!is_aquarium_end(ptr)) {
        struct aquarium * tmp = ptr->next;
        move_free(ptr->moves);
        view_free(ptr->views);
        fish_free(ptr->fishes);
        free(ptr->name);
        free(ptr);
        ptr = tmp;
    }
    free(a_list);
}

// find / size
struct aquarium * aquarium_find(char* name, struct aquarium * a_list) 
{
    if (aquarium_is_empty(a_list)) return NULL;
    struct aquarium * ptr = a_list;
    while (!is_aquarium_end(ptr)) {
        if (!strcmp(ptr->name, name)) return ptr;
        ptr = ptr->next;
    }
    return NULL;
}
size_t aquarium_size(struct aquarium * a_list)
{
    size_t count = 0;
    struct aquarium * ptr = a_list;
    while (!is_last_aquarium(ptr)) {
        count++;
        ptr = ptr->next;
    }
    return count;
}

// show
char* aquarium_show(struct aquarium * a) // freed
{
    char* result = (char*) calloc(BUFFER_SIZE,sizeof(char)); 
    char* views = views_show(a->views);
    char* fishes = fishes_show(a->fishes);
    sprintf(result, "\n---- %s ----\n%d x %d \nviews : %s\nfishes : %s", 
            a->name, a->width, a->height, views, fishes);
    free(views);
    free(fishes);
    return result;
}


































/*
void get_saved_aquarium() 
{
    struct stat st;
    if (stat("aquarium.dat", &st) == -1) 
        error("Error: cannot stat file");
    // calculate number of aquariums in file
    int num_aquarium = st.st_size / sizeof(struct aquarium);
    printf("There are %d aquarium in data.\n", num_aquarium);
}

bool aquarium_find(char* name, struct aquarium * a)
{
    bool found = false;
    FILE *f = fopen("aquarium.dat", "rb"); 
    if (f == NULL) error("Error : cannot open file\n");
    while (fread(a, sizeof(struct aquarium), 1, f) == 1) {
        if (!strcmp(a->name, name)) {
            found = true; break; }
    }
    fclose(f);
    return found;
}

struct aquarium * aquarium_copy(struct aquarium * a)
{
    struct aquarium * cpy_a = malloc(sizeof(struct aquarium));
    cpy_a->name = malloc(MAX_NAMES*sizeof(char));
    strcpy(cpy_a->name,a->name);
    cpy_a->width = a->width;
    cpy_a->height = a->height;
    cpy_a->views = a->views;
    cpy_a->fishes = a->fishes;
    cpy_a->moves = a->moves;
    cpy_a->next = NULL;
    return cpy_a;
}

void aquarium_save(struct aquarium * a)
{
    // push to data
    FILE * f = fopen("aquarium.dat", "ab");
    if (f == NULL) error("Error : cannot open file\n");
    fwrite(a, sizeof(struct aquarium), 1, f); 
    fclose(f); 
    // export
    FILE * fp = fopen("aquarium.txt", "a"); // open the file for writing
    if (fp == NULL)  error("Error : cannot open file\n");
    char* result = aquarium_show(a);
    int n = write(fileno(fp), result, strlen(result));
    if (n < 0) error("Error on printing in .txt");
    free(result);
    fclose(fp);
}

*/