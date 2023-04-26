#include "utils.h"

void error(const char *msg)        
{
    perror(msg);
    exit(1);
}

void bzero(void *s, size_t n) 
{
    memset(s, 0, n);
}

size_t get_array_length(char** arr) 
{
    int length = -1;
    while (arr[++length] != NULL);
    return length;
}

char** split_string(char* str, const char* delimiters) 
{
    char** tokens = malloc(sizeof(char*) * (MAX_COMMANDS + 1)); 
    //static char* tokens[MAX_COMMANDS + 2]; // + delete free(cmds)
    int count = 0;
    char* token = strtok(str, delimiters);
    while (token != NULL && strcmp(token,"\n") && count < MAX_COMMANDS) {
        tokens[count++] = token;
        token = strtok(NULL, delimiters);
    }
    tokens[count] = NULL;
    return tokens;
}

int from_percent(int x, int coord, int dim)
{
    return coord + (dim * x / 100);
}

int to_percent(int x, int coord, int dim)
{
    return 100*(x - coord) / dim;
}


