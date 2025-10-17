// Library Book Loan System Using pthreads

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_BOOKS 3
#define NUM_STUDENTS 10

// Three separate locks, one for each book - allows concurrent access to different books
pthread_mutex_t lock[NUM_BOOKS] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};

// Three condition variables, one for each book - for waiting when book is unavailable
pthread_cond_t cond[NUM_BOOKS] = {PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER};

// Book availability status (1 = available, 0 = on loan)
int book_available[NUM_BOOKS] = {1, 1, 1};


void loan_book(int student_id) {
    int book_id = student_id % NUM_BOOKS;
    
    printf("Student %d is trying to loan Book %c\n", student_id, 'A' + book_id);
    
    // Lock the mutex for this book
    pthread_mutex_lock(&lock[book_id]);
    
    // Wait while the book is not available
    while (!book_available[book_id]) {
        printf("Student %d waiting for Book %c\n", student_id, 'A' + book_id);
        pthread_cond_wait(&cond[book_id], &lock[book_id]);
    }
    
    // Book on loan
    book_available[book_id] = 0;
    printf("Student %d has loaned Book %c\n", student_id, 'A' + book_id);
    
    // Unlock before sleeping
    pthread_mutex_unlock(&lock[book_id]);
    
    // Simulate reading the book for 5 seconds
    sleep(5);
}

void return_book(int book_id) {
    // Lock the mutex for this book
    pthread_mutex_lock(&lock[book_id]);
    
    // Mark the book as available
    book_available[book_id] = 1;
    printf("Book %c has been returned\n", 'A' + book_id);
    
    // Notify one waiting student
    pthread_cond_signal(&cond[book_id]);
    pthread_mutex_unlock(&lock[book_id]);
}

void* student_thread(void* arg) {
    
    // Extract student ID from thread argument
    int student_id = *(int*)arg;
    free(arg);  // Free the memory allocated for student_id
    
    // Try to loan the book
    loan_book(student_id);
    
    // Return the book
    return_book(student_id % NUM_BOOKS);
    
    return NULL;
}

int main(void) {

    pthread_t students[NUM_STUDENTS];
    
    // Create student threads
    for (int i = 0; i < NUM_STUDENTS; i++) {
        int* id = malloc(sizeof(int));
        if (id == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        *id = i;
        if (pthread_create(&students[i], NULL, student_thread, id) != 0) {
            perror("pthread_create");
            free(id);
            exit(EXIT_FAILURE);
        }
    }
    
    // Wait for all students to finish
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(students[i], NULL);
    }
    
    return 0;
}

