#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

#define READ 0
#define WRITE 1
#define MAX 1024
using namespace std;

int main () {
    int fd[2];
    ssize_t num;
    pid_t pid;
    char str[MAX];

    if (pipe (fd) < 0) {
        perror ("plumbing problem");
        exit (1);
    }
    if ((pid = fork ()) < 0) {
        perror ("fork failed");
        exit (1);
    } else if (!pid) {
        close (fd[READ]);
        string actual_str = "alright";
        int actual_size = actual_str.size() + 1;
        write (fd[WRITE], (const void *) actual_str.c_str(), (size_t) actual_size);
        exit (0);
    }
    close (fd[WRITE]);

    //point D
    num = read (fd[READ], (void *) str, (size_t) sizeof (str));
    if (num > MAX) {
        perror ("pipe read error\n");
        exit (1);
    }
    puts (str);
    return 0;
}
