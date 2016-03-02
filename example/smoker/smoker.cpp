//
// Created by Hans Dulimarta on 2/15/16.
//

#include <thread>
#include <random>
#include <vector>
#include <iostream>
#include <string>
#include <term.h>
#include <curses.h>
#include <unistd.h>

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

#include "gv_semaphore.h"
//enum supply {PAPER, MATCH, TOBACCO};
vector<string> SUPPLY_NAME {"Paper  ", "Match  ", "Tobacco"};
std::mutex table_empty, observe_mutex;
gvsu::semaphore supplies[3];
gvsu::semaphore go[3];
bool supply_avail[3];
std::mutex output;
std::random_device rd;

void show_text_at (const string& txt, int r, int c) {
    output.lock();
    putp (tparm(tigetstr((char*)"cup"), r, c));
    cout << txt << endl;
    cout.flush();
    output.unlock();
}

void clear_text_at (int r, int c) {
    output.lock();
    putp (tparm(tigetstr((char*)"cup"), r, c));
    putp (tigetstr((char*)"el"));
    output.unlock();
}

void do_agent(int k) {
    std::uniform_int_distribution<int> supply_time(800, 2500);
    std::uniform_int_distribution<short> odd_even (0, 9);
    int item1 = (k + 2) % 3;
    int item2 = (k + 1) % 3;

    while (true) {
        table_empty.lock();
        int delay = supply_time(rd);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        short val = odd_even(rd);
        /* swap item type randomly */
        if (val % 2 == 0)
            std::swap(item1, item2);
        string msg = "Agent supplies " +
            SUPPLY_NAME[item1] + " and " + SUPPLY_NAME[item2];
        show_text_at(msg, 0, 0);
        supplies[item1].signal();
        supplies[item2].signal();
    }
}

void do_observe (int k) {

    int item1 = (k + 2) % 3;
    int item2 = (k + 1) % 3;
    string me = SUPPLY_NAME[k] + " observer";
    show_text_at(me, 2 + k , 0);
    string msg;
    while (true) {
        supplies[k].wait();
        clear_text_at(k + 2, 17);
        observe_mutex.lock();
        if (supply_avail[item1]) {
            supply_avail[item1] = false;
            msg = "found " + SUPPLY_NAME[item1] + " notify smoker with "
                  + SUPPLY_NAME[item2];
            go[item2].signal();
        }
        else if (supply_avail[item2]) {
            supply_avail[item2] = false;
            msg = "found " + SUPPLY_NAME[item2] + " notify smoker with "
                  + SUPPLY_NAME[item1];
            go[item1].signal();
        }
        else {
            msg = "is active";
            supply_avail[k] = true;
        }
        show_text_at (msg, k + 2, 17);
        observe_mutex.unlock();
    }
}

void do_smoke (int i) {
    std::uniform_int_distribution<int> prep_time(800, 2800);
    std::uniform_int_distribution<int> smoke_time(2000, 4000);
    show_text_at("Smoker with " + SUPPLY_NAME[i], i + 6, 0);
    int delay;
    string msg;

    while (true) {
        clear_text_at (i + 6, 20);
        show_text_at("waiting", i + 6, 20);
        go [i].wait();
        table_empty.unlock();
        clear_text_at (i + 6, 20);
        show_text_at("making cigarette", i + 6, 28);
        delay = prep_time(rd);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        clear_text_at (i + 6, 20);
        show_text_at("is smoking", i + 6, 45);
        delay = smoke_time(rd);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}

int main() {

    int rc, err;
    rc = setupterm (NULL, fileno(stdout), &err);
    if (rc == ERR) {
        cerr << "Terminal initialization error" << endl;
        exit (-1);
    }
    supply_avail[0] = supply_avail[1] = supply_avail[2] = false;
    putp (tigetstr((char *) "clear"));
    putp (tigetstr((char *) "smcup"));
    for (int k = 0; k < 3; k++) {
        std::thread agent(do_agent, k);
        agent.detach();

        std::thread observer (do_observe, k);
        observer.detach();

        std::thread smoker (do_smoke, k);
        smoker.detach();
    }
    pause();
    putp (tigetstr((char *) "rmcup"));
    return 0;
}
