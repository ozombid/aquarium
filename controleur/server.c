#include "lib/client.h"
#include "lib/syntax.h"

// writing message for sprintf
char message[BUFFER_SIZE];
// server status
bool down;
// server clients
struct client * client_list;
// server aquariums
struct aquarium * aquarium_list;

void admin(struct client * c, char** cmds) // view = NULL
{
    if (!strcmp(cmds[0],"down")) down = true;  // admin can stop server anytime

    else if (!strcmp(cmds[0],"load")) // 
    { 
        c->aquarium = NULL;
        struct aquarium * a = aquarium_find(cmds[1], aquarium_list);
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
        struct frame f = get_frame(cmds[3],"+x");
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
                struct client * c = client_find_by_view(v, client_list);
                c->status = LEFT;
            }
            view_remove(v, c->aquarium->views);
            sprintf(message,"View %s deleted",cmds[2]);
            client_write(c, message);
        }
    }
    else client_write(c,"Permission denied");
}

void user(struct client * c, char** cmds)
{
    size_t cmds_len = get_array_length(cmds);

    if (!strcmp(cmds[0],"load")) 
    { 
        c->aquarium = NULL;
        struct aquarium * a = aquarium_find(cmds[1], aquarium_list);
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
        struct shape s = get_shape(cmds[3],"+x");
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
}

void sign_in(int socket)
{
    struct client * c;
    while (1) {
        // write
        bzero(message, BUFFER_SIZE);
        strcpy(message, "admin or user ? : ");
        printf("sent : %s \n", message);
        send(socket, message, strlen(message), 0);
        // read 
        bzero(message, BUFFER_SIZE);
        if (read(socket, message, BUFFER_SIZE) < 0) error("Error on reading");
        // this is for Java Client
        size_t len = strlen(message);
        if (len > 0 && message[len-1] == '\n') message[len-1] = '\0';
        //
        printf("recieved : %s \n", message);
        // check
        void (*f)(struct client *, char**) = !strcmp(message, ADMIN) ? admin :
            !strcmp(message, USER) ? user : NULL;
        if (f != NULL) {
            c = client_create(socket);
            client_set_id(c, message);
            client_set_handler(c, f);
            client_pop(c, client_list);
            printf("Added to list of sockets \n");
            break;
        }
    }
    while (1) {
        // write
        bzero(c->wbuffer, BUFFER_SIZE);
        strcpy(c->wbuffer, "Enter your name : ");
        printf("sent to %s : %s\n", c->name, c->wbuffer);
        send(c->socket, c->wbuffer, strlen(c->wbuffer), 0);
        // read 
        client_read(c);
        printf("recieved from %s : %s \n", c->name, c->rbuffer);
        // check
        if (client_find_by_name(c->rbuffer, client_list) != NULL) {
            bzero(c->wbuffer, BUFFER_SIZE);
            strcpy(c->wbuffer, "Name already exists");
            printf("sent to %s : %s\n", c->name, c->wbuffer);
            send(c->socket, c->wbuffer, strlen(c->wbuffer), 0);
        }
        else {
           client_set_name(c, c->rbuffer);
           printf("Client registred ! \n");
           break;
        }
    }

    client_write(c, "Welcome to the aquarium server!");
}

int main(int argc, char *argv[])
{
    if (argc<2) {
        fprintf(stderr,"ERROR : no port provided \n");
        exit(1);
    }

    // initalize export file 
    fopen("aquarium.txt", "rw");

    // socket variables
    int portno = atoi(argv[1]);
	int opt = true;
	int master_socket, addrlen;
	struct sockaddr_in address;
    down = false;
	
    // initialize aquarium list
    aquarium_list = aquarium_empty();
    struct aquarium * aquarium_1 = aquarium_init();
    aquarium_pop(aquarium_1, aquarium_list);
    aquarium_save(aquarium_1);
    // initialize client list
    client_list = client_empty();

	// set of socket descriptors
	fd_set readfds;
	// create a master socket
	if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) error("socket failed");
	// set master socket to allow multiple connections, this is just a good habit, it will work without this
	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) error("setsockopt");
	// type of socket created
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(portno);
	// bind the socket to localhost port 8888
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) error("bind failed");
	printf("Listener on port %d \n", portno);
	// try to specify maximum of 3 pending connections for the master socket
	if (listen(master_socket, 3) < 0) error("listen");
	// accept the incoming connection
	addrlen = sizeof(address);
		
	while(1)
	{
        if (down) break;
		// clear the socket set
		FD_ZERO(&readfds);
		// add master socket to set
		FD_SET(master_socket, &readfds);
		int max_sd = master_socket;
		// add child sockets to set
        if (!client_is_empty(client_list)) 
        {
            printf("setting %ld client \n", client_size(client_list));
            struct client * ptr = client_list;
            while (!is_client_end(ptr)) {
                int sd = ptr->socket;
                // if valid socket descriptor then add to read list
                if (sd > 0) FD_SET(sd, &readfds);
                // highest file descriptor number, need it for the select function
                if (sd > max_sd) max_sd = sd;
                ptr = ptr->next;
            }
        }

		// wait for an activity on one of the sockets, timeout is NULL, so wait indefinitely
        puts("Waiting for activities ... \n");
		int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
		if ((activity < 0) && (errno!=EINTR)) error("select error");
        printf("Got something : ");

		if (FD_ISSET(master_socket, &readfds)) 
        {   // If something happened on the master socket, then its an incoming connection
            printf("New connection : ");
            int new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
			if (new_socket < 0) error("accept");
			// inform user of socket number - used in send and receive commands
			printf("socket fd is %d, ip is : %s, port : %d \n",
                    new_socket, inet_ntoa(address.sin_addr), ntohs (address.sin_port));
			// add new socket client to client list
            sign_in(new_socket);
            printf("\n");
		}

        else 
        {   // else its some IO operation on some other socket
            printf("IO operation ! \n");
            assert (!client_is_empty(client_list));
            struct client * ptr = client_list;
            while (!is_client_end(ptr)) 
            {
                int sd = ptr->socket;
                struct client * tmp = ptr->next;
                if (FD_ISSET(sd, &readfds))
                {
                    // Check if it was for closing, and also read the incoming message
                    if (client_read(ptr) == false || ptr->status == LEFT) {
                        // Somebody disconnected, get his details and print
                        getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                        printf("Host disconnected, ip %s, port %d \n",
                            inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                        // Close the socket and mark as 0 in list for reuse
                        client_remove(ptr, client_list);
                        close(sd);
                    }
                    // Echo back the message that came in
                    else {
                        bzero(message, BUFFER_SIZE);
                        // parse and get command
                        char** cmds = split_string(ptr->rbuffer," ");
                        // verify syntax
                        if (!control_server_syntax(cmds)) client_write(ptr, "Command not found");
                        // apply
                        else ptr->handler(ptr, cmds);
                        // free
                        free(cmds);
                        printf("\n");
                    }                   
                }   
                ptr = tmp;           
            }    
        }
	}         
             
    // free        
    aquarium_free(aquarium_list);
    client_free(client_list);
    remove("aquarium.txt");
	return 0;
}
