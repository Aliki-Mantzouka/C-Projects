#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "utils.h"

int main() {
    // Προσθήκη του /bin στο PATH του προγράμματος
    setenv("PATH", "/bin:/usr/bin:/usr/local/bin", 1); // Ρυθμίζει το PATH για το πρόγραμμα

    char line[1024];

    while (1) {
        printf("mysh4> "); // Προτροπή του κελύφους
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin)) {
            
            printf("\n");
            break;
        }

        // Αφαίρεση του newline χαρακτήρα (\n) στο τέλος της γραμμής
        line[strcspn(line, "\n")] = 0;

        // Καθαρισμός κενών διαστημάτων στην αρχή και στο τέλος
        char* trimmed = trim_whitespace(line);

        if (strlen(trimmed) == 0) {
            // Αν ο χρήστης πάτησε μόνο Enter, συνέχισε
            continue;
        }

        if (strcmp(trimmed, "exit") == 0) {
            // Αν η εντολή είναι "exit", βγαίνουμε από το κέλυφος
            break;
        }

        // Εκτέλεση της γραμμής εντολών
        execute_pipeline(trimmed);
    }

    return 0;
}
