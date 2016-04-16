// is this Sample Program 1? <stdio.h>
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

using namespace std;

string print_permissions(const struct stat fileStat) {
    stringstream permissions;
    /* 
     * We "and" the st_mode attribute with the given 
     * bitmask fields to retrieve each permission that
     * the file has.
     */
    permissions << (S_ISDIR(fileStat.st_mode) ? "d" : "-")
        << ((fileStat.st_mode & S_IRUSR) ? "r" : "-")
        << ((fileStat.st_mode & S_IWUSR) ? "w" : "-")
        << ((fileStat.st_mode & S_IXUSR) ? "x" : "-")
        << ((fileStat.st_mode & S_IRGRP) ? "r" : "-")
        << ((fileStat.st_mode & S_IWGRP) ? "w" : "-")
        << ((fileStat.st_mode & S_IXGRP) ? "x" : "-")
        << ((fileStat.st_mode & S_IROTH) ? "r" : "-")
        << ((fileStat.st_mode & S_IWOTH) ? "w" : "-")
        << ((fileStat.st_mode & S_IXOTH) ? "x" : "-");
    
    return permissions.str();
}

string get_format_time(const time_t& time) {
    char timeBuf[32];
    if (strftime(timeBuf, sizeof(timeBuf), "%b %e %H:%M", localtime(&time))) {
        return timeBuf;
    }
    return "";
}

int main(int argc, char* argv[]) {
    const string workingDir = ".";
	int opt = 0;
    int inode_flag = 0;
    int long_flag = 0;
    int byte_flag = 0;
    int all_flag = 0;
    DIR *dirPtr;
	string listDir = "";
    struct dirent *entryPtr;
	struct stat statBuf;
    string current_file_name = "";

    // Check each of the ls-like flags provided by the user
    while ((opt = getopt(argc, argv, "ails")) != -1) {
        switch(opt) {
            case 'a':
                all_flag = 1;
                break;
            case 's':
                byte_flag = 1;
                break;
            case 'i':
                inode_flag = 1;
                break;
            case 'l':
                long_flag = 1;
                break;
            default: /* '?' */
                /* We shouldn't get here. */
                return -1;
        }
    }

    // If the user did not provide a directory, then we can default
    // to the current directory
    argc == optind ? listDir = "." : listDir = string(argv[optind]);

    // Attempt to open the current directory, but if the directory is invalid,
    // attempt to display file. Otherwise, report error to user.
    if (chdir(listDir.c_str()) < 0) {
        if ((stat(listDir.c_str(), &statBuf)) < 0) {
            cout << "our_ls: cannot access " << listDir
                << ": No such file or directory" << endl;
        } else {
            cout << listDir << endl;
        }
        return -1;
    }

    if ((dirPtr = opendir(workingDir.c_str())) == NULL) {
        cout << "That didn't work." << endl;
        return -1;
    }

    struct passwd* passwd;
    struct group* group;
    while ((entryPtr = readdir(dirPtr))) {
        // Run stat function
        stat(entryPtr->d_name, &statBuf);
        // If the current file is hidden, don't show it
        if (string((entryPtr->d_name)).at(0) == '.')
            if (all_flag != 1)
                continue;
        // Print inode value
        if (inode_flag) {
            cout << statBuf.st_ino << " " << flush;
        }
        // Print size in blocks
        if (byte_flag) {
            cout << statBuf.st_blocks << " " << flush;
        }
        // Print permissions for current dir/file
        // Print user/group ownership, size in bytes, and formatted time 
        if (long_flag) {
            passwd = getpwuid(statBuf.st_uid);
            group = getgrgid(statBuf.st_gid);

            cout << print_permissions(statBuf) << " " << statBuf.st_nlink;
            if (passwd != NULL) {
                cout << " " << getpwuid(statBuf.st_uid)->pw_name;
            }
            if (group != NULL) {
                cout << " " << getgrgid(statBuf.st_gid)->gr_name;
            }
            cout << "\t " << statBuf.st_size
                << "\t " << get_format_time(statBuf.st_mtime) 
                << "\t" << flush;
        }
        // Print actual filename, regardless
        cout << entryPtr->d_name << endl;
    }

    closedir(dirPtr);

    return 0;
}
