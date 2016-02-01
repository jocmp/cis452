#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define READ 0
#define WRITE 1
#define MAX 1024

void sigUserOne(int sigNum) {


}

int main () {
    int pvc[2];
    ssize_t num;
    pid_t pid;
    char str[MAX];

    if (pipe (pvc) < 0) {
        perror ("plumbing problem");
        exit (1);
    }
    if ((pid = fork ()) < 0) {
        perror ("fork failed");
        exit (1);
    } else if (!pid) {
        dup2 (pvc[WRITE], STDOUT_FILENO);
        close (pvc[WRITE]);
        exit (0);
    }
    num = read (STDIN_FILENO, (void *) str, (size_t) sizeof (str));
    if (num > MAX) {
        perror ("pipe read error\n");
        exit (1);
    }
    puts (str);
    return 0;
}
