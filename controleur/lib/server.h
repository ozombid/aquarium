#include "client.h"
#include "syntax.h"

#define ON true
#define OFF false

// writing message for sprintf
static char message[BUFFER_SIZE];

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

void sign_in(int socket, struct server * s);

// handlers
void admin(struct client * c, struct server * server, char** cmds);

void user(struct client * c, struct server * server, char** cmds);