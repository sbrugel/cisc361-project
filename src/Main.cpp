#include <algorithm>
#include <cstdlib>
#include <memory>

#include "utility/String.h"
#include "Bankers.h"
#include "Commands.h"
#include "Input.h"
#include "System.h"

// Anonymous namespace = TU-local methods
namespace {

void printHelp() {
    std::cout << "--- CISC361 Term Project ---\n"
                 "Options:\n"
                 "-h : Prints this help message\n"
                 "-f <file> : Runs the given input file\n"
                 "-f <number> : Runs the given input file based on its number\n"
                 "-i : Interactive mode\n" << std::endl;
}

[[nodiscard]] std::string_view getOptionValue(const int argc, const char* const argv[], std::string_view option, std::string_view default_ = "") {
    auto end = argv + argc;
    auto it = std::find(argv, end, option);
    if (it != end && ++it != end) {
        return *it;
    }
    return default_;
}

[[nodiscard]] bool optionExists(const int argc, const char* const argv[], std::string_view option) {
    return std::find(argv, argv + argc, option) != (argv + argc);
}

[[nodiscard]] std::unique_ptr<IInput> getInput(const int argc, const char* const argv[]) {
    if (optionExists(argc, argv, "-h")) {
        printHelp();
        exit(EXIT_SUCCESS);
    } else if (optionExists(argc, argv, "-i")) {
        return std::make_unique<TerminalInput>();
    } else if (optionExists(argc, argv, "-f")) {
        auto value = getOptionValue(argc, argv, "-f");
        if (value == "0" || value == "1" || value == "2") {
            return std::make_unique<FileInput>(string::format("../inputs/i%d.txt", std::stoi(value.data())));
        }
        return std::make_unique<FileInput>(value);
    }
    return nullptr;
}

[[nodiscard]] bool isSystemSafeAfterDeviceRequest(const System& s, CommandDeviceRequestInfo info) {
    if (s.availableDevices < info.devicesRequested)
        return false;

    std::vector<int> max;
    std::vector<int> available;

    for (Job job : s.cpuQueue) {
        available.push_back(job.devicesRequired);
        if (job.id == info.jobID) {
            // NOTE: THIS DOES NOT UPDATE THE JOB IN THE QUEUE!!
            job.devicesHeld += info.devicesRequested;
        }
        max.push_back(job.devicesRequired);
    }

    return reducedBankers(max, available, s.availableDevices);
}

[[nodiscard]] bool isQueueSafeAfterAddingJob(const JobQueue& queue, Job j, int totalDevices) {
    int freeDevices = totalDevices - queue.getTotalDevicesHeld() - j.devicesHeld;


    if (freeDevices < 0) {
        return false;
    }

    std::vector<int> max;
    std::vector<int> available;

    for (Job job : queue) {
        available.push_back(job.devicesRequired);
        max.push_back(job.devicesRequired);
    }
    available.push_back(0);
    max.push_back(j.devicesRequired);

    return reducedBankers(max, available, freeDevices);
}

} // namespace

int main(const int argc, const char* const argv[]) {
    System s{};

    // reading the input to get all properties
    auto input = getInput(argc, argv);
    if (!input || !input->isValid()) {
        std::cout << "Unable to read input. Did you specify a valid command-line option?" << '\n' << std::endl;
        printHelp();
        return EXIT_FAILURE;
    }

    bool firstJob = true;
    while (auto next = input->nextLine()) {
        std::string line = *next;
        CommandInfo command = parseCommand(line);

        // Handle this here, because we don't want to update the simulation if true
        if (command.type == CommandType::SYSTEM) {
            // Reset everything
            auto info = std::get<CommandSystemInfo>(command.info);

            s = System{};
            s.time = command.time;
            s.totalMemory = info.memoryAmount;
            s.availableMemory = info.memoryAmount;
            s.totalDevices = info.deviceAmount;
            s.availableDevices = info.deviceAmount;
            s.quantum = info.quantum;
            firstJob = true;

            // Next line!
            continue;
        }

        int timeBreak = -1;
        if (command.type == CommandType::DEVICE_REQUEST || command.type == CommandType::DEVICE_RELEASE) {
            timeBreak = command.time;
        }

        // Update simulation to latest timestamp
        while (s.time < command.time) {
            s.time += 1;
            if (!s.waitQueue.isEmpty()){
                auto wQCopy = s.waitQueue;
                for (auto job : wQCopy){
                    bool safe = isQueueSafeAfterAddingJob(s.readyQueue, job, s.totalDevices);
                    if (safe) {
                        s.readyQueue.push(job);
                        s.availableDevices -= job.devicesHeld;
                        s.waitQueue.remove(job.id);
                    }
                    else {
                        s.readyQueue.remove(job.id);
                    }
                }
            }
            if (!s.hq1.isEmpty() || !s.hq2.isEmpty()) {
                if (!s.hq1.isEmpty()) {
                    Job pushFromHq1 = s.hq1.pop();
                    if (s.availableMemory >= pushFromHq1.memoryRequired) {
                        s.readyQueue.push(pushFromHq1);
                        s.availableMemory -= pushFromHq1.memoryRequired;
                    } else {
                        s.hq1.push(pushFromHq1);
                    }
                } else {
                    Job pushFromHq2 = s.hq2.pop();
                    if (s.availableMemory >= pushFromHq2.memoryRequired) {
                        s.readyQueue.push(pushFromHq2);
                        s.availableMemory -= pushFromHq2.memoryRequired;
                    } else {
                        s.hq2.push(pushFromHq2);
                    }
                }
            }
            if (s.cpuQueue.isEmpty() && !s.readyQueue.isEmpty()) {
                Job job = s.readyQueue.pop();
                s.cpuQueue.push(job);
            }
            if (!s.cpuQueue.isEmpty()) {
                Job job = s.cpuQueue.peek();
                if (job.currentTime < job.runningTime && job.quantumLeft < s.quantum) {
                    job = s.cpuQueue.pop();
                    job.currentTime += 1;
                    job.quantumLeft += 1;
                    s.cpuQueue.push(job);
                }
                if (job.quantumLeft == s.quantum && !s.cpuQueue.isEmpty() && job.currentTime != job.runningTime) {
                    Job putBack = s.cpuQueue.pop();
                    putBack.quantumLeft = 0;
                    putBack.currentTime = job.currentTime;
                    s.readyQueue.push(putBack);
                }
                if (s.cpuQueue.isEmpty() && !s.readyQueue.isEmpty()) {
                    Job j = s.readyQueue.pop();
                    s.cpuQueue.push(j);
                }
                if (job.currentTime == job.runningTime && !s.cpuQueue.isEmpty()) {
                    Job completeJob = s.cpuQueue.pop();
                    s.availableMemory += completeJob.memoryRequired;
                    s.availableDevices += completeJob.devicesHeld;
                    completeJob.finishTime = s.time;
                    s.completeQueue.push(completeJob);
                }
            }
            if (s.time == timeBreak) {
                break;
            }
        }
        if(firstJob){
            s.time -= 1;
            firstJob = false;
        }

        // Let's see what command it is!
        switch (command.type) {
            case CommandType::SYSTEM:
                // Handled earlier
                break;
            case CommandType::NEW_JOB: {
                auto info = std::get<CommandNewJobInfo>(command.info);

                Job j{info.jobID, info.priority, command.time, info.executionTimeLength, info.memoryRequired, info.devicesRequired, 0, 0, 0, -1};
                if (j.memoryRequired > s.totalMemory || j.devicesRequired > s.totalDevices) {
                    break; // too much needed
                }

                if (j.priority == 1) {
                    // put in SJF
                    s.hq1.push(j);
                } else {
                    // put in FIFO
                    s.hq2.push(j);
                }
                break;
            }
            case CommandType::DEVICE_REQUEST: {
                auto info = std::get<CommandDeviceRequestInfo>(command.info);

                Job process = s.cpuQueue.peek();
                if (!s.cpuQueue.isEmpty()) {
                    process = s.cpuQueue.pop();
                }
                bool safe = isSystemSafeAfterDeviceRequest(s, info);
                if (safe) {
                    process.quantumLeft = 0;
                    process.devicesHeld += info.devicesRequested;
                    s.availableDevices -= info.devicesRequested;
                    s.readyQueue.push(process);
                }
                else {
                    process.devicesHeld = info.devicesRequested;
                    s.waitQueue.push(process);
                }
                break;
            }
            case CommandType::DEVICE_RELEASE: {
                auto info = std::get<CommandDeviceReleaseInfo>(command.info);
                if (!s.cpuQueue.isEmpty()) {
                    Job process = s.cpuQueue.pop();
                    process.devicesHeld -= info.devicesReleased;
                    s.availableDevices += info.devicesReleased;
                    s.readyQueue.push(process);
                }
                break;
            }
            case CommandType::DISPLAY: {
                std::cout << s.prettyDisplay(command.time) << std::endl;
                break;
            }
            default:
                std::cout << "Broken line: \"" << line << '\"' << std::endl;
                return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
