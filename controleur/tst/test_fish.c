#include "../lib/fish.h"

void show(struct fish * f_list) 
{
    char* result = (char*) calloc(BUFFER_SIZE,sizeof(char)); 
    if (fish_is_empty(f_list)) {
        strcat(result, "empty list");
    }
    else {
        struct fish * ptr = f_list->next;
        while(!is_fish_end(ptr)) {
            char* fish_str = fish_show(*ptr); // get the string representation of the fish
            strcat(result, fish_str); // concatenate the fish string to the result string
            free(fish_str); // free the memory used by the fish string
            ptr = ptr->next;
        }
    }
    printf("%s\n",result);
    free(result);
}

int main()
{
    struct shape f1 = {1,1,1,1};
    struct shape f2 = {2,2,2,2};
    struct shape f3 = {3,3,3,3};
    struct shape f4 = {4,4,4,4};

    struct move m = {"random"};

    struct fish * v1 = fish_empty();
    show(v1); 
    fish_push("el_1",f1,m,v1); // 1
    show(v1);
    fish_push("el_2",f2,m,v1); // 2 - 1
    show(v1);
    fish_pop("el_3",f3,m,v1); // 2 - 1 - 3
    show(v1);

    fish_free(v1);

    struct fish * v2 = fish_empty();
    show(v2);
    fish_pop("el_1",f1,m,v2); // 1
    show(v2);
    fish_pop("el_2",f2,m,v2); // 1 - 2
    show(v2);
    fish_push("el_3",f3,m,v2); // 3 - 1 - 2
    show(v2);
    struct fish * v = fish_find("el_2",v2);
    if (v!=NULL) fish_remove(v,v2);
    show(v2);
    
    fish_free(v2);

    return 0;
}