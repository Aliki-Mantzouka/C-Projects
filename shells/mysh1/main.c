#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"  // Περιλαμβάνει τις συναρτήσεις για την εκτέλεση εντολών
#include "utils.h"    // Περιλαμβάνει τις βοηθητικές συναρτήσεις

#define MAX_CMD_LEN 1024

int main() {
    // Προσθήκη των βασικών διαδρομών στο PATH του προγράμματος
    setenv("PATH", "/bin:/usr/bin:/usr/local/bin", 1);  // Ρυθμίζει το PATH για το πρόγραμμα

    char cmd[MAX_CMD_LEN];

    while (1) {
        printf("mysh1> ");  // Προτροπή του κελύφους
        fflush(stdout);     // Βεβαιώνεται ότι η προτροπή εμφανίζεται άμεσα

        // Έλεγχος για End Of File (EOF)
        if (fgets(cmd, MAX_CMD_LEN, stdin) == NULL) {
            printf("\n");
            break;
        }

        remove_newline(cmd);  // Αφαίρεση του χαρακτήρα αλλαγής γραμμής

        // Αν ο χρήστης πάτησε μόνο Enter, αγνόησε την είσοδο
        if (strlen(cmd) == 0) {
            continue;
        }

        // Αν η εντολή είναι "exit", βγαίνουμε από το κέλυφος
        if (strcmp(cmd, "exit") == 0) {
            break;
        }

        // Εκτέλεση της εντολής (με ή χωρίς ορίσματα)
        execute_command(cmd);
    }

    return 0;
}
