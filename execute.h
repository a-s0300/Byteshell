#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <fcntl.h>
#include <sys/types.h>
using namespace std;
#define DELIMITER " \n\t\r"

void execute_cd(const vector<string>& tokens) {
    if (tokens.size() < 2) {
        cerr << "cd: expected argument" << endl;
        return;
    }

    if (chdir(tokens[1].c_str()) != 0) {
        perror("cd");
    }
}

void execute_rename(const vector<string>& tokens) {
    if (tokens.size() < 3) {
        cerr << "rename: expected two arguments (old_name new_name)" << endl;
        return;
    }

    const char *old_name = tokens[1].c_str();
    const char *new_name = tokens[2].c_str();

    if (rename(old_name, new_name) != 0) {
        perror("rename");
    } else {
        cout << "File renamed successfully." << endl;
    }
}

void execute_ls() {
    DIR *dir = opendir(".");
    if (dir == nullptr) {
        perror("ls");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        cout << entry->d_name << "  ";
    }

    if (closedir(dir) != 0) {
        perror("ls: closedir");
    }

    cout << endl;
}

void execute_pwd() {
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) == nullptr) {
        perror("pwd");
        return;
    }

    cout << cwd << endl;
}


void execute_mkdir(const vector<string>& tokens) {
    if (tokens.size() < 2) {
        cerr << "mkdir: expected directory name" << endl;
        return;
    }

    if (mkdir(tokens[1].c_str(), 0755) != 0) {
        perror("mkdir");
    }
}


void execute_rm(const vector<string>& tokens) {
    if (tokens.size() < 2) {
        cerr << "rm: expected file or directory name" << endl;
        return;
    }

    struct stat statbuf;
    if (stat(tokens[1].c_str(), &statbuf) != 0) {
        perror("rm: stat");
        return;
    }

    if (S_ISDIR(statbuf.st_mode)) {
        if (rmdir(tokens[1].c_str()) != 0) {
            perror("rmdir");
        }
    } else {
        if (unlink(tokens[1].c_str()) != 0) {
            perror("unlink");
        }
    }
}

void execute_touch(const vector<string>& tokens) {
    if (tokens.size() < 2) {
        cerr << "touch: expected file name" << endl;
        return;
    }
    const char *filename = tokens[1].c_str();
    int fd = open(filename, O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("touch");
        return;
    }
    if (close(fd) != 0) {
        perror("touch: close");
    }
}
