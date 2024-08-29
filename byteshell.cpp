#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include "token_ip.h"
#include "autoComplete.h"
using namespace std;

#define DELIMITER " \n\t\r"
int main() {
    char* cmd;
    cout << "                       __Welcome to Byteshell__            " << endl;
    rl_attempted_completion_function = custom_completion;

    while (true) {
        cmd = readline("Byteshell> ");

        if (cmd == nullptr) {
            if (feof(stdin)) {
                cout << endl;
                break;
            } else {
                perror("readline");
                continue;
            }
        }

        if (strlen(cmd) > 0) {
            add_history(cmd);
        }
        process_input(cmd);

        free(cmd);
    }
    return 0;
}
