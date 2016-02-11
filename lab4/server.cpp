#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <future>

#define RUNNING 1

using namespace std;
// Prototypes
void find_file_low(char* arg);
void *find_file_high(void *arg);

int serviced;
int prob[5] = { 1, 1, 1, 0, 1};

int main(int argc, char* argv[]) {
    int status;
    int prob_count = 1;
    serviced = 0;
    while (RUNNING) {
        string input;
        char* c_input;
        cout << "Enter a filename: ";
        cin >> input;
        c_input = const_cast<char*>(input.c_str());
        if (prob[prob_count]) {
              thread low(find_file_low, const_cast<char*>(input.c_str()));
              low.detach();
        } else {
            thread high(find_file_high, c_input);
            high.detach();
        }

        prob_count == 5 ? prob_count = 1 : prob_count++;
    }
    return 0;
}

void find_file_low(char* arg) {
    const char *val_ptr = arg;
    sleep((rand() % 4) + 7);
    cout << endl << "Couldn't find " << val_ptr << ". Sorry." << endl;
}

/* High probability routine */
void *find_file_high(void *arg) {
    const char *val_ptr = (char *)arg;
    sleep(1);
    cout << endl << "Found " << val_ptr << ".docx" << endl;
    return NULL;
}
