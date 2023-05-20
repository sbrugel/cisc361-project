#include <algorithm>
#include <cstdlib>
#include <memory>

#include "utility/String.h"
#include "Commands.h"
#include "Input.h"
#include "System.h"

namespace {

void printHelp() {
    std::cout << "--- CISC361 Term Project ---\n"
                 "Options:\n"
                 "-h : Prints this help message\n"
                 "-f <file> : Runs the given input file\n"
                 "-f <number> : Runs the given input file based on its number\n"
                 "-i : Interactive mode\n" << std::endl;
}

std::string_view getOptionValue(const int argc, const char* const argv[], std::string_view option, std::string_view default_ = "") {
    auto end = argv + argc;
    auto it = std::find(argv, end, option);
    if (it != end && ++it != end) {
        return *it;
    }
    return default_;
}

bool optionExists(const int argc, const char* const argv[], std::string_view option) {
    return std::find(argv, argv + argc, option) != (argv + argc);
}

std::unique_ptr<IInput> getInput(const int argc, const char* const argv[]) {
    if (optionExists(argc, argv, "-h")) {
        printHelp();
        exit(EXIT_SUCCESS);
    } else if (optionExists(argc, argv, "-i")) {
        return std::make_unique<TerminalInput>();
    } else if (optionExists(argc, argv, "-f")) {
        auto value = getOptionValue(argc, argv, "-f");
        if (value == "0" || value == "1" || value == "2") {
            return std::make_unique<FileInput>(string::format("../inputs/i%s.txt", value));
        }
        return std::make_unique<FileInput>(value);
    }
    return nullptr;
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

            // Next line!
            continue;
        }

        // Update simulation to latest timestamp
        Job job{};
        while (s.time < command.time) {
            s.time += 1;
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
                        s.hq1.push(pushFromHq2);
                    }
                }
            }
            if (s.cpuQueue.isEmpty() && !s.readyQueue.isEmpty()) {
                job = s.readyQueue.pop();
                s.cpuQueue.push(job);
            }
            if (!s.cpuQueue.isEmpty()) {
                job = s.cpuQueue.peek();
                if (job.quantumLeft == s.quantum && !s.cpuQueue.isEmpty()) {
                    Job putBack = s.cpuQueue.pop();
                    putBack.quantumLeft = 0;
                    putBack.currentTime = job.currentTime;
                    s.readyQueue.push(putBack);
                }
                if (s.cpuQueue.isEmpty() && !s.readyQueue.isEmpty()) {
                    job = s.readyQueue.pop();
                    s.cpuQueue.push(job);
                }
                if (job.currentTime < job.runningTime && job.quantumLeft < s.quantum) {
                    job = s.cpuQueue.pop();
                    job.currentTime += 1;
                    job.quantumLeft += 1;
                    s.cpuQueue.push(job);
                }
                if (job.currentTime == job.runningTime && !s.cpuQueue.isEmpty()) {
                    Job completeJob = s.cpuQueue.pop();
                    s.availableMemory += completeJob.memoryRequired;
                    completeJob.finishTime = s.time;
                    s.completeQueue.push(completeJob);
                }
            }
        }

        // Let's see what command it is!
        switch (command.type) {
            case CommandType::SYSTEM:
                // Handled earlier
                break;
            case CommandType::NEW_JOB: {
                auto info = std::get<CommandNewJobInfo>(command.info);

                Job j{info.jobID, info.priority, command.time, info.executionTimeLength, info.memoryRequired, info.devicesRequired, 0, 0, -1};
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
                // todo: do stuff with device request command
                break;
            }
            case CommandType::DEVICE_RELEASE: {
                auto info = std::get<CommandDeviceReleaseInfo>(command.info);
                // todo: do stuff with device release command
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
