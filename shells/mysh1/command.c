#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "command.h"  // Περιλαμβάνει τις δηλώσεις των συναρτήσεων

// Εκτέλεση εντολής από τον χρήστη
void execute_command(char *cmd) {
    pid_t pid = fork();  // Δημιουργία νέας διεργασίας

    if (pid == -1) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {  // Παιδική διεργασία
        char *args[] = {cmd, NULL};  // Δημιουργία πίνακα για την εντολή
        if (execvp(cmd, args) == -1) {
            perror("execvp failed");
            exit(1);
        }
    } else {  // Γονική διεργασία
        wait(NULL);  // Αναμονή για την ολοκλήρωση της παιδικής διεργασίας
    }
}

