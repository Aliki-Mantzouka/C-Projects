#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "command.h"
#include "utils.h"

// Βοηθητική συνάρτηση για την αφαίρεση εισαγωγικών από τα ορίσματα
void remove_quotes(char* str) {
    if (!str) return;
    int len = strlen(str);
    if (len >= 2) {
        // Αν το string ξεκινά και τελειώνει με " ή '
        if ((str[0] == '"' && str[len-1] == '"') || (str[0] == '\'' && str[len-1] == '\'')) {
            // Μετακίνηση των χαρακτήρων κατά μία θέση
            memmove(str, str + 1, len - 2);
            // Τερματισμός του string στη νέα θέση
            str[len - 2] = '\0';
        }
    }
}

void handle_redirection(char** args) {
    for (int i = 0; args[i] != NULL; i++) {
        // Έλεγχος για ανακατεύθυνση εξόδου >
        if (strcmp(args[i], ">") == 0) {
            int fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) { perror("open out"); exit(1); }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL; // Τερματίζουμε τα ορίσματα πριν το σύμβολο >
        }
        // Έλεγχος για ανακατεύθυνση εισόδου <
        else if (strcmp(args[i], "<") == 0) {
            int fd = open(args[i+1], O_RDONLY);
            if (fd < 0) { perror("open in"); exit(1); }
            dup2(fd, STDIN_FILENO);
            close(fd);
            args[i] = NULL;
        }
    }
}

void execute_pipeline(char* line) {
    char** commands = split_line(line, "|");
    int num_cmds = 0;
    while (commands[num_cmds] != NULL) num_cmds++;

    // Δημιουργούμε pipes ΜΟΝΟ αν num_cmds > 1
    int num_pipes = num_cmds - 1;
    int *pipefds = NULL;

    if (num_pipes > 0) {
        pipefds = malloc(2 * num_pipes * sizeof(int));
        if (!pipefds) { perror("malloc"); exit(1); }
        for (int i = 0; i < num_pipes; i++) {
            if (pipe(pipefds + i * 2) < 0) { 
                perror("pipe"); 
                free(pipefds);
                exit(1); 
            }
        }
    }

    for (int i = 0; i < num_cmds; i++) {
        pid_t pid = fork();
        if (pid == 0) { // Διεργασία-παιδί
            // Σύνδεση pipes μόνο αν υπάρχουν (δηλαδή αν num_cmds > 1)
            if (num_pipes > 0) {
                // Αν δεν είναι η πρώτη εντολή, σύνδεσε το STDIN με το προηγούμενο pipe
                if (i > 0) dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
                // Αν δεν είναι η τελευταία εντολή, σύνδεσε το STDOUT με το επόμενο pipe
                if (i < num_cmds - 1) dup2(pipefds[i * 2 + 1], STDOUT_FILENO);

                // Κλείσιμο όλων των pipe file descriptors στο παιδί
                for (int j = 0; j < 2 * num_pipes; j++) close(pipefds[j]);
            }

            char** args = split_line(commands[i], " \t\r\n");
            
            // Αφαίρεση εισαγωγικών από τα ορίσματα
            for (int k = 0; args[k] != NULL; k++) {
                remove_quotes(args[k]);
            }

            // Διαχείριση ανακατεύθυνσης (<, >)
            handle_redirection(args);

            execvp(args[0], args);
            perror("execvp error");
            exit(1);
        } else if (pid < 0) {
            perror("fork");
            exit(1);
        }
    }

    // Ο γονέας κλείνει όλα τα pipes για να στείλει EOF
    if (num_pipes > 0) {
        for (int i = 0; i < 2 * num_pipes; i++) {
            close(pipefds[i]);
        }
    }

    // Αναμονή για όλες τις εντολές του pipeline
    for (int i = 0; i < num_cmds; i++) {
        wait(NULL);
    }
    
    // Αποδέσμευση μνήμης
    if (pipefds) free(pipefds);
    free(commands);
}
