#include "../lib/view.h"

void show(struct view * v_list) 
{
    char* result = (char*) calloc(BUFFER_SIZE,sizeof(char)); 
    if (view_is_empty(v_list)) {
        strcat(result, "empty list");
    }
    else {
        struct view * ptr = v_list->next;
        while(!is_view_end(ptr)) {
            char* view_str = view_show(*ptr); // get the string representation of the view
            strcat(result, view_str); // concatenate the view string to the result string
            free(view_str); // free the memory used by the view string
            ptr = ptr->next;
        }
    }
    printf("%s\n",result);
    free(result);
}

int main()
{
    struct frame f1 = {1,1,1,1};
    struct frame f2 = {2,2,2,2};
    struct frame f3 = {3,3,3,3};
    struct frame f4 = {4,4,4,4};

    struct view * v1 = view_empty();
    show(v1); 
    view_push("el_1",f1,v1); // 1
    show(v1);
    view_push("el_2",f2,v1); // 2 - 1
    show(v1);
    view_pop("el_3",f3,v1); // 2 - 1 - 3
    show(v1);

    view_free(v1);

    struct view * v2 = view_empty();
    show(v2);
    view_pop("el_1",f1,v2); // 1
    show(v2);
    view_pop("el_2",f2,v2); // 1 - 2
    show(v2);
    view_push("el_3",f3,v2); // 3 - 1 - 2
    show(v2);
    struct view * v = view_find("el_2",v2);
    if (v!=NULL) view_remove(v,v2);
    show(v2);
    
    view_free(v2);

    return 0;
}