#include "lib/client.h"
#include "lib/syntax.h"

// port
int portno;
// sockets
int sockfd;
int newsockfd;
// reading aquarium
struct aquarium my_aquarium;
// client 
struct client * my_client;
// server read/write buffers
char rbuffer[BUFFER_SIZE];
char wbuffer[BUFFER_SIZE];

int connection()
{

    // set server 
    struct sockaddr_in serv_addr; // server adress
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // set client
    struct sockaddr_in cli_addr; 
    socklen_t clilen = sizeof(cli_addr);

    // bind
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Binding failed");

    // listen
    listen(sockfd, 5);

    // accept
    int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) error("Error on accept");

    return newsockfd;
}

void sign_in(int socket, struct client * c_list)
{
    
    while (1) {
        // write 
        bzero(wbuffer, BUFFER_SIZE);
        strcpy(wbuffer, "Enter your name : ");
        printf("sent : %s\n",wbuffer);
        write(newsockfd, wbuffer, strlen(wbuffer));
        // read 
        bzero(rbuffer, BUFFER_SIZE);
        if (read(newsockfd, rbuffer, BUFFER_SIZE) < 0) error("Error on reading");
        printf("recieved : %s \n", rbuffer);
        // check
        struct client * c = client_find(rbuffer, c_list);
        if (c != NULL) {
            bzero(wbuffer, BUFFER_SIZE);
            strcpy(wbuffer, "Name already exists! ");
            printf("sent : %s\n\n",wbuffer);
            write(newsockfd, wbuffer, strlen(wbuffer));
        }
        else {
           printf("Client registred ! \n");
           break;
        }
    }
}

void admin(struct client * c, struct aquarium * a)
{
    bool is_empty = true;
    // start terminal
    bzero(wbuffer, BUFFER_SIZE);
    strcpy(wbuffer, "Welcome to the aquarium server!");
    write_buffer(newsockfd, wbuffer); 
    bzero(wbuffer, BUFFER_SIZE);
    while(1) 
    {
        // read from prompt
        bzero(rbuffer, BUFFER_SIZE);
        if (read(newsockfd, rbuffer, BUFFER_SIZE) < 0) error("Error on reading");
        printf("recieved : %s", rbuffer);

        // parse and get command
        char** cmds = split_string(rbuffer," ");
        size_t cmds_len = get_array_length(cmds);
        ////printf("parsed : %s...%s...\n",cmds[0],cmds[1]);

        // verify syntax
        if (!control_server_syntax(cmds)) 
        {
            sprintf(wbuffer, "Command not found");
            write_buffer(newsockfd, wbuffer); 
        } 

        // shutdown server
        else if (!strcmp(cmds[0],"bye")) 
        {
            sprintf(wbuffer,"Good Bye !");
            write_buffer(newsockfd, wbuffer); 
            free(cmds);
            break;
        }

        else if (!strcmp(cmds[0],"load")) 
        { 
            is_empty = true;
            if (!aquarium_find(cmds[1], a))  {
                sprintf(wbuffer,"Aquarium not found");
                write_buffer(newsockfd, wbuffer);
            }
            else {
                is_empty = false;
                sprintf(wbuffer,"Aquarium loaded (%ld display view)",view_size(a->views));
                write_buffer(newsockfd, wbuffer);
            }
        }
        
        // commands that need a loaded aquarium
        else if (is_empty == true) 
        {
            sprintf(wbuffer, "Nothing loaded to modify");
            write_buffer(newsockfd, wbuffer);
        }

        else if (!strcmp(cmds[0],"save")) 
        { 
            strcpy(a->name,cmds[1]);
            aquarium_save(a);
            sprintf(wbuffer,"Aquarium saved (%ld display view)",view_size(a->views));
            write_buffer(newsockfd, wbuffer);
            // nothing is loaded then
            is_empty = true;
        }

        else if (!strcmp(cmds[0],"show")) // view
        {
            char* result = views_show(a->views);
            strcpy(wbuffer, result);
            write_buffer(newsockfd, wbuffer);
            free(result);
        }

        else if (!strcmp(cmds[0],"add") && !strcmp(cmds[1],"view")) 
        {
            struct frame f = get_frame(cmds[3],"+x");
            if (!aquarium_fit_frame(a, f)) {
                sprintf(wbuffer,"Cannot add this view (out of aquarium)");
                write_buffer(newsockfd, wbuffer);
            }
            else {
                view_pop(cmds[2], f, a->views);
                sprintf(wbuffer,"View %s added",cmds[2]);
                write_buffer(newsockfd, wbuffer);
            }
        }

        else if (!strcmp(cmds[0],"del") && !strcmp(cmds[1],"view"))
        {
            struct view * v = view_find(cmds[2], a->views);
            if (v == NULL) {
                sprintf(wbuffer,"View not found");
                write_buffer(newsockfd, wbuffer);
            }
            else {
                view_remove(v, a->views);
                sprintf(wbuffer,"View %s deleted",cmds[2]);
                write_buffer(newsockfd, wbuffer);
            }
        }

        else 
        {
            sprintf(wbuffer,"Permission denied \n");
            write_buffer(newsockfd, wbuffer);
        }

        // set wbuffer to zero and free
        bzero(wbuffer, BUFFER_SIZE);
        free(cmds);
    }
}

