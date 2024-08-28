#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <sys/types.h>
#include "execute.h"
using namespace std;
#define DELIMITER " \n\t\r"
vector<string> tokenize_input(const string &input) {
    vector<string> tokens;
    size_t start = 0;
    size_t end = 0;

    while ((start = input.find_first_not_of(DELIMITER, end)) != string::npos) {
        end = input.find_first_of(DELIMITER, start);
        try {
            tokens.push_back(input.substr(start, end - start));
        } catch (const std::exception &e) {
            cerr << "Error tokenizing input: " << e.what() << endl;
            tokens.clear();
            break;
        }
    }
    return tokens;
}
void execute_command(const vector<string>& tokens) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) { //check for child proces
        vector<char*> args;
        try {
            for (size_t i = 0; i < tokens.size(); ++i) {
                args.push_back(const_cast<char*>(tokens[i].c_str()));
            }
        } catch (const std::exception &e) {
            cerr << "Error handling arguments: " << e.what() << endl;
            exit(EXIT_FAILURE);
        }

        args.push_back(nullptr);

        if (execvp(args[0], args.data()) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {  // check for parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
        }

        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            cerr << "Command exited with non-zero status" << endl;
        } else if (WIFSIGNALED(status)) {
            cerr << "Command terminated by signal: " << WTERMSIG(status) << endl;
        }
    }
}

void process_input(const string &cmd) {
    vector<string> tokens = tokenize_input(cmd);

    if (tokens.empty()) {
        return;
    }

    if (tokens[0] == "cd") {
        execute_cd(tokens);
    } else if (tokens[0] == "pwd") {
        execute_pwd();
    } else if (tokens[0] == "ls") {
        execute_ls();
    } else if (tokens[0] == "mkdir") {
        execute_mkdir(tokens);
    } else if (tokens[0] == "rm") {
        execute_rm(tokens);
    } else if (tokens[0] == "touch") {
        execute_touch(tokens);
    } else if (tokens[0] == "rename") {
        execute_rename(tokens);
    } else if (tokens[0] == "exit") {
        exit(0);
    } else {
        execute_command(tokens);
    }
}
