#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <ctime>
#include <set>

using namespace std;

int main(int argc, char* argv[]) {
    bool symbolic = false;
    int opt = 0;
    string root_path = "";
    while ((opt = getopt(argc, argv, "shec")) != -1) {
        switch(opt) {
            case 's':
                symbolic = true;
                break;
            default: /* '?' */
                /* We shouldn't get here. */
                return -1;
        }
    }

    if (optind >= argc) {
        cout << "Expected argument after options\n" << endl;
        exit(1);
    } else if ((argc - optind) < 2) {
        exit(1);
    }

    root_path = argv[optind];

    // Link paths
    if (symbolic) { /* -s flag supplied */
        symlink(target.c_str(), linkpath.c_str());
    } else { /* hard link */
        link(target.c_str(), linkpath.c_str());
    }

    return 0;
}
