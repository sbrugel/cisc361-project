#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Commands.h"
#include "Job.h"
#include "HoldQueue.h"
#include "ProcessQueue.h"

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

    ProcessQueue readyQueue{"Ready Queue"}; //
    ProcessQueue waitQueue{"Wait Queue"};

    // reading the input to get all properties
    std::ifstream infile("../inputs/i2.txt");
    if (!infile.is_open()) {
        std::cout << "Unable to open file." << std::endl;
        return 0;
    }

    std::string line;
    while (std::getline(infile, line)) {
        CommandInfo command = parseCommand(line);

        // Debug commands
        // std::cout << std::string{command} << std::endl;

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

                std::cout << "a new job has arrived its Id is "
                          << j.id << " and has required memory of " << j.memoryRequired << std::endl;
                if (system.availableMemory >= j.memoryRequired) {
                    std::cout << "adding it to the ready queue" << std::endl;
                    readyQueue.enqueue(j);
                    system.availableMemory = system.totalMemory - readyQueue.getMemory();
                    std::cout << "memory available: " << system.availableMemory << " / " << system.totalMemory << std::endl;
                } else {
                    if (j.priority == 1) {
                        // put in SJF
                        std::cout << "adding it to the hold queue 1" << std::endl;
                        hq1.enqueue(j);
                    } else {
                        // put in FIFO
                        std::cout << "adding it to the hold queue 2" << std::endl;
                        hq2.enqueue(j);
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

    std::cout << std::string{readyQueue} << std::endl;
    std::cout << std::string{hq1} << std::endl;
    std::cout << std::string{hq2} << std::endl;

    infile.close();
    return 0;
}
