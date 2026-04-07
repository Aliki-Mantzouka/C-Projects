#include <stdio.h>
#include <stdlib.h>
#include <string.h>     // για strtok
#include <unistd.h>     // για fork, execvp
#include <sys/wait.h>   // για wait
#include "command.h"

#define MAX_ARGS 64  // Μέγιστος αριθμός λέξεων (εντολή + ορίσματα)

// Βοηθητική συνάρτηση για την αφαίρεση εισαγωγικών από τα ορίσματα
void remove_quotes(char* str) {
    if (!str) return;
    int len = strlen(str);
    if (len >= 2) {
        // Αν το string ξεκινά και τελειώνει με " ή '
        if ((str[0] == '"' && str[len-1] == '"') || (str[0] == '\'' && str[len-1] == '\'')) {
            // Μετακίνηση των χαρακτήρων κατά μία θέση για να φύγει το πρώτο "
            memmove(str, str + 1, len - 2);
            // Τερματισμός του string στη νέα θέση για να φύγει το τελευταίο "
            str[len - 2] = '\0';
        }
    }
}

// Η συνάρτηση αυτή παίρνει ένα string, π.χ. "ls -l -a"
// και το σπάει σε κομμάτια: ["ls", "-l", "-a", NULL]
void execute_command(char *cmdline) {
    char *args[MAX_ARGS];
    int i = 0;

    // "Σπάμε" την είσοδο με βάση τα κενά, tabs και αλλαγές γραμμής
    char *token = strtok(cmdline, " \t\r\n");

    if (token == NULL) {
        // Αν ο χρήστης πάτησε μόνο Enter
        return;
    }

    // Γεμίζουμε τον πίνακα args με όλα τα tokens
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i] = token;
        
        // Αφαιρούμε τα εισαγωγικά από το κάθε token (π.χ. το "include" γίνεται include)
        remove_quotes(args[i]);
        
        i++;
        token = strtok(NULL, " \t\r\n");
    }
    args[i] = NULL;  // Tελειώνουμε με NULL για την execvp

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return;
    }

    if (pid == 0) {
        // Παιδική διεργασία: εκτέλεση της εντολής
        if (execvp(args[0], args) == -1) {
            perror("execvp error");
        }
        exit(EXIT_FAILURE);
    } else {
        // Γονική διεργασία: αναμονή για το παιδί
        wait(NULL);
    }
}
