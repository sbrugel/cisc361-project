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

    std::vector<Job> jobs;

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
                jobs.push_back(j); // add the job to the list of all system jobs

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

    const int MAX_TIME = 50; // temporary but we still need a maximum time of some sort
    std::vector<Job>::iterator jobIter;
    while (system.time < MAX_TIME) {
        std::cout << "the current system time is " << system.time << std::endl;

        for (jobIter = jobs.begin(); jobIter != jobs.end(); ++jobIter) {
            if (jobIter->arrivalTime == system.time) {
                std::cout << "oh dang!!!! a new job has arrived at time " << system.time << "!!! its Id is "
                          << jobIter->id << std::endl;
                if (system.availableMemory >= jobIter->memoryRequired) {
                    std::cout << "going to ready queue" << std::endl;
                } else {
                    if (jobIter->priority == 1) {
                        // put in SJF
                        hq1.enqueue(*jobIter);
                        hq1.printQueue();
                    } else {
                        // put in FIFO
                        hq2.enqueue(*jobIter);
                        hq2.printQueue();
                    }
                }
            }
        }

        system.time++;
    }
    infile.close();
    return 0;
}
