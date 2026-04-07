#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"  // execute_command
#include "utils.h"    // remove_newline

#define MAX_CMD_LEN 1024

int main() {

	// Προσθήκη του /bin στο PATH του προγράμματος
    setenv("PATH", "/bin:/usr/bin:/usr/local/bin", 1);  // Ρυθμίζει το PATH για το πρόγραμμα
							
    char cmd[MAX_CMD_LEN];

    while (1) {
        printf("mysh2> ");   // Προτροπή του κελύφους
        fflush(stdout);      // Σιγουρεύει ότι εμφανίζεται αμέσως

        if (fgets(cmd, MAX_CMD_LEN, stdin) == NULL) {
            // End Of File (EOF)
            printf("\n");
            break;
        }

        remove_newline(cmd); // Αφαιρούμε το '\n'

        // Αν ο χρήστης πάτησε μόνο Enter
        if (strlen(cmd) == 0) {
            continue;
        }

        // Αν η εντολή είναι "exit", βγαίνουμε από το κέλυφος
        if (strcmp(cmd, "exit") == 0) {
            break;
        }

        // Εκτελούμε την εντολή (με ή χωρίς ορίσματα)
        execute_command(cmd);
    }

    return 0;
}

