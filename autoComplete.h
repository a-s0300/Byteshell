#include<iostream>
#include<algorithm>
using namespace std;

vector<string> commands = {"cd", "ls", "pwd", "mkdir", "rm", "touch", "rename", "exit"};

char* command_generator(const char* text, int state) {
    static size_t list_index, len;
    if (!state) {
        list_index = 0;
        len = strlen(text);
    }

    while (list_index < commands.size()) {
        const string& command = commands[list_index++];
        if (command.compare(0, len, text) == 0) {
            return strdup(command.c_str());
        }
    }

    return nullptr;
}

char* file_generator(const char* text, int state) {
    static DIR* dir;
    static vector<string> files;
    static size_t list_index;
    static string dir_path;

    if (!state) {
        if (dir) {
            closedir(dir);
            dir = nullptr;
        }
        files.clear();
        list_index = 0;

        string input(text);
        size_t last_slash = input.find_last_of('/');
        if (last_slash != string::npos) {
            dir_path = input.substr(0, last_slash + 1);
            input = input.substr(last_slash + 1);
        } else {
            dir_path = "./";
        }

        dir = opendir(dir_path.c_str());
        if (!dir) {
            return nullptr;
        }

        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            string name(entry->d_name);
            if (name.compare(0, input.length(), input) == 0) {
                files.push_back(name);
            }
        }
        sort(files.begin(), files.end());
    }

    if (list_index < files.size()) {
        string completion = dir_path + files[list_index++];
        struct stat st;
        if (stat(completion.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
            completion += "/";
        }
        return strdup(completion.c_str());
    }

    return nullptr;
}

char** custom_completion(const char* text, int start, int end) {
    rl_attempted_completion_over = 1;
    if (start == 0) {
        return rl_completion_matches(text, command_generator);
    } else {
        return rl_completion_matches(text, file_generator);
    }
}