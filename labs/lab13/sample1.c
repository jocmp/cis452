// is this Sample Program 1?<sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#define SIZE 30

int main (int argc, char *argv[]) {
    struct flock fileLock;
    int fd;
    char buf[SIZE] = "// is this Sample Program 1?";

    if (argc < 2) {
        printf ("usage: filename\n");
        exit (1);
    }
    if ((fd = open (argv[1], O_RDWR)) < 0) { // read-write
        perror ("there is");
        exit (1);
    }

    fileLock.l_type = F_WRLCK; // Take  out a write lease
    fileLock.l_whence = SEEK_SET; // the beginning of the file is locked
    fileLock.l_start = 0;
    fileLock.l_len = 0;
    if (fcntl (fd, F_SETLK, &fileLock) < 0) {
        perror ("no way");
        exit (1);
    }

    write (fd, buf, SIZE - 2);

    sleep (2);
    
    fileLock.l_type = F_UNLCK;
    if (fcntl (fd, F_SETLK, &fileLock) < 0) {
        perror ("no way");
        exit (1);
    }
    
    printf("Unlocked. Now to sleep again.\n");

    sleep (20);
    
    printf("we're awake. Closing file descriptor.\n");

    close (fd);

    return 0;
}
