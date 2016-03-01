/* 
 * Author: Josiah Campbell & Kevin Tarquinio
 * Version: Winter 2016
 * Reminder for compiling:
 * g++ -Wall -g -lpthread -std=c++11 server.cpp -o serv
 */
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <signal.h>

#define RUNNING 1

using namespace std;

// Prototypes
void *find_file(string arg);
void sig_handler(int sig_num);

int serviced;
bool accepting_input;

int main(int argc, char* argv[]) {
    serviced = 0;
    accepting_input = true; 
    signal(SIGINT, sig_handler);
    while (RUNNING) {
        string input;
        if (accepting_input) {
            cout << "Enter a filename: " << flush;
            cin >> input;
            thread file(find_file, input); 
            // Thread can run independently of dispatcher
            // and free memory after finishing
            file.detach(); 
        }
    }
    return 0;
}

void sig_handler(int sig_num) {
    cout << " received" << endl;
    accepting_input = false;
    cout << "We handled " << serviced << " file queries." << endl;
    exit(0);
}

void *find_file(string arg) {
    int probability = rand() % 101;
    if (probability > 20) {
        sleep(1);
        cout << endl << "Found " << arg << ".docx" << endl;
    } else {
        sleep((rand() % 4) + 7);
        cout << endl << "Couldn't find " << arg << ".jsf Sorry." << endl;
    }
    // We handled another file. 
    serviced++;
    return NULL;
}