void user(struct client * c, struct aquarium * a)
{
    bool is_empty = true;
    // start terminal
    bzero(wbuffer, BUFFER_SIZE);
    strcpy(wbuffer, "Welcome to the aquarium server!");
    write_buffer(newsockfd, wbuffer); 
    bzero(wbuffer, BUFFER_SIZE);
    while(1) 
    {
        // read from prompt
        bzero(rbuffer, BUFFER_SIZE);
        if (read(newsockfd, rbuffer, BUFFER_SIZE) < 0) error("Error on reading");
        printf("recieved : %s", rbuffer);

        // parse and get command
        char** cmds = split_string(rbuffer," ");
        size_t cmds_len = get_array_length(cmds);
        ////printf("parsed : %s...%s...\n",cmds[0],cmds[1]);

        // verify syntax
        if (!control_server_syntax(cmds)) 
        {
            sprintf(wbuffer, "Command not found");
            write_buffer(newsockfd, wbuffer); 
        } 

        // shutdown server
        else if (!strcmp(cmds[0],"bye")) 
        {
            sprintf(wbuffer,"Good Bye !");
            write_buffer(newsockfd, wbuffer); 
            free(cmds);
            break;
        }

        else if (!strcmp(cmds[0],"load")) 
        { 
            is_empty = true;
            if (!aquarium_find(cmds[1], a))  {
                sprintf(wbuffer,"Aquarium not found");
                write_buffer(newsockfd, wbuffer);
            }
            else {
                is_empty = false;
                sprintf(wbuffer,"Aquarium loaded (%ld display view)",view_size(a->views));
                write_buffer(newsockfd, wbuffer);
            }
        }
        
        // commands that need a loaded aquarium
        else if (is_empty == true) 
        {
            sprintf(wbuffer, "Nothing loaded to modify");
            write_buffer(newsockfd, wbuffer);
        }

        else if (!strcmp(cmds[0],"hello")) 
        {
            bool d = false;
            if (cmds_len == 2) d = true;
            else {
                struct view * v = view_find(cmds[3], a->views);
                if (v == NULL || view_is_taken(v)) d = true;
                else {
                    // no need to add client field to aquariums
                    client_view(c, v);
                    sprintf(wbuffer,"Greeting %s", v->name);
                    write_buffer(newsockfd, wbuffer);
                }
            }
            if (d == true) {
                struct view * v = view_find_available(a->views);
                if (v == NULL) {
                    sprintf(wbuffer,"No greeting");
                    write_buffer(newsockfd, wbuffer);
                }
                else {
                    client_view(c, v);
                    sprintf(wbuffer,"Greeting %s", v->name);
                    write_buffer(newsockfd, wbuffer);
                }
            }
        }

        else if (!strcmp(cmds[0],"status")) // fish
        {
            char* result = fishes_show(a->fishes);
            strcpy(wbuffer, result);
            write_buffer(newsockfd, wbuffer);
            free(result);
        }

        else if (!strcmp(cmds[0],"add") && !strcmp(cmds[1],"fish")) 
        {
            struct shape s = get_shape(cmds[3],"+x");
            struct move * m = move_find(cmds[4], a->moves);
            if (!aquarium_fit_shape(a, s)) {
                sprintf(wbuffer,"Cannot add this fish (out of aquarium)");
                write_buffer(newsockfd, wbuffer);
            }
            else if (m == NULL) {
                sprintf(wbuffer,"Move does not exist");
                write_buffer(newsockfd, wbuffer);
            }
            else {
                fish_pop(cmds[2], s, *m, a->fishes);
                sprintf(wbuffer,"Fish %s added",cmds[2]);
                write_buffer(newsockfd, wbuffer);
            }
        }

        else if (!strcmp(cmds[0],"del") && !strcmp(cmds[1],"fish"))
        {
            struct fish * f = fish_find(cmds[2], a->fishes);
            if (f == NULL) {
                sprintf(wbuffer,"Fish not found");
                write_buffer(newsockfd, wbuffer);
            }
            else {
                fish_remove(f, a->fishes);
                sprintf(wbuffer,"Fish %s deleted",cmds[2]);
                write_buffer(newsockfd, wbuffer);
            }
        }

        else if (!strcmp(cmds[0],"start")) 
        {
            struct fish * f = fish_find(cmds[1], a->fishes);
            if (f == NULL) {
                sprintf(wbuffer,"Fish not found");
                write_buffer(newsockfd, wbuffer);
            }
            else {
                fish_start(f);
                sprintf(wbuffer,"Fish %s started",cmds[1]);
                write_buffer(newsockfd, wbuffer);
            }
        }

        else 
        {
            sprintf(wbuffer,"Permission denied \n");
            write_buffer(newsockfd, wbuffer);
        }


        // set wbuffer to zero and free
        bzero(wbuffer, BUFFER_SIZE);
        free(cmds);
    }
}

int main(int argc, char* argv[])
{
    if (argc<2) {
        fprintf(stderr,"ERROR : no port provided \n");
        exit(1);
    }

    // set port
    portno = atoi(argv[1]);

    // initalize data/export file 
    fopen("aquarium.txt", "rw");
    fopen("aquarium.dat", "rw");

    // initialize socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR : opening socket");
    // initalize aquarium
    struct aquarium * initial_aquarium = aquarium_init();
    aquarium_save(initial_aquarium);
    // initialize client list
    struct client * client_list = client_empty();

    // waiting a client connection
    printf("Waiting a client... \n");
    newsockfd = connection();
    printf("Connected to a client ! \n");

    // register client
    sign_in(client_list);
    // add client
    client_push(rbuffer, client_list);
    printf("Client added ! \n\n");

    // get client
    my_client = client_find(rbuffer, client_list);
    assert (my_client != NULL);

    // communicate
    user(my_client, &my_aquarium);

    // free
    aquarium_free(initial_aquarium);
    client_free(client_list);
    // close
    close(newsockfd);
    close(sockfd);
    // remove data/export file 
    remove("aquarium.txt");
    remove("aquarium.dat");
    // die in peace
    return 0;
}
