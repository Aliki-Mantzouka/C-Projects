#ifndef COMMAND_H
#define COMMAND_H

// Εκτελεί μια εντολή κελύφους (π.χ. "ls -l")
// Αναλαμβάνει να κάνει parsing (strtok) και fork/execvp
void execute_command(char *cmdline);

#endif

