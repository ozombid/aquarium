#include "client.h"

static const struct client mark_end = {NEG, NONE, NONE, NONE, NONE, LEFT, (pthread_t)0, NULL, NULL, NULL, NULL}; 
#define SENTINEL ((struct client *) &mark_end)

// init
struct client * client_empty() 
{
    struct client * c = malloc(sizeof(struct client));
    *c = (struct client){NEG, NONE, NONE, NONE, NONE, LEFT, (pthread_t)0, NULL, NULL, NULL, SENTINEL};
    return c;
}
struct client * client_create(int socket)
{
    struct client * c = calloc(1,sizeof(struct client));
    if (c == NULL) error("ERROR : NULL calloc");
    // client identities
    c->socket = socket;
    c->name = malloc(MAX_NAMES*sizeof(char));
    strcpy(c->name, "new client");
    c->id = malloc(MAX_NAMES*sizeof(char));
    // buffers
    c->wbuffer = malloc(BUFFER_SIZE*sizeof(char));
    c->rbuffer = malloc(BUFFER_SIZE*sizeof(char));
    // client status is connected at creation
    c->status = CONNECTED;
    // server command handler
    c->handler = NULL;
    // a client points to a view in an aquarium
    c->aquarium = NULL;
    c->view = NULL;
    // unknown next
    c->next = NULL;
    // create sign in thread
    return c;
}

// setters
void client_set_name(struct client * c, char* name)
{
    strcpy(c->name, name);
}
void client_set_id(struct client * c, char* id)
{
    strcpy(c->id, id);
}
void client_set_view(struct client * c, struct view * v)
{
    view_take(v);
    c->view = v;
}
void client_clear_view(struct client * c)
{
    if (c->view != NULL) {
        view_release(c->view);
        c->view = NULL;
    }
}
void client_set_aquarium(struct client * c, struct aquarium * a)
{
    c->aquarium = a;
}
void client_set_handler(struct client * c, void (*f)(struct client *,  struct server * s))
{
    c->handler = f;
}

// bools
bool is_client_end(struct client * c)
{
    return c == SENTINEL;
}
bool is_last_client(struct client * c)
{
    return is_client_end(c->next);
}
bool client_is_empty(struct client * c_list)
{
    return is_last_client(c_list);
}

// add / remove
void client_pop(struct client * c, struct client * c_list) 
{
    c->next = SENTINEL;
    struct client * ptr = c_list;
    while (!is_last_client(ptr)) ptr = ptr->next;
    ptr->next = c;
}
void client_push(struct client * c, struct client * c_list) 
{
    c->next = c_list->next;
    c_list->next = c;
}
void client_remove(struct client * c, struct client * c_list) 
{
    assert(!client_is_empty(c_list));
    struct client * ptr = c_list;
    while (ptr->next != c) ptr = ptr->next;
    struct client * tmp = ptr->next->next;
    ptr->next = tmp;
    if (c->view != NULL && c->status == CONNECTED)
        if (view_is_taken(c->view)) view_release(c->view);
    free(c->name);
    free(c->id);
    free(c->rbuffer);
    free(c->wbuffer);
    free(c);
}

// find
struct client * client_find_by_name(char* name, struct client * c_list) 
{
    if (client_is_empty(c_list)) return NULL;
    struct client * ptr = c_list;
    while (!is_client_end(ptr)) {
        if (!strcmp(ptr->name,name)) return ptr;
        ptr = ptr->next;
    }
    return NULL;
}
struct client * client_find_by_view(struct view * v, struct client * c_list) 
{
    if (client_is_empty(c_list)) return NULL;
    struct client * ptr = c_list;
    while (!is_client_end(ptr)) {
        if (ptr->view != NULL)
            if (ptr->view == v) return ptr;
        ptr = ptr->next;
    }
    return NULL;
}

// size / free
size_t client_size(struct client * c_list)
{
    size_t count = 0;
    struct client * ptr = c_list;
    while (!is_last_client(ptr)) {
        count++;
        ptr = ptr->next;
    }
    return count;
}
void client_free(struct client * c_list)
{
    struct client * ptr = c_list->next;
    while (!is_client_end(ptr)) {
        struct client * tmp = ptr->next;
        free(ptr->name);
        free(ptr->id);
        free(ptr->rbuffer);
        free(ptr->wbuffer);
        free(ptr);
        ptr = tmp;
    }
    free(c_list);
}

// show 
char* client_show(struct client c) 
{
    char* result = (char*) malloc(50*sizeof(char));
    char* status = c.status == CONNECTED ? "connected" : "disconnected";
    sprintf(result, "%s : %s", c.name, status); 
    return result;
}
char* clients_show(struct client * c_list) 
{
    char* result = (char*) calloc(BUFFER_SIZE,sizeof(char)); 
    if (client_is_empty(c_list)) {
        strcat(result, "empty list of clients");
    }
    else {
        sprintf(result, "%ld clients to server", client_size(c_list));
        struct client * ptr = c_list->next;
        while(!is_client_end(ptr)) {
            char* client_str = client_show(*ptr); 
            strcat(result, "\n");
            strcat(result, client_str); 
            free(client_str); 
            ptr = ptr->next;
        }
    }
    return result;
}

// read / write
bool client_read(struct client * c)
{
    bzero(c->rbuffer, BUFFER_SIZE);
    int message = read(c->socket, c->rbuffer, BUFFER_SIZE);
    if (message < 0) error("Error on reading");
    // this is for Java Client
    size_t len = strlen(c->rbuffer);
    if (len > 0 && c->rbuffer[len-1] == '\n') c->rbuffer[len-1] = '\0';
    //
    printf("recieved from %s : %s.\n", c->name, c->rbuffer);
    if (message == 0) return false;
    return true;
}

void client_write_OK(struct client * c, char* message)
{
    bzero(c->wbuffer, BUFFER_SIZE);
    printf("sent to %s : %s\n", c->name, message);
    sprintf(c->wbuffer, "OK : %s \n\033[32mserver@%s\033[0m:\033[33m~/control\033[0m$ ", message, c->id); 
    send(c->socket, c->wbuffer, strlen(c->wbuffer), 0); 
}

void client_write(struct client * c, char* message)
{
    bzero(c->wbuffer, BUFFER_SIZE);
    printf("sent to %s : %s\n", c->name, message);
    sprintf(c->wbuffer, "%s \n\033[32mserver@%s\033[0m:\033[33m~/control\033[0m$ ", message, c->id); 
    send(c->socket, c->wbuffer, strlen(c->wbuffer), 0); 
}

void client_simple_write(struct client * c, char* message)
{
    bzero(c->wbuffer, BUFFER_SIZE);
    printf("sent to %s : %s\n", c->name, message);
    sprintf(c->wbuffer, "%s", message); 
    send(c->socket, c->wbuffer, strlen(c->wbuffer), 0); 
}

void client_write_close(struct client * c)
{
    bzero(c->wbuffer, BUFFER_SIZE);
    printf("sent close to %s\n", c->name);
    sprintf(c->wbuffer, "\033[32mserver@%s\033[0m:\033[33m~/control\033[0m$ ", c->id); 
    send(c->socket, c->wbuffer, strlen(c->wbuffer), 0); 
}

void client_wait_read(struct client * c)
{
    usleep(100000);
}
