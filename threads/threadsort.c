#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

/* * Δομή για τα δεδομένα κάθε νήματος.
 * Επειδή η pthread_create δέχεται μόνο μία παράμετρο, βάζουμε τα πάντα εδώ.
 */
typedef struct {
    int *array;     // Δείκτης στον κοινό πίνακα
    int start;      // Δείκτης έναρξης του υποπίνακα (start index)
    int end;        // Δείκτης λήξης του υποπίνακα (end index)
    int thread_id;  // Ο αύξων αριθμός του νήματος (0, 1, 2...)
    int sync_mode;  // 0 για unsafe, 1 για safe mode (με mutex)
} thread_data_t;

// Δήλωση του Mutex για τον συγχρονισμό των νημάτων στο Mode 1
pthread_mutex_t lock;

/* * Συνάρτηση Συγχώνευσης (Merge).
 * Ενώνει δύο ταξινομημένους υποπίνακες σε έναν μεγαλύτερο.
 */
void merge(int arr[], int l, int m, int r, int sync_mode) {
    int n = r - l + 1; // Συνολικό μέγεθος του τμήματος προς συγχώνευση
    int *temp = (int*)malloc(n * sizeof(int)); // Προσωρινός βοηθητικός πίνακας

    // Έλεγχος αν η malloc πέτυχε (Error Handling)
    if (temp == NULL) {
        perror("Failed to allocate temp array");
        exit(1);
    }

    // Αντιγραφή των δεδομένων από τον κοινό πίνακα στον προσωρινό temp
    for (int i = 0; i < n; i++)
        temp[i] = arr[l + i];

    int i = 0;           // Δείκτης για το αριστερό κομμάτι μέσα στον temp
    int j = m - l + 1;   // Δείκτης για το δεξί κομμάτι μέσα στον temp
    int k = l;           // Δείκτης για την εγγραφή πίσω στον αρχικό πίνακα arr

    //Tο σημείο που τα νήματα γράφουν πίσω στον κοινό πίνακα. Αν sync_mode == 1, κλειδώνουμε.//
    if (sync_mode == 1) pthread_mutex_lock(&lock);

    // Σύγκριση στοιχείων και τοποθέτηση στον πίνακα με τη σωστή σειρά
    while (i <= m - l && j < n) {
        if (temp[i] <= temp[j]) arr[k++] = temp[i++];
        else arr[k++] = temp[j++];
    }

    // Αν περίσσεψαν στοιχεία στο αριστερό κομμάτι, τα προσθέτουμε
    while (i <= m - l) arr[k++] = temp[i++];

    // Αν περίσσεψαν στοιχεία στο δεξί κομμάτι, τα προσθέτουμε
    while (j < n) arr[k++] = temp[j++];

    // Ξεκλείδωμα του mutex αφού τελείωσε η εγγραφή στον κοινό πίνακα
    if (sync_mode == 1) pthread_mutex_unlock(&lock);

    free(temp); // Αποδέσμευση της μνήμης του προσωρινού πίνακα
}

/* * Η αναδρομική συνάρτηση Merge Sort
 */
void merge_sort(int arr[], int l, int r, int sync_mode) {
    if (l < r) {
        int m = l + (r - l) / 2; // Εύρεση του μέσου (αποφυγή overflow)
        merge_sort(arr, l, m, sync_mode);      // Ταξινόμηση αριστερού μέρους
        merge_sort(arr, m + 1, r, sync_mode);  // Ταξινόμηση δεξιού μέρους
        merge(arr, l, m, r, sync_mode);        // Συγχώνευση των δύο μερών
    }
}

/* * Η συνάρτηση που εκτελείται από κάθε νήμα
 */
void* thread_work(void* arg) {
    thread_data_t* data = (thread_data_t*) arg;

    // Εκτύπωση μηνύματος έναρξης
    printf("Thread with id \"t%d\" sorts subarray [%d, %d]\n", data->thread_id, data->start, data->end);

    // Κάθε νήμα καλεί την merge_sort μόνο για το δικό του κομμάτι
    merge_sort(data->array, data->start, data->end, data->sync_mode);

    return NULL;
}

int main(int argc, char *argv[]) {
    // Έλεγχος αν δόθηκαν τα σωστά 4 ορίσματα
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <size> <threads> <sync_mode> <out_file>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);           // Μέγεθος πίνακα
    int num_threads = atoi(argv[2]); // Αριθμός νημάτων
    int sync_mode = atoi(argv[3]);   // Τρόπος συγχρονισμού (0 ή 1)
    char *out_file = argv[4];        // Όνομα αρχείου εξόδου

    // Έλεγχος αν τα νήματα είναι περισσότερα από το μέγεθος του πίνακα
    if (num_threads > N || num_threads < 1) {
        fprintf(stderr, "Error: Invalid number of threads.\n");
        return 1;
    }

    // Δημιουργία και γέμισμα του πίνακα με τυχαίους αριθμούς
    int *array = malloc(N * sizeof(int));
    // Έλεγχος αν η malloc πέτυχε για τον κύριο πίνακα (Error Handling)
    if (array == NULL) {
        perror("Failed to allocate main array");
        return 1;
    }
    
    srand(time(NULL));
    for (int i = 0; i < N; i++) array[i] = rand() % 10000;

    pthread_t threads[num_threads];
    thread_data_t t_args[num_threads];
    pthread_mutex_init(&lock, NULL); // Αρχικοποίηση του mutex

    // Υπολογισμός του μεγέθους κάθε τμήματος για τα νήματα
    int chunk = N / num_threads;
    for (int i = 0; i < num_threads; i++) {
        t_args[i].array = array;
        t_args[i].start = i * chunk;
        // Το τελευταίο νήμα παίρνει όλα τα υπόλοιπα στοιχεία αν η διαίρεση δεν είναι τέλεια
        t_args[i].end = (i == num_threads - 1) ? (N - 1) : ((i + 1) * chunk - 1);
        t_args[i].thread_id = i;
        t_args[i].sync_mode = sync_mode;

        // Δημιουργία του νήματος με έλεγχο επιτυχίας (Error Handling)
        if (pthread_create(&threads[i], NULL, thread_work, &t_args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // Αναμονή για να ολοκληρώσουν όλα τα νήματα την ταξινόμηση των τμημάτων τους
    for (int i = 0; i < num_threads; i++) pthread_join(threads[i], NULL);

    // Τελική συγχώνευση όλων των τμημάτων από το κύριο νήμα (main)
    if (num_threads > 1) {
        merge_sort(array, 0, N - 1, sync_mode);
    }

    // Αποθήκευση του τελικού ταξινομημένου πίνακα στο αρχείο
    FILE *f = fopen(out_file, "w");
    if (f) {
        for (int i = 0; i < N; i++) fprintf(f, "%d\n", array[i]);
        fclose(f);
    }

    // Καθαρισμός πόρων
    pthread_mutex_destroy(&lock);
    free(array);
    return 0;
}
