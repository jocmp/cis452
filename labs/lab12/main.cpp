#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <iostream>
#include <vector>
#include <pwd.h>
#include <grp.h>

using namespace std;

int print_permissions(const struct stat &fileStat) {

    cout << (S_ISDIR(fileStat.st_mode) ? "d" : "-")
        << ((fileStat.st_mode & S_IRUSR) ? "r" : "-")
        << ((fileStat.st_mode & S_IWUSR) ? "w" : "-")
        << ((fileStat.st_mode & S_IXUSR) ? "x" : "-")
        << ((fileStat.st_mode & S_IRGRP) ? "r" : "-")
        << ((fileStat.st_mode & S_IWGRP) ? "w" : "-")
        << ((fileStat.st_mode & S_IXGRP) ? "x" : "-")
        << ((fileStat.st_mode & S_IROTH) ? "r" : "-")
        << ((fileStat.st_mode & S_IWOTH) ? "w" : "-")
        << ((fileStat.st_mode & S_IXOTH) ? "x" : "-")
        << " " << flush;

    return 0;
}

int main(int argc, char* argv[]) {
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

    argc == optind ? listDir = "." : listDir = string(argv[optind]);

    if ((dirPtr = opendir(listDir.c_str())) == NULL) {
        cout << listDir << endl;
        return -1;
    }

    while ((entryPtr = readdir(dirPtr))) {
        // Run stat function
        current_file_name = entryPtr->d_name;

        stat(entryPtr->d_name, &statBuf);

        if (current_file_name.at(0) == '.') {
            if (all_flag != 1) {
                continue;
            }
        }

        if (inode_flag) {
            cout << statBuf.st_ino << " " << flush;
        }

        if (long_flag) {
            print_permissions(statBuf);
            cout << statBuf.st_nlink << "\t" << flush;
            cout << getpwuid(statBuf.st_uid)->pw_name
            << " " << getgrgid(statBuf.st_gid)->gr_name << "\t"
            << " " << statBuf.st_size << " "
            << " " << statBuf.st_mtime << " " << flush;
        }

        cout << entryPtr->d_name << flush;

        if (byte_flag) {
            cout << "\t" << statBuf.st_blocks << flush;
        }

        cout << endl;
    }

    closedir(dirPtr);

    return 0;
}