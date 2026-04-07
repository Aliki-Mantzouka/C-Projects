#include <string.h>
#include "utils.h"

// Βρίσκει το '\n' στη συμβολοσειρά και το αντικαθιστά με '\0'
// Έτσι "καθαρίζουμε" την είσοδο από fgets
void remove_newline(char *cmd) {
    cmd[strcspn(cmd, "\n")] = '\0';
}

