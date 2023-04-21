#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFER_SIZE 255

void error(const char *msg)        
{
    perror(msg);
    exit(1);
}

void bzero(void *s, size_t n) 
{
    memset(s, 0, n);
}

int main(int argc, char* argv[])
{
    if (argc<3) {
        fprintf(stderr,"ERROR : no port/hostname provided \n");
        exit(1);
    }

    /* 1 -- initialization */

    // set port
    int portno = atoi(argv[2]);

    // initialize socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR : opening socket");

    // set host
    struct hostent * host = gethostbyname(argv[1]);
    if (host == NULL) {
        fprintf(stderr,"ERROR : no such host \n");
        exit(1);
    }

    // set server
    struct sockaddr_in serv_addr;
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
    serv_addr.sin_port = htons(portno);

    // connect
    if ( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr )) < 0)
        error("Connection failed");

    /* 2 -- communication */

    char buffer[BUFFER_SIZE];
    int n;
    // read
    bzero(buffer, BUFFER_SIZE);
    n = read(sockfd, buffer, BUFFER_SIZE);
    if (n < 0) error("Error on reading");
    printf("%s", buffer);

    while(1) 
    {
        // write 
        bzero(buffer, BUFFER_SIZE);
        //fgets(buffer, 255, stdin);
        scanf(" %255[^\n]", buffer);  //! 255
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) error("Error on writing");

        // read
        bzero(buffer, BUFFER_SIZE);
        n = read(sockfd, buffer, BUFFER_SIZE);
        if (n < 0) error("Error on reading");
        printf("> %s", buffer);

        // break
        if (!strcmp("Good Bye ! \n", buffer)) break;
    }

    // close
    close(sockfd);
         
    return 0;
} 