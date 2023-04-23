#include "aquarium.h"

#define CONNECTED true
#define LEFT false 
#define ADMIN "admin"
#define USER "user"

struct server;

struct client {
    // identities
    int socket;
    char* name;
    char* id;
    // buffers
    char* wbuffer;
    char* rbuffer;
    // status
    bool status;
    // server handler
    void (*handler)(struct client *,  struct server * s, char**);
    // pointers
    struct aquarium * aquarium;
    struct view * view;
    // next
    struct client * next;
};

// init
struct client * client_empty();

struct client * client_create(int socket);

// setters
void client_set_name(struct client * c, char* name);

void client_set_id(struct client * c, char* id);

void client_set_view(struct client * c, struct view * v);

void client_clear_view(struct client * c);

void client_set_aquarium(struct client * c, struct aquarium * a);

void client_set_handler(struct client * c, void (*f)(struct client *, struct server * s, char**));

// bools
bool is_client_end(struct client * c);

bool is_last_client(struct client * c);

bool client_is_empty(struct client * c_list);

// add / remove
void client_pop(struct client * c, struct client * c_list);

void client_push(struct client * c, struct client * c_list);

void client_remove(struct client * c, struct client * c_list);

// find
struct client * client_find_by_name(char* name, struct client * c_list);

struct client * client_find_by_view(struct view * v, struct client * c_list);

// size / free
size_t client_size(struct client * c_list);

void client_free(struct client * c_list);

// show 
char* client_show(struct client c);

char* clients_show(struct client * c_list);

// read / write
bool client_read(struct client * c);

void client_write(struct client * c, char* message);
