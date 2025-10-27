#include <stdio.h>      // input/output functions
#include <stdlib.h>     // general utilities (exit, malloc)
#include <unistd.h>     // fork, sleep
#include <sys/ipc.h>    // IPC key generation
#include <sys/shm.h>    // shared memory functions
#include <semaphore.h>  // semaphore functions
#include <fcntl.h>      // flags for sem_open
#include <sys/wait.h>   // wait for child

#define BUFFER_SIZE 5
typedef struct 
{
    int buffer[BUFFER_SIZE];
    int in;
    int out;
}
 SharedMemory;
int main()
{
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, sizeof(SharedMemory), 0666 | IPC_CREAT);
    SharedMemory *shm = (SharedMemory *)shmat(shmid, NULL, 0);
    sem_t *empty = sem_open("/empty", O_CREAT, 0666, BUFFER_SIZE);
    sem_t *full = sem_open("/full", O_CREAT, 0666, 0);
    sem_t *mutex = sem_open("/mutex", O_CREAT, 0666, 1);

    shm->in = 0;
    shm->out = 0;

    pid_t pid = fork();
    if (pid == 0) 
    {  
    for (int i = 1; i <= 10; i++) 
    {
            sem_wait(empty);
            sem_wait(mutex);
            shm->buffer[shm->in] = i;
            shm->in = (shm->in + 1) % BUFFER_SIZE;
            sem_post(mutex);
            sem_post(full);
            printf("Produced: %d\n", i);
            sleep(1);
        }
        exit(0);
    } 
    else
    {  
    for (int i = 1; i <= 10; i++) 
        {
            sem_wait(full);
            sem_wait(mutex);
            int item = shm->buffer[shm->out];
            shm->out = (shm->out + 1) % BUFFER_SIZE;
            sem_post(mutex);
            sem_post(empty);
            printf("Consumed: %d\n", item);
            sleep(2);
        }
        wait(NULL);
    }
    sem_close(empty);
    sem_close(full);
    sem_close(mutex);
    sem_unlink("/empty");
    sem_unlink("/full");
    sem_unlink("/mutex");
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
