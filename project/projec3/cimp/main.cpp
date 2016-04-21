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
#include <getopt.h>
#include <cstring>

using namespace std;

const string WORKING_DIR = ".";

string get_format_time(const time_t& time) {
    char timeBuf[32];
    if (strftime(timeBuf, sizeof(timeBuf), "%b %e %H:%M", localtime(&time))) {
        return timeBuf;
    }
    return "";
}

vector<pair<pair<int, int>, vector<string>>> depth_first_search(const string dir_path) {
    DIR* dirPtr;
    struct dirent *entry_ptr;
    struct stat stat_buffer;
    int lstat_status = 0;
    string entry_path = "";
    int file_count = 0;
    int directory_size = 0;
    vector<pair<pair<int, int>, vector<string>>> sub_directory;

    if ((dirPtr = opendir(dir_path.c_str())) == NULL) {
        cout << "That didn't work." << endl;
        exit(1);
    }

    while ((entry_ptr = readdir(dirPtr))) {
        // Run stat function

        // Don't recurse through the current or parent directories
        if ((strcmp(entry_ptr->d_name, ".") == 0) ||
                (strcmp(entry_ptr->d_name, "..") == 0)) {
            continue;
        }
        entry_path = dir_path + "/" + entry_ptr->d_name;
        lstat_status = lstat(entry_ptr->d_name, &stat_buffer);
        if (lstat_status != 0) {
            cout << "Trouble opening " << entry_path << endl;
            exit(1);
        }
        if (S_ISLNK(stat_buffer.st_mode)) {
            continue;
        } else if (S_ISREG(stat_buffer.st_mode)) {
            directory_size += stat_buffer.st_size;
            file_count++;
        } else if (S_ISDIR(stat_buffer.st_mode)) {
            sub_directory = depth_first_search(entry_ptr->d_name);
        }
    }
}

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

    // If the user did not provide a directory, then we can default
    // to the current directory
    argc == optind ? root_path = "." : root_path = string(argv[optind]);

    return 0;
}
