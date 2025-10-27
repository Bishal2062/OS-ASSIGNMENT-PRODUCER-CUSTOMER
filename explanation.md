# Explanation –--> Producer Consumer using Shared Memory & Semaphores

This file below explains what my program does, how I built it, and what’s actually happening in the code behind the scene.

## Description of the program

The whole idea is about two processes — Producer and Consumer that work together using a shared memory.
- The Producer keeps the data (like certain numbers or messages) into a shared space.
- The Consumer then takes that data out one by one.
- But both of them have to take turns and not access the shared area at the same time; otherwise things are going to break

To handle this, I used semaphores to synchronize them and make sure they don’t mess with each other. 

## Here's how it works (Step-by-Step)

1. First, I create a shared memory
 I used `shmget()` and `shmat()` to make a shared segment both processes can access.

2. Then I create semaphores
   One semaphore for “full slots” → tells consumer if there’s data to consume. 
   One semaphore for “empty slots” → tells producer if it can add new data.   
   One semaphore for "mutex" → ensures only one process accesses memory at a time.

3. After that i fork the processes:  
   I used `fork()` so the same program splits into two — one acting as the producer and the other as the consumer.

4. Producer Loop:  
   The producer keeps adding items to the shared memory buffer until it’s full.  
   It waits if there’s no space (empty == 0).

5. Consumer Loop:  
   The consumer keeps taking items from the buffer one by one.  
   It waits if there’s nothing there (full == 0).

6. Synchronization:  
   Every time producer or consumer wants to enter the shared area, they `wait(mutex)`.  
   After they’re done, they `signal(mutex)` to release it.

## Key Components Explained :

Semaphores are like counters that help control access between the producer and consumer. I am using three of them in my program; one for counting empty slots, one for full slots, and one for mutual exclusion. The mutex part is for making sure both processes don’t enter the shared area at the same time in order to prevent them from breaking.

Shared memory is the space where both processes read and write data. I have created it using shmget() to make the shared memory segment and shmat() to attach it to both processes so they can use the same area.

Mutual exclusion means only one process can use the shared buffer at a time. I handled this by locking with a semaphore before accessing the buffer and unlocking it after finishing, so that no overlapping happens.

Process communication is done at the process level, not thread level. I have used fork() to make two processes — one acts as the producer and the other as the consumer. Both attach to the same shared memory and stay in sync using semaphores.


## Compilation and How to Run

To compile the code, I opened the terminal and used the GCC compiler. The command I used was 
gcc -o prod_cons producer_consumer.c -lrt -pthread
 This created an executable file called prod_cons from my C code.

After compiling, I ran the program by typing 
./prod_cons
 That’s what actually starts the producer and consumer processes and shows the output.

If any shared memory error shows up, it means that an old shared memory segment is still there.
For fixing that, I checked all active shared memories using ipcs -m and then removed the one that was stuck using 
ipcrm -m <shmid> 