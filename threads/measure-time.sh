#!/bin/sh

# Δημιουργία κεφαλίδας στο αρχείο CSV
echo "array_size,number_of_threads,mode,execution_time" > thread_times.csv

# Ορισμός μεγέθους πίνακα σε 10000000 στοιχεία
SIZE=10000000

# Λίστα με τους διαφορετικούς αριθμούς νημάτων που θα δοκιμαστούν
THREADS="1 2 4 8 16 32"

for t in $THREADS; do
    # Επιλογή mode ανάλογα με τον αριθμό των νημάτων:
    # Αν έχουμε μόνο 1 νήμα, τρέχουμε μόνο το Mode 1 (ασφαλές)
    # Για περισσότερα νήματα, τρέχουμε και το Mode 0 (χωρίς συγχρονισμό) και το Mode 1 (με συγχρονισμό) για να συγκρίνουμε επιδόσεις
    if [ "$t" = "1" ]; then
        MODES="1"
    else
        MODES="0 1"
    fi

    for m in $MODES; do
	# Καταγραφή ώρας έναρξης (σε milliseconds)
        start=$(date +%s%3N)
        
        # Εκτέλεση του προγράμματος threadsort (Αφαιρέθηκε το > /dev/null για να φαίνονται τα IDs των νημάτων)
        # Χρησιμοποιούμε το /dev/null ως όνομα αρχείου εξόδου για να μην γεμίζει ο δίσκος
        ./threadsort $SIZE $t $m /dev/null
        
	# Καταγραφή ώρας λήξης
        end=$(date +%s%3N)

        # Υπολογισμός διάρκειας εκτέλεσης
        runtime=$((end - start))

        # Εγγραφή των αποτελεσμάτων στο αρχείο CSV
        echo "$SIZE,$t,$m,$runtime" >> thread_times.csv
    done
done
