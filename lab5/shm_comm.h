#ifndef H_SHM_COMM
#define H_SHM_COMM

struct shm_comm_data {
    int read;
    bool newInfo;
    char user_input[2048];
};

#endif
