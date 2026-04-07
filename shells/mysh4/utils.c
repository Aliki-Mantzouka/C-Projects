#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "utils.h"

char* trim_whitespace(char* str) {
    if (!str) return NULL;
    // Αφαίρεση αρχικών κενών
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    // Αφαίρεση τελικών κενών
    char* end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

char** split_line(char* line, const char* delimiters) {
    int bufsize = 64, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) exit(EXIT_FAILURE);

    token = strtok(line, delimiters);
    while (token != NULL) {
        // Καθαρίζουμε κάθε token από περιττά κενά
        tokens[position] = trim_whitespace(token);
        position++;
        token = strtok(NULL, delimiters);
    }
    tokens[position] = NULL;
    return tokens;
}
