Name: Yashwanth Reddy Medapati
B-Number: B01038355
BU Email ID: vmedapati@binghamton.edu

Project: CS551 — Programming Project 2

My implementation includes mybarrier_init(), mybarrier_wait(), and mybarrier_destroy() using Pthread mutexes and condition variables.
The barrier blocks until the specified number of threads arrive, then releases all threads.
After release, the barrier cannot be reused and any additional calls return -1.
mybarrier_destroy() waits for all threads to exit the wait function before freeing resources.

All required functions compile and run correctly.
