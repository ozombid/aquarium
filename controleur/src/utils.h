#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // close
#include <assert.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h> // close
#include <netdb.h>
#include <sys/time.h> // FD_SET, FD_ISSET, FD_ZERO macros
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>

#define MAX_COMMANDS 10
#define MAX_NAMES 100
#define BUFFER_SIZE 1024
#define NONE "none"
#define NEG -1


void error(const char *msg);

void bzero(void *s, size_t n);

size_t get_array_length(char** arr);

/*
The split_string function takes a string str and a set of delimiters 
and splits the string into tokens based on the delimiters provided
*/
char** split_string(char* str, const char* delimiters);


int to_percent(int x, int coord, int dim);

int from_percent(int x, int coord, int dim);
