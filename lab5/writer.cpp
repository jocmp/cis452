#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include "shm_comm.h"

#define FOO 4096
#define PROJ_ID 16

using namespace std;


int main (int argc, char* argv[]) {
    int shmId;
    struct shm_comm_data* shmPtr;
    string input;
    key_t file_to_key = ftok("../", 'K');

    if ((shmId = shmget (file_to_key, sizeof (shm_comm_data),
                    IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        perror ("i can't get no..\n");
        exit (1);
    }

    if ((shmPtr = (shm_comm_data*) shmat(shmId, 0, 0)) == (void *) -1) {
        perror ("can't attach\n");
        exit (1);
    }
    
    while(input.compare("quit") != 0) {
        cin >> input;
        strcpy(shmPtr->user_input, input.c_str());
        shmPtr->read = 0;
        shmPtr->newInfo = true;
    }

    if (shmctl (shmId, IPC_RMID, 0) < 0) {
        perror ("can't deallocate\n");
        exit (1);
    }

    return 0;
}
