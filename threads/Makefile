# Ορισμός μεταβλητών για τον μεταγλωττιστή και των flags
CC = gcc
CFLAGS = -Wall -pthread
TARGET = threadsort

# Χτίσιμο του εκτελέσιμου threadsort (χωρίς να τρέχει το script)
all: $(TARGET)

# Κανόνας μεταγλώττισης του πηγαίου κώδικα
$(TARGET): threadsort.c
	$(CC) $(CFLAGS) threadsort.c -o $(TARGET)

# Δημιουργία του CSV μέσω του measure-time.sh (εξαρτάται από το threadsort)
thread_times.csv: $(TARGET)
	sh measure-time.sh

# Μεταγλώττιση με Undefined και Address Sanitizers για έλεγχο ακαθόριστης συμπεριφοράς και λαθών στη διαχείριση της μνήμης
# Προστέθηκε το -pthread στο τέλος για σωστό linking των νημάτων
ub: threadsort.c
	$(CC) $(CFLAGS) -fsanitize=undefined -fsanitize=address threadsort.c -o $(TARGET) -pthread

# Μεταγλώττιση με Thread Sanitizer για έλεγχο για προβλήματα ταυτοχρονισμού μεταξύ των νημάτων
# Προστέθηκε το -pthread στο τέλος για σωστό linking των νημάτων
thread: threadsort.c
	$(CC) $(CFLAGS) -fsanitize=thread threadsort.c -o $(TARGET) -pthread

# Καθαρισμός των παραγόμενων αρχείων
clean:
	rm -f $(TARGET) *.txt thread_times.csv
