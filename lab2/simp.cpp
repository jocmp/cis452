#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <sys/time.h>
#include <sys/resource.h>

/**
 * Filename: simp.cpp
 * Authors: Josiah Campbell & Kevin Tarquinio
 * Version: Winter 2016
 */

using namespace std;

const string HELP("\nEnter \"stats\" to toggle resource usage");
const string PTAG_START("[");
const string PTAG_END("]-> ");
const double MILLION(1000000);
const int RUNNING(1);

void put_user_input(vector<string>& vector) {
    string input_line;
    // read a line
    getline(cin, input_line);
    istringstream input_stream(input_line);
    string tok;
    while (input_stream >> tok) {
        vector.push_back(tok);
    }
}

void put_arg_vector(vector<string>& strings, vector<char*>& charps) {
    for(unsigned int i = 0; i < strings.size(); i++) {
        // In order to pass a non-const char* to the arguments
        // vector, we must un-cast the const from the return 
        // value of inputs[i].c_str(), which returns
        // a const char*
        charps.push_back(const_cast<char*>(strings[i].c_str()));
    }
    // man execvp line 36: 
    // "The list of arguments must be terminated by a NULL pointer"
    charps.push_back((char*) NULL);
}

int main(int argc, char* argv[]) {
    bool stats = false;
    struct rusage child_usage;
    vector<string> inputs;
    vector<char*> arguments;
    string first_token;
    int prompt_count = 0;
    // first execvp param
    const char* path;
    int status;
    pid_t pid;
    // Display a prompt to the user
    cout << "\nWELCOME TO \033[1;31mSIMP\033[0m: "
        << "SIMP IS MOSTLY POINTLESS" << HELP << endl;
    while(RUNNING) {
        prompt_count++;
        cout << PTAG_START << prompt_count << PTAG_END;
        // get and parse the user input
        put_user_input(inputs);
        // If the input is empty, then we should
        // allow the user to go to a new prompt
        if (inputs.empty()) {
            continue;
        }
        // Otherwise, we can get the first token
        first_token = inputs[0];
        // put_arg_vector moves string values into
        // mutable char* values
        put_arg_vector(inputs, arguments);
        path = first_token.c_str();
        if (first_token.compare("quit") == 0) {
            break;
        }
        if (first_token.compare("stats") == 0) {
            // Toggle resource usage info
            stats ? stats = false : stats = true;
            cout << "stats " << (stats ? "on" : "off") << endl;
            inputs.clear();
            arguments.clear(); 
            continue;
        }
        if ((pid = fork()) < 0) {
            perror("Fork failed.");
            exit(1);
        } else if (pid == 0) { 
            // Access to arguments vector is contiguous, so one
            // can pass the address-of the vector
            if (execvp(path, &arguments[0]) < 0) {
                perror("Exec failed");
                exit(1);
            }
            exit(3);
        } else {
            // wait3 will return the status and usage from 
            // any given child
            wait3(&status, 0, &child_usage);            
            if(stats) { /* If stats are enable */
                // User time is returned in microseconds (1e6 seconds).
                // We can convert it to seconds.
                double u_seconds = child_usage.ru_utime.tv_usec / MILLION;
                cout << "Involuntary Context Switches: "
                    << child_usage.ru_nivcsw << endl
                    << "User CPU Time: " << u_seconds << " seconds" << endl; 
            }
        }
        inputs.clear();
        arguments.clear();
    }
    return 0;
}

