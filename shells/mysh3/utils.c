#include <string.h>
#include "utils.h"

void remove_newline(char *cmd) {
    cmd[strcspn(cmd, "\n")] = '\0';  // Αφαίρεση του χαρακτήρα newline
}

