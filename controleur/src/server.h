#include "client.h"
#include "syntax.h"

#define ON true
#define OFF false

struct server {
    // server status
    bool status;
    // server clients
    struct client * client_list;
    // server aquariums
    struct aquarium * aquarium_list;
};

struct server * server_init();

bool server_is_down(struct server * s);

void server_kill(struct server * s);

void server_free(struct server * s);

void *sign_in(void * args);

// handlers
void handler_id(struct client * c, struct server * server);

void handler_name(struct client * c, struct server * server);

void handler_admin(struct client * c, struct server * server);

void handler_user(struct client * c, struct server * server);