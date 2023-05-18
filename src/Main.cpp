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

    JobQueue readyQueue{JobQueueSortType::RR, "Ready Queue"};
    JobQueue waitQueue{JobQueueSortType::RR, "Wait Queue"};
    JobQueue CPUQueue{JobQueueSortType::NONE, "CPUQueue"};

    JobQueue completeQueue{JobQueueSortType::NONE, "Complete Queue"};

    // reading the input to get all properties
    std::ifstream infile("../inputs/i0.txt");
    if (!infile.is_open()) {
        std::cout << "Unable to open file." << std::endl;
        return 0;
    }

    std::string line;
    int commandArrival;
    while (std::getline(infile, line)) {
        CommandInfo command = parseCommand(line);

        switch(command.type) {
            case CommandType::SYSTEM: {
                auto info = std::get<CommandSystemInfo>(command.info);
                commandArrival = info.startTime;
                break;
            }
            case CommandType::NEW_JOB: {
                auto info = std::get<CommandNewJobInfo>(command.info);
                commandArrival = info.arrivalTime;
                break;
            }
            case CommandType::DEVICE_REQUEST: {
                auto info = std::get<CommandDeviceRequestInfo>(command.info);
                commandArrival = info.arrivalTime;
                break;
            }
            case CommandType::DEVICE_RELEASE: {
                auto info = std::get<CommandDeviceReleaseInfo>(command.info);
                commandArrival = info.arrivalTime;
                break;
            }
            case CommandType::DISPLAY: {
                auto info = std::get<CommandDisplayInfo>(command.info);
                commandArrival = info.arrivalTime;
                break;
            }
        }

        while (system.time < commandArrival) {
            system.time += 1;
            Job job;
            if (!hq1.isEmpty() || !hq2.isEmpty()){
                if (!hq1.isEmpty()){
                    Job pushFromHq1 = hq1.dequeue_front();
                    if (system.availableMemory >= pushFromHq1.memoryRequired) {
                        readyQueue.push_back(pushFromHq1);
                        system.availableMemory -= pushFromHq1.memoryRequired;
                    }
                    else {
                        hq1.push(pushFromHq1);
                    }
                }
                else {
                    Job pushFromHq2 = hq2.dequeue_front();
                    if (system.availableMemory >= pushFromHq2.memoryRequired){
                        readyQueue.push_back(pushFromHq2);
                        system.availableMemory -= pushFromHq2.memoryRequired;
                    }
                    else {
                        hq1.push(pushFromHq2);
                    }
                }
            }
            if (CPUQueue.isEmpty() && !readyQueue.isEmpty()){
                job = readyQueue.dequeue_front();
                CPUQueue.push_back(job);
            }
            if (!CPUQueue.isEmpty()){
                if (job.quantumLeft == system.quantum && !CPUQueue.isEmpty()){
                    job.quantumLeft = 0;
                    Job putBack = CPUQueue.dequeue_front();
                    putBack.currentTime = job.currentTime;
                    readyQueue.push_back(putBack);
                }
                if (CPUQueue.isEmpty() && !readyQueue.isEmpty()) {
                    job = readyQueue.dequeue_front();
                    CPUQueue.push_back(job);
                }
                if (job.currentTime < job.runningTime && job.quantumLeft < system.quantum) {
                    job.currentTime += 1;
                    job.quantumLeft += 1;
                }
                if (job.currentTime == job.runningTime && !CPUQueue.isEmpty()) {
                    system.availableMemory += job.memoryRequired;
                    Job completeJob = CPUQueue.dequeue_front();
                    completeJob.finishTime = system.time;
                    completeQueue.push_back(completeJob);
                }
            }
        }
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

                Job j{info.jobID, info.priority, info.arrivalTime, info.executionTimeLength, info.memoryRequired,
                      info.devicesRequired, 0, 0, -1};
                if (j.memoryRequired > system.totalMemory || j.devicesRequired > system.totalDevices)
                    break; // too much needed


                if (j.priority == 1) {
                    // put in SJF
                    hq1.push(j);
                }
                else {
                    // put in FIFO
                    hq2.push(j);
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

                std::cout << "At time " << info.arrivalTime << ":" << std::endl;

                std::cout << "Currently available main memory = " << system.availableMemory << ":" << std::endl;
                std::cout << "Currently available devices = " << system.availableDevices << ":" << std::endl;

                std::cout << std::string{completeQueue} << "\n" << std::endl;

                std::cout << std::string{hq1} << "\n" << std::endl;
                std::cout << std::string{hq2} << "\n" << std::endl;
                std::cout << std::string{readyQueue} << "\n" << std::endl;
                std::cout << "Process running on CPU:" << std::endl;
                std::cout << "// TODO" << "\n"
                          << std::endl; // todo: print job ID, time accrued, time left (runningTime - currentTime)
                std::cout << std::string{waitQueue} << "\n" << std::endl;

                if (info.arrivalTime == 9999) {
                    std::cout << "System turnaround time: " << std::endl;
                    std::cout << "// TODO" << std::endl; // todo: print system turnaround
                }
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
