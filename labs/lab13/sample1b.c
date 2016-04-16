// is this Sample Program 1?<stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
    struct flock fileLock;
    FILE* fileDescriptor;
    char* buffer = NULL;
    size_t lineSize = 60;
    const char* READ = "r";

    if (argc < 2) {
        printf ("usage: filename\n");
        exit (1);
    }
    if ((fileDescriptor = fopen (argv[1], READ)) < 0) { // read
        perror ("there is");
        exit (1);
    }

    fileLock.l_type = F_RDLCK; // Take  out a read lease
    fileLock.l_whence = SEEK_SET; // the beginning of the file is locked
    fileLock.l_start = 0;
    fileLock.l_len = 0;
    
    if (fcntl (fileno(fileDescriptor), F_SETLKW, &fileLock) < 0) {
        perror ("no way");
        exit (1);
    }

    getline(&buffer, &lineSize, fileDescriptor);
    printf("%s", buffer);
    sleep(5);
    
    fileLock.l_type = F_UNLCK;
    if (fcntl (fileno(fileDescriptor), F_SETLK, &fileLock) < 0) {
        perror ("no way");
        exit (1);
    }
    
    printf("we're awake. Closing file descriptor.\n");

    fclose (fileDescriptor);

    return 0;
}
