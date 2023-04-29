#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "utility/String.h"

bool validNumArgs(int allowedNumArgs, std::vector<std::string>& args) {
    return args.size() == allowedNumArgs + 1; // includes the command
}

int main() {
    // system properties
    int time; // initialize to starting time; increment by one unit
    int totalMemory;
    int availableMemory;
    int totalDevices;
    int availableDevices;
    int timeQuantum;

    // reading the input to get all properties
    std::ifstream infile("../inputs/i0.txt"); // make sure input folder is placed in the root directory
    std::string line; // currently-read line
    if (infile.is_open()) {
        while (std::getline(infile, line)) {
            // TODO: process input instead of printing
            std::cout << line << std::endl;

            auto words = string::split(line, ' '); // std::vector<std::string>
            for (const auto& word : words) {
                std::cout << word << std::endl;
            }

            std::string command = words[0]; // get the first character, this corresponds to the "command"
            if (command == "C") {
                std::cout << "OS initialization" << std::endl;
                if (validNumArgs(4, words)) {
                    std::cout << "Valid args! :D" << std::endl;
                }
            } else if (command == "A") {
                std::cout << "Job arrival" << std::endl;
                if (validNumArgs(6, words)) {
                    std::cout << "Valid args! :D" << std::endl;
                }
            } else if (command == "Q") {
                std::cout << "Device request" << std::endl;
                if (validNumArgs(3, words)) {
                    std::cout << "Valid args! :D" << std::endl;
                }
            } else if (command == "L") {
                std::cout << "Device release" << std::endl;
                if (validNumArgs(3, words)) {
                    std::cout << "Valid args! :D" << std::endl;
                }
            } else if (command == "D") {
                std::cout << "Display" << std::endl;
                if (validNumArgs(1, words)) {
                    std::cout << "Valid args! :D" << std::endl;
                }
            }
        }
        infile.close();
    } else {
        std::cout << "Unable to open file." << std::endl;
        return 0;
    }

    return 0;
}
