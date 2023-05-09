#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Commands.h"
#include "Job.h"
#include "HoldQueue.h"

struct System {
    int time; // the current time; gets incremented
    int totalMemory;
    int availableMemory;
    int totalDevices;
    int availableDevices;
    int quantum;
};

int main() {
    System system{};

    HoldQueue hq1{true}; // SJF
    HoldQueue hq2{false}; // FIFO

    // reading the input to get all properties
    std::ifstream infile("../inputs/i0.txt");
    if (!infile.is_open()) {
        std::cout << "Unable to open file." << std::endl;
        return 0;
    }

    std::string line;
    while (std::getline(infile, line)) {
        CommandInfo command = parseCommand(line);

        // Debug commands
        std::cout << std::string{command} << std::endl;

        // Let's see what command it is!
        switch (command.type) {
            case CommandType::SYSTEM: {
                auto info = std::get<CommandSystemInfo>(command.info);

                // clear previous information
                hq1.clearQueue();
                hq2.clearQueue();

                // setup system
                system.time = info.startTime;
                system.totalMemory = info.memoryAmount;
                system.availableMemory = system.totalMemory;
                system.totalDevices = info.deviceAmount;
                system.availableDevices = system.totalDevices;
                system.quantum = info.quantum;

                break;
            }
            case CommandType::NEW_JOB: {
                auto info = std::get<CommandNewJobInfo>(command.info);

                Job j{info.jobID, info.priority, info.arrivalTime, info.executionTimeLength, info.memoryRequired, info.devicesRequired, 0};
                if (j.memoryRequired > system.totalMemory || j.devicesRequired > system.totalDevices) break; // too much needed

                std::cout << "oh dang!!!! a new job has arrived at time " << system.time << "!!! its Id is "
                          << j.id << std::endl;
                if (system.availableMemory >= j.memoryRequired) {
                    std::cout << "going to ready queue" << std::endl;
                } else {
                    if (j.priority == 1) {
                        // put in SJF
                        hq1.enqueue(j);
                        std::cout << std::string{hq1};
                    } else {
                        // put in FIFO
                        hq2.enqueue(j);
                        std::cout << std::string{hq2};
                    }
                }

                break;
            }
            case CommandType::DEVICE_REQUEST: {
                auto info = std::get<CommandDeviceRequestInfo>(command.info);
                // todo: do stuff with device request command
                break;
            }
            case CommandType::DEVICE_RELEASE: {
                auto info = std::get<CommandDeviceReleaseInfo>(command.info);
                // todo: do stuff with device release command
                break;
            }
            case CommandType::DISPLAY: {
                auto info = std::get<CommandDisplayInfo>(command.info);
                // todo: do stuff with display command
                break;
            }
            default:
                std::cout << "Broken line: \"" << line << '\"' << std::endl;
                return 0;
        }
    }

    infile.close();
    return 0;
}
