#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"  // Για τη συνάρτηση execute_command
#include "utils.h"    // Για τη συνάρτηση remove_newline

#define MAX_CMD_LEN 1024

int main() {
	// Προσθήκη του /bin στο PATH του προγράμματος
    setenv("PATH", "/bin:/usr/bin:/usr/local/bin", 1); // Ρυθμίζει το PATH για το πρόγραμμα

    char cmdline[MAX_CMD_LEN];  // Χώρος για την εντολή που θα πληκτρολογήσει ο χρήστης

    while (1) {
        printf("mysh3> ");
        fflush(stdout);  // Βεβαιώνεται ότι η προτροπή εμφανίζεται άμεσα

        if (fgets(cmdline, MAX_CMD_LEN, stdin) == NULL) {
            printf("\n");
            break;
        }

        remove_newline(cmdline);  // Αφαιρεί τον χαρακτήρα νέας γραμμής από την είσοδο

        if (strlen(cmdline) == 0) {
            continue;  // Αν η εντολή είναι κενή, πηγαίνουμε στην επόμενη επανάληψη
        }

        if (strcmp(cmdline, "exit") == 0) {
            break;  // Αν η εντολή είναι "exit", τερματίζει το πρόγραμμα
        }

        execute_command(cmdline);  // Εκτέλεση της εντολής
    }

    return 0;
}

