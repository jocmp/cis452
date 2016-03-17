#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>

#define SIZE 16

int main (int argc, char* argv[]) {
    int status;
    long int i, loop, temp, *shmPtr;
    int shmId;
    pid_t pid;
    int semId;
    /* man semop:
       struct sembuf, containing the following members:
        unsigned short sem_num;   semaphore number
         short          sem_op;    semaphore operation
         short          sem_flg;   operation flags
    */
    struct sembuf m_wait = { 0, -1, SEM_UNDO };
    struct sembuf signal = { 0, 1, SEM_UNDO };

    if (argc < 2) {
        printf("Not enough arguments!");
        return 0;
    }
    loop = atol(argv[1]);
    if ((shmId = shmget (IPC_PRIVATE, SIZE,
                         IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        perror ("i can't get no..\n");
        exit (1);
    }
    if ((shmPtr = shmat (shmId, 0, 0)) == (void *) -1) {
        perror ("can't attach\n");
        exit (1);
    }
    shmPtr[0] = 0;
    shmPtr[1] = 1;
    /* create a new semaphore set for use by this (and other) processes..*/
    semId = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    /* initialize the semaphore set referenced by the
    previously obtained semIdhandle. */
    semctl(semId, 0, SETVAL, 1);

    if (!(pid = fork ())) {
        for (i = 0; i < loop; i++) {
            semop(semId, &m_wait, 1);
            temp = shmPtr[0];
            shmPtr[0] = shmPtr[1];
            shmPtr[1] = temp;
            semop(semId, &signal, 1);
        }
        if (shmdt (shmPtr) < 0) {
            perror ("just can 't let go\n");
            exit (1);
        }
        exit (0);
    } else {
        for (i = 0; i < loop; i++) {
            semop(semId, &m_wait, 1);
            temp = shmPtr[0];
            shmPtr[0] = shmPtr[1];
            shmPtr[1] = temp;
            semop(semId, &signal, 1);
        }
    }
    wait (&status);
    printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

    semctl(semId, 0, IPC_RMID);

    if (shmdt (shmPtr) < 0) {
        perror ("just can't let go\n");
        exit (1);
    }
    if (shmctl (shmId, IPC_RMID, 0) < 0) {
        perror ("can't deallocate\n");
        exit (1);
    }
    return 0;
}
