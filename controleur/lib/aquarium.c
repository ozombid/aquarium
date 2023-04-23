#include "aquarium.h"

static const struct aquarium mark_end = {"end of list", -1, -1, NULL, NULL, NULL, NULL}; 
#define SENTINEL ((struct aquarium *) &mark_end)

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
    move_pop("random", m);
    move_pop("linear", m);

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