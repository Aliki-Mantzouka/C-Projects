#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "command.h"

#define MAX_ARGS 64

// Βοηθητική συνάρτηση για την αφαίρεση εισαγωγικών
void remove_quotes(char* str) {
    if (!str) return;
    int len = strlen(str);
    if (len >= 2) {
        if ((str[0] == '"' && str[len-1] == '"') || (str[0] == '\'' && str[len-1] == '\'')) {
            memmove(str, str + 1, len - 2);
            str[len - 2] = '\0';
        }
    }
}

void execute_command(char *cmdline) {
    char *args[MAX_ARGS];
    int i = 0;
    int input_redirect = 0;
    int output_redirect = 0;
    char *input_file = NULL;
    char *output_file = NULL;

    // "Σπάμε" την είσοδο με βάση τα κενά, tabs και αλλαγές γραμμής
    char *token = strtok(cmdline, " \t\r\n");

    while (token != NULL && i < MAX_ARGS - 1) {
        if (strcmp(token, "<") == 0) {
            input_redirect = 1;
            input_file = strtok(NULL, " \t\r\n"); 
        } else if (strcmp(token, ">") == 0) {
            output_redirect = 1;
            output_file = strtok(NULL, " \t\r\n");
        } else {
            // Αποθηκεύουμε το token και του αφαιρούμε τα εισαγωγικά (αν έχει)
            args[i] = token;
            remove_quotes(args[i]);
            i++;
        }
        token = strtok(NULL, " \t\r\n");
    }
    args[i] = NULL;

    if (args[0] == NULL) return; // Αν δε δόθηκε εντολή

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return;
    }

    if (pid == 0) {  // Παιδική διεργασία
        if (input_redirect && input_file) {
            int input_fd = open(input_file, O_RDONLY);
            if (input_fd == -1) {
                perror("Input file open failed");
                exit(1);
            }
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        if (output_redirect && output_file) {
            int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_fd == -1) {
                perror("Output file open failed");
                exit(1);
            }
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        // Εκτέλεση της εντολής
        execvp(args[0], args);
        perror("execvp failed");
        exit(1);
    } else {  // Γονική διεργασία
        wait(NULL);
    }
}
