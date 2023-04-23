#include "lib/server.h"

int main(int argc, char *argv[])
{
    if (argc<2) {
        fprintf(stderr,"ERROR : no port provided \n");
        exit(1);
    }

    // socket variables
    int portno = atoi(argv[1]);
	int opt = true;
	int master_socket, addrlen;
	struct sockaddr_in address;

    struct server * server = server_init();

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
        if (server_is_down(server)) break;
		// clear the socket set
		FD_ZERO(&readfds);
		// add master socket to set
		FD_SET(master_socket, &readfds);
		int max_sd = master_socket;
		// add child sockets to set
        if (!client_is_empty(server->client_list)) 
        {
            printf("setting %ld client \n", client_size(server->client_list));
            struct client * ptr = server->client_list;
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
            sign_in(new_socket, server);
            printf("\n");
		}

        else 
        {   // else its some IO operation on some other socket
            printf("IO operation ! \n");
            assert (!client_is_empty(server->client_list));
            struct client * ptr = server->client_list;
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
                        client_remove(ptr, server->client_list);
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
                        else ptr->handler(ptr, server, cmds);
                        // free
                        free(cmds);
                    }                   
                }   
                ptr = tmp;           
            }    
        }
	}         
             
    // free        
    server_free(server);
	return 0;
}
