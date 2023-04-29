#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "utility/String.h"

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

            auto words = string::split(line, ' ');
            for (const auto& word : words) {
                std::cout << word << std::endl;
            }
        }
        infile.close();
    } else {
        std::cout << "Unable to open file." << std::endl;
        return 0;
    }

    return 0;
}
