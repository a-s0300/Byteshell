# Byteshell
Byteshell - custom linux terminal

- To compile : 
                g++ -o byteshell byteshell.cpp -lreadline
- To run      :
                ./byteshell

- This shell can perform the following commands :
                - cd directory
                    changes the current working directory
                - ls
                    lists the files and folders in the current directory
                - rename oldfilename newfilename
                    renames the file 
                - touch filename
                    creates an empty file
                - pwd
                    prints the current working directory
                - rm filename/directory
                    removes the file/directory
                - mkdir directory
                    creates a new directory
                - exit
                    exits the program

- Additional features : 
                          command and filename/directory completition using TAB
                          maintains the command history which can be accessed using arrow keys
                          robust error handling capabilities
