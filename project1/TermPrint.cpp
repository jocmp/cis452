#include <curses.h>
#include <term.h>
#include <iostream>
#include <sstream>
#include <string>

/**
 * Filename: TermPrint.cpp
 * Author: Josiah Campbell
 * Version: Winter 2016
 */
using namespace std;

// Prototypes
void display_screen_size();
void display_name_label();

const string NAMELABEL("Name: ");
const string NAMEGREET("Hello, ");
const string ENTERLABEL("Enter input: ");

int main (int argc, char* argv[]) {
    string user_input;
    int enter_line;
    // Setup terminal window
    /* Thus, the simplest setup call for 
     * term setup
     */
    setupterm((char *)0, 1, (int *)0);
    /* terminfo(5) Line 292
     * clear screen and home cursor (P*)
     */
    enter_line = lines - 2; 
    putp(clear_screen);
    display_screen_size(); 
    display_name_label();
    putp(tparm(cursor_address, enter_line, 0));
    putp(ENTERLABEL.c_str());
    while (user_input.compare("quit") != 0) {
        getline(cin, user_input);
        if (user_input.size() > 0) {
            putp(tparm(cursor_address, (lines / 2), 
                    (columns / 2)));
            putp(clr_eol);  
            putp(user_input.c_str());
        }
        putp(tparm(cursor_address, enter_line, 0));
        putp(clr_eol);
        putp(ENTERLABEL.c_str());
    }
    // Reset the screen and move the cursor home
    putp(clear_screen);
    putp(cursor_home);
    return 0;
}

void display_screen_size() {
    // Display row and column count 
    ostringstream text;
    text << "This window has " << lines << " rows and " << columns 
        << " columns." << endl;
    putp(tparm(cursor_address, 0, 0));
    putp(text.str().c_str());
}

void display_name_label() {
    // Move cursor to the screen center and 
    // display name label
    putp(tparm(cursor_address, (lines / 2), (columns / 2) - NAMELABEL.size()));
    putp(NAMELABEL.c_str());
}
