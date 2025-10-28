# Project Description

This project shows how two processes - a producer and a consumer, work together using shared memory and semaphores.


The producer keeps putting data into a shared memory buffer, and the consumer keeps taking that data out one by one.Both can’t access the shared space at the same time, so I used semaphores to make sure they take turns properly without any overlap.


Everything is done at the process level using fork() instead of threads.


The main goal of this project is to show how synchronization, shared memory handling, and inter-process communication actually work together in a real system.This has helped me understand how to control access between processes and how semaphores keep everything balanced when multiple processes share the same resources.

To compile and run the program, I have used the GCC compiler in Linux.
The command I used was:

gcc -o prod_cons producer_consumer.c -lrt -pthread

After that, I ran it with ./prod_cons, which starts both the producer and consumer processes.

If any shared memory error shows up, it usually means an old memory segment is still active.
In that case, I checked it using ipcs -m and removed the stuck one with ipcrm -m <shmid>.
After doing that, everything ran smoothly again.

In brief, this project has helped me understand how producers and consumers communicate safely using shared memory, and how semaphores manage synchronization between them.
