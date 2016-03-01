/* 
 * Author: Josiah Campbell & Kevin Tarquinio
 * Version: Winter 2016
 * Filename: shm_comm.h
 */
#ifndef H_SHM_COMM
#define H_SHM_COMM

struct shm_comm_data {
    int read;
    bool newInfo;
    char user_input[2048];
};

#endif
