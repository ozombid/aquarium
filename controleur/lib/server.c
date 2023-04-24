#include "server.h"

struct server * server_init()
{
    // initalize export file 
    fopen("aquarium.txt", "rw");
    // hello server 
    struct server * s = calloc(1, sizeof(struct server));
    // power on the server
    s->status = ON;
    // initialize client list
    s->client_list = client_empty();
    // initialize aquarium list
    s->aquarium_list = aquarium_empty();
    struct aquarium * aquarium_1 = aquarium_init();
    aquarium_pop(aquarium_1, s->aquarium_list);
    aquarium_save(aquarium_1);

    return s;
}

bool server_is_down(struct server * s)
{
    return s->status == OFF;
}

void server_kill(struct server * s)
{
    s->status = OFF;
}

void server_free(struct server * s)
{
    aquarium_free(s->aquarium_list);
    client_free(s->client_list);
    free(s);
    remove("aquarium.txt");
}

void handler_name(struct client * c, struct server * s) // view = NULL
{
    if (client_find_by_name(c->rbuffer, s->client_list) != NULL)
        client_simple_write(c, "Name already exists \n> enter your name : ");
    else {
        client_set_name(c, c->rbuffer);
        client_simple_write(c, "admin or user ? : ");
        client_set_handler(c, handler_id);
    }
}

void handler_id(struct client * c, struct server * s) // view = NULL
{
    if (strcmp(c->rbuffer, ADMIN) && strcmp(c->rbuffer, USER)) 
        client_simple_write(c, "admin or user ? : ");
    else {
        client_set_id(c, c->rbuffer);
        void (*f)(struct client *, struct server * s) = !strcmp(c->id, ADMIN) ? handler_admin :
            !strcmp(c->id, USER) ? handler_user : NULL;
        assert (f != NULL);
        client_set_handler(c, f);
        printf("Client registred ! \n");
        client_write(c, "Welcome to the aquarium server!");
    }
}

void handler_admin(struct client * c, struct server * s) // view = NULL
{
    // writing message for sprintf
    char message[BUFFER_SIZE];
    // parse and get command
    char** cmds = split_string(c->rbuffer," ");
    // verify syntax
    if (!control_server_syntax(cmds)) client_write(c, "Command not found");

    else if (!strcmp(cmds[0],"down")) server_kill(s);  // admin can stop server anytime

    else if (!strcmp(cmds[0],"load")) // 
    { 
        c->aquarium = NULL;
        struct aquarium * a = aquarium_find(cmds[1], s->aquarium_list);
        if (a == NULL) client_write(c, "Aquarium not found");
        else {
            client_set_aquarium(c, a);
            sprintf(message,"Aquarium loaded (%ld display view)",view_size(c->aquarium->views));
            client_write(c, message);
        }
    }
    // commands that need a loaded aquarium
    else if (c->aquarium == NULL) client_write(c, "Load aquarium first");

    else if (!strcmp(cmds[0],"save")) 
    { 
        strcpy(c->aquarium->name,cmds[1]);
        aquarium_save(c->aquarium);
        sprintf(message,"Aquarium saved (%ld display view)",view_size(c->aquarium->views));
        client_write(c, message);
        // nothing is loaded then
        c->aquarium = NULL;
    }
    else if (!strcmp(cmds[0],"show")) // view
    {
        char* result = views_show(c->aquarium->views);
        client_write(c, result);
        free(result);
    }
    else if (!strcmp(cmds[0],"add") && !strcmp(cmds[1],"view")) 
    {
        struct frame f = frame_create(cmds[3],"+x");
        if (!aquarium_fit_frame(c->aquarium, f)) client_write(c,"Cannot add this view (out of aquarium)");
        else {
            view_pop(cmds[2], f, c->aquarium->views);
            sprintf(message,"View %s added",cmds[2]);
            client_write(c, message);
        }
    }
    else if (!strcmp(cmds[0],"del") && !strcmp(cmds[1],"view"))
    {
        struct view * v = view_find(cmds[2], c->aquarium->views);
        if (v == NULL) client_write(c,"View not found");
        else {
            if (view_is_taken(v)) {
                struct client * c = client_find_by_view(v, s->client_list);
                c->status = LEFT;
            }
            view_remove(v, c->aquarium->views);
            sprintf(message,"View %s deleted",cmds[2]);
            client_write(c, message);
        }
    }
    else client_write(c,"Permission denied");

    // free
    free(cmds);
}

