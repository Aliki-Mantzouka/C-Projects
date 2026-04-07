CC = gcc
CFLAGS = -Wall -Wextra -g
SANITIZE_FLAGS = -fsanitize=address -fsanitize=undefined -g

# Χτίζει όλα τα κελύφη
all: mysh1 mysh2 mysh3 mysh4

# --------- Κανονικά builds ---------

mysh1: mysh1/mysh1

mysh1/mysh1: mysh1/main.c mysh1/command.c mysh1/utils.c
	$(CC) $(CFLAGS) -o mysh1/mysh1 mysh1/main.c mysh1/command.c mysh1/utils.c

mysh2: mysh2/mysh2

mysh2/mysh2: mysh2/main.c mysh2/command.c mysh2/utils.c
	$(CC) $(CFLAGS) -o mysh2/mysh2 mysh2/main.c mysh2/command.c mysh2/utils.c

mysh3: mysh3/mysh3

mysh3/mysh3: mysh3/main.c mysh3/command.c mysh3/utils.c
	$(CC) $(CFLAGS) -o mysh3/mysh3 mysh3/main.c mysh3/command.c mysh3/utils.c

mysh4: mysh4/mysh4

mysh4/mysh4: mysh4/main.c mysh4/command.c mysh4/utils.c
	$(CC) $(CFLAGS) -o mysh4/mysh4 mysh4/main.c mysh4/command.c mysh4/utils.c

# --------- Builds για testing με sanitizers ---------

test: mysh1_test mysh2_test mysh3_test mysh4_test

mysh1_test: mysh1/main.c mysh1/command.c mysh1/utils.c
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -o mysh1/mysh1_test mysh1/main.c mysh1/command.c mysh1/utils.c

mysh2_test: mysh2/main.c mysh2/command.c mysh2/utils.c
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -o mysh2/mysh2_test mysh2/main.c mysh2/command.c mysh2/utils.c

mysh3_test: mysh3/main.c mysh3/command.c mysh3/utils.c
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -o mysh3/mysh3_test mysh3/main.c mysh3/command.c mysh3/utils.c

mysh4_test: mysh4/main.c mysh4/command.c mysh4/utils.c
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -o mysh4/mysh4_test mysh4/main.c mysh4/command.c mysh4/utils.c

# --------- Καθάρισμα ---------

clean:
	rm -f mysh1/mysh1 mysh1/mysh1_test mysh2/mysh2 mysh2/mysh2_test mysh3/mysh3 mysh3/mysh3_test mysh4/mysh4 mysh4/mysh4_test

.PHONY: all mysh1 mysh2 mysh3 mysh4 test clean

