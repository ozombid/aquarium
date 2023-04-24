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
#define BUFFER_SIZE 255
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

/*
The split_n_string function is similar to split_string, but it splits a string 
into tokens and converts each token to an integer. 
The converted integers are then stored in an integer array ints. 
The function returns the number of integers stored in the ints array.
*/
struct frame get_frame(char* str, const char* delim);
