#include <stdio.h>
#include <string.h>
#include "utils.h"

// Συνάρτηση για την αφαίρεση του χαρακτήρα newline
void remove_newline(char *cmd) {
    cmd[strcspn(cmd, "\n")] = 0;  // Αφαίρεση του χαρακτήρα newline
}

