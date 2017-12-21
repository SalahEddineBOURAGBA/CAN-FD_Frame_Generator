# CAN-FD_Frame_Generator
This program can generate random CAN/CAN FD frames, and analyse them.

To run the program you need to have g++ compiler installed, type "g++ --version" to check if it's installed, if not,
install it using the command line "sudo apt-get install g++".

Go the program directory in terminal, then run the following command: 
"g++ functions.cpp analyzer_functions.cpp generator_functions.cpp test.cpp main.cpp -o can -std=c++11"

Now run the executable with the command: "./can"
