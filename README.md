# Library-Book-Loan-System

CIS 450 - Class activity 4

Implementing a multi-threaded Library Book Loan System using POSIX threads (pthreads) in C

Library Overview:
- There are 3 books: A, B, and C (each has only 1 copy).
- There are 10 students, each represented by a separate thread.
- Each student thread will attempt to loan a book and hold it for 5 seconds.

Book Assignment Rule:
- Each student has a unique student_id from 0 to 9.
- Each student can only borrow the book assigned by: book_id = student_id % 3

Which maps to:
- 0 → Book A
- 1 → Book B
- 2 → Book C

Example:
- Student 4 → book_id = 4 % 3 = 1 → Book B

Loaning Rules:
- If the assigned book is available, the student loans it and sleeps for 5 seconds to simulate usage.
- If the book is already loaned, the student must wait using a pthread_cond_t condition variable.
- After 5 seconds, the student returns the book and notifies a waiting thread (if any).

Task:
Implement a thread-safe library system using pthread_mutex_t and pthread_cond_t to synchronize access to books.

Define the following interface:

    void* student_thread(void* arg); // Entry function for each student

    void loan_book(int student_id);
    void return_book(int book_id);

Implementation Requirements:
1. Use pthread_mutex_t to protect shared data (e.g., book availability).
2. Use pthread_cond_t to allow students to wait for books to become available.
3. Each book should have:
    - A flag for availability (e.g., int available = 1)
    - A pthread_mutex_t
    - A pthread_cond_t
4. The main function should:
    - Create 10 threads (students 0 to 9)
    - Wait for all threads to complete using pthread_join

Example Behavior:
- Student 3 (3 % 3 = 0) wants Book A.
- Student 6 (6 % 3 = 0) also wants Book A.
- Student 3 loans Book A and sleeps for 5 seconds.
- Student 6 waits on Book A’s condition variable.
- After 5 seconds, Student 3 calls return_book(0), and Student 6 is notified.

Objective:
Use pthreads, mutexes, and condition variables to ensure:
- No two students can loan the same book at the same time.
- Waiting students are properly notified when a book becomes available.
- Correct usage of pthread_cond_wait() and pthread_cond_signal().

Bonus (Optional):
Add print logs such as:
- "Student 4 is trying to loan Book B"
- "Student 4 has loaned Book B"
- "Student 4 returned Book B"
- "Student 7 is waiting for Book B"
------------------------------------------------------------------

Program Output (Screenshot): 
<img width="390" height="536" alt="Screenshot 2025-10-16 at 11 42 55 PM" src="https://github.com/user-attachments/assets/5fb7bc48-b50c-491c-bdbd-82ed3c40c604" />



