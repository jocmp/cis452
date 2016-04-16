/*
 * Lab 13 Mini-prog
 * Josiah Campbell & Kevin Tarquinio
 * Winter 2016
 */
#include <dirent.h>
#include <unistd.h>                                                             
#include <stdlib.h>                                                             
#include <stdio.h>                                         
#include <sys/types.h>
#include <errno.h>
#include <iostream>


// man 2 link
// man 2/7 link
using namespace std;

int main(int argc, char* argv[]) {
    const string workingDir = ".";
    bool symbolic_flag = false;
    int opt = 0;
    string target = "";
    string linkpath = "";
    // Check each of the flags provided by the user
    while ((opt = getopt(argc, argv, "s")) != -1) {
        switch(opt) {
            case 's':
                symbolic_flag = true;
                break;
            default: /* '?' */
                /* We shouldn't get here. */
                return -1;
        }
    }

    /*
     * We need to make sure that they provided the correct
     * number of paths to link. Too few, and there's not 
     * enough info to call "link."
     *
     * We ignore extraneous arguments, but at least two must 
     * be supplied.
     */
    if (optind >= argc) {
        cout << "Expected argument after options\n" << endl;
        exit(1);
    } else if ((argc - optind) < 2) {
        cout << "usage: our_ln [-s] TARGET LINK NAME" << endl;
        exit(1);
    }

    target = argv[optind];
    linkpath = argv[optind + 1];
    
    // Link paths
    if (symbolic_flag) { /* -s flag supplied */
        symlink(target.c_str(), linkpath.c_str());
    } else { /* hard link */
        link(target.c_str(), linkpath.c_str());
    }
    // If the user did not provide a directory, then we can default
    // to the current directory
    // argc == optind ? listDir = "." : listDir = string(argv[optind]);

    return 0;
}