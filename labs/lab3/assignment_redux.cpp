// File: Lab 3 Assigment
// Authors: Josiah Campbell & Kevin Tarquinio
// Version: Winter 2016
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define RUNNING 1

using namespace std;

int status;
bool doneWithThis = false;

// Sighandler for Control+C
void sigHandler(int sigNum) {
    cout << " received" << endl;
    doneWithThis = true;
}

// Sighandler for signal int
void sigHandlerUser(int sigNum) {
    cout << "received a SIGUSR1 signal" << endl;
}

// Sighandler for signal int
void sigHandlerUserTwo(int sigNum) {
    cout << "received a SIGUSR2 signal" << endl;
}

int main () {
    pid_t pid;
    if ((pid = fork ()) < 0) {
        exit (1);
    } else if (pid == 0) {
        // Child logic
        int rd;
        signal(SIGINT, sigHandler);
        while (RUNNING) {
            sleep(rand() % 3 + 1);
            if (doneWithThis) {
                cout << "That's it, I'm shutting you down." << endl;
                exit(1);
            }
            rd = rand();
            if (rd % 2 == 0) {
                kill(getppid(), SIGUSR1);
            } else {
                kill(getppid(), SIGUSR2);
            }
        }
    }
    // Parent logic
    cout << "spawned child PID# " << pid << endl;
    signal(SIGUSR1, sigHandlerUser);
    signal(SIGUSR2, sigHandlerUserTwo); 
    while (!doneWithThis) {
        cout << "waiting...\t" << flush;
        pause();
    }
    wait(&status); 
    return 0;
}