void handler_user(struct client * c, struct server * s)
{
    // writing message for sprintf
    char message[BUFFER_SIZE];
    bzero(message, BUFFER_SIZE);
    // parse and get command
    char** cmds = split_string(c->rbuffer," ");
    size_t cmds_len = get_array_length(cmds);
    // verify syntax
    if (!control_server_syntax(cmds)) client_write(c, "Command not found");

    else if (!strcmp(cmds[0],"load")) 
    { 
        c->aquarium = NULL;
        struct aquarium * a = aquarium_find(cmds[1], s->aquarium_list);
        if (a == NULL) client_write(c, "Aquarium not found");
        else {
            client_clear_view(c);
            client_set_aquarium(c, a);
            sprintf(message,"Aquarium loaded (%ld display view)",view_size(c->aquarium->views));
            client_write(c, message);
        }
    }
    // commands that need a loaded aquarium
    else if (c->aquarium == NULL) client_write(c, "Load aquarium first");

    else if (!strcmp(cmds[0],"hello")) 
    {
        bool d = false;
        if (cmds_len == 2) d = true;
        else {
            struct view * v = view_find(cmds[3], c->aquarium->views);
            if (v == NULL || view_is_taken(v)) d = true;
            else { // no need to add client field to aquariums
                client_clear_view(c);
                client_set_view(c, v);
                sprintf(message,"Greeting %s", v->name);
                client_write(c, message);
            }
        }
        if (d == true) {
            struct view * v = view_find_available(c->aquarium->views);
            if (v == NULL) client_write(c, "No greeting");
            else {
                client_clear_view(c);
                client_set_view(c, v);
                sprintf(message,"Greeting %s", v->name);
                client_write(c, message);
            }
        }
    }
    else if (!strcmp(cmds[0],"status")) // fish
    {
        char* result = fishes_show(c->aquarium->fishes);
        client_write(c, result);
        free(result);
    }
    else if (!strcmp(cmds[0],"add") && !strcmp(cmds[1],"fish")) 
    {
        struct shape s = shape_create(cmds[3],"+x");
        struct move * m = move_find(cmds[4], c->aquarium->moves);
        if (!aquarium_fit_shape(c->aquarium, s)) client_write(c,"Cannot add this fish (out of aquarium)");
        else if (m == NULL) client_write(c,"Move does not exist");
        else {
            fish_pop(cmds[2], s, *m, c->aquarium->fishes);
            sprintf(message,"Fish %s added",cmds[2]);
            client_write(c, message);
        }
    }
    else if (!strcmp(cmds[0],"del") && !strcmp(cmds[1],"fish"))
    {
        struct fish * f = fish_find(cmds[2], c->aquarium->fishes);
        if (f == NULL) client_write(c, "Fish not found");
        else {
            fish_remove(f, c->aquarium->fishes);
            sprintf(message,"Fish %s deleted",cmds[2]);
            client_write(c, message); 
        }
    }
    else if (!strcmp(cmds[0],"start")) 
    {
        struct fish * f = fish_find(cmds[1], c->aquarium->fishes);
        if (f == NULL) client_write(c, "Fish not found");
        else {
            fish_start(f);
            sprintf(message,"Fish %s started",cmds[1]);
            client_write(c, message); 
        }
    }
    else client_write(c, "Permission denied"); 

    // free
    free(cmds);
}


