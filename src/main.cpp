#include <iostream>
#include <fstream>
#include <string>

#include "Job.h"
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
                if (!validNumArgs(4, words)) {
                    std::cout << "Command C: Invalid arguments supplied" << std::endl;
                    return 0;
                }
                time = std::stoi(words[1]); // stoi = string to int
                totalMemory = std::stoi(words[2].substr(2));
                availableMemory = totalMemory;
                totalDevices = std::stoi(words[3].substr(2));
                availableDevices = totalDevices;
                timeQuantum = std::stoi(words[4].substr(2));
            } else if (command == "A") {
                std::cout << "Job arrival" << std::endl;
                if (!validNumArgs(6, words)) {
                    std::cout << "Command A: Invalid arguments supplied" << std::endl;
                    return 0;
                }
                Job j;
                j.arrivalTime = -1;
                j.maximumDevices = -1;
                j.priority = -1;
                j.requiredMemory = -1;
                j.runningTime = -1;
            } else if (command == "Q") {
                std::cout << "Device request" << std::endl;
                if (!validNumArgs(3, words)) {
                    std::cout << "Command Q: Invalid arguments supplied" << std::endl;
                    return 0;
                }
            } else if (command == "L") {
                std::cout << "Device release" << std::endl;
                if (!validNumArgs(3, words)) {
                    std::cout << "Command L: Invalid arguments supplied" << std::endl;
                    return 0;
                }
            } else if (command == "D") {
                std::cout << "Display" << std::endl;
                if (!validNumArgs(1, words)) {
                    std::cout << "Command D: Invalid arguments supplied" << std::endl;
                    return 0;
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
