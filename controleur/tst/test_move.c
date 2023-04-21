#include "../lib/move.h"

void show(struct move * m_list) 
{
    char* result = (char*) calloc(BUFFER_SIZE,sizeof(char)); 
    if (move_is_empty(m_list)) {
        strcat(result, "empty list");
    }
    else {
        struct move * ptr = m_list->next;
        while(!is_move_end(ptr)) {
            char* move_str = move_show(*ptr); // get the string representation of the move
            strcat(result, move_str); // concatenate the move string to the result string
            free(move_str); // free the memory used by the move string
            ptr = ptr->next;
        }
    }
    printf("%s\n",result);
    free(result);
}

int main()
{
    struct move * v1 = move_empty();
    show(v1); 
    move_push("el_1",v1); // 1
    show(v1);
    move_push("el_2",v1); // 2 - 1
    show(v1);
    move_pop("el_3",v1); // 2 - 1 - 3
    show(v1);

    move_free(v1);

    struct move * v2 = move_empty();
    show(v2);
    move_pop("el_1",v2); // 1
    show(v2);
    move_pop("el_2",v2); // 1 - 2
    show(v2);
    move_push("el_3",v2); // 3 - 1 - 2
    show(v2);
    struct move * v = move_find("el_2",v2);
    if (v!=NULL) move_remove(v,v2);
    show(v2);
    
    move_free(v2);

    return 0;
}