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
    } else if (pid == 0) {
        close (fd[READ]);
        string spawn = "spawned child PID# " + to_string(getpid());
        int str_size = spawn.size() + 1;
        write (fd[WRITE], (const void *) spawn.c_str(), (size_t) str_size);
        exit (0);
    }
    close (fd[WRITE]);
    num = read (fd[READ], (void *) str, (size_t) sizeof (str));
    if (num > MAX) {
        perror ("pipe read error\n");
        exit (1);
    }
    cout << str << endl;
    return 0;
}
