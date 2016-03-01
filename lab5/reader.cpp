/* 
 * Author: Josiah Campbell & Kevin Tarquinio
 * Version: Winter 2016
 * Filename: reader.cpp
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <iostream>
#include "shm_comm.h"
#define FOO 4096

using namespace std;

int main (int argc, char* argv[]) {
    int shmId;
    struct shm_comm_data* shmPtr;
    key_t file_to_key;
    string input;
    file_to_key = ftok("../", 'K');

    if ((shmId =
         shmget (file_to_key, sizeof (shm_comm_data),
                 IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        perror ("i can't get no..\n");
        exit (1);
    }
    
    if ((shmPtr = (shm_comm_data*) shmat (shmId, 0, 0)) == (void *) -1) {
        perror ("can't attach\n");
        exit (1);
    }

    while(input.compare("quit") != 0) {
        if (shmPtr->newInfo == true && shmPtr->read < 1) {
            input = string(shmPtr->user_input); 
            cout << input << endl;
            shmPtr->read++;
            if (shmPtr->read > 1) 
                shmPtr->newInfo = false;
        }
    }

    if (shmctl (shmId, IPC_RMID, 0) < 0) {
       perror ("can't deallocate\n");
       exit (1);
    }

    return 0;
}
