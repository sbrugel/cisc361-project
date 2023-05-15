#include <iostream>
#include <fstream>
#include <string>

#include "Commands.h"
#include "Job.h"
#include "JobQueue.h"

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

    JobQueue hq1{JobQueueSortType::SJF, "Hold Queue 1"};
    JobQueue hq2{JobQueueSortType::FIFO, "Hold Queue 2"};

    JobQueue readyQueue{JobQueueSortType::RR, "Ready Queue"}; //
    JobQueue waitQueue{JobQueueSortType::RR, "Wait Queue"};

    JobQueue completeQueue{JobQueueSortType::NONE, "Complete Queue"};

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

                // reset queues
                hq1.clear();
                hq2.clear();
                readyQueue.clear();
                waitQueue.clear();

                // setup system
                system = System{};
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
                    readyQueue.push(j);
                    system.availableMemory = system.totalMemory - readyQueue.getTotalMemoryRequired();
                    std::cout << "memory available: " << system.availableMemory << " / " << system.totalMemory << std::endl;
                } else {
                    if (j.priority == 1) {
                        // put in SJF
                        std::cout << "adding it to the hold queue 1" << std::endl;
                        hq1.push(j);
                    } else {
                        // put in FIFO
                        std::cout << "adding it to the hold queue 2" << std::endl;
                        hq2.push(j);
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
                std::cout << "At time " << info.arrivalTime << ":" << std::endl;

                std::cout << "Currently available main memory = " << system.availableMemory << ":" << std::endl;
                std::cout << "Currently available devices = " << system.availableDevices << ":" << std::endl;

                std::cout << "Completed jobs: " << std::endl;
                std::cout << "// TODO" << "\n" << std::endl;

                std::cout << "Hold Queue 1:" << std::endl;
                std::cout << std::string{hq1} << "\n"  << std::endl;
                std::cout << "Hold Queue 2:" << std::endl;
                std::cout << std::string{hq2} << "\n"  << std::endl;
                std::cout << "Ready queue:" << std::endl;
                std::cout << std::string{readyQueue} << "\n"  << std::endl;
                std::cout << "Process running on CPU:" << std::endl;
                std::cout << "// TODO"  << "\n" << std::endl;
                std::cout << "Wait queue:" << std::endl;
                std::cout << std::string{waitQueue} << "\n"  << std::endl;

                std::cout << "System turnaround time: " << std::endl;
                std::cout << "// TODO" << std::endl;
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
