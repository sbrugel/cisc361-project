#include <algorithm>
#include <cstdlib>
#include <memory>

#include "Bankers.h"
#include "Commands.h"
#include "Input.h"
#include "String.h"
#include "System.h"

// Anonymous namespace = TU-local methods
// This is a collection of miscellaneous helper functions used in the main function
namespace {

/**
 * Prints a help message. Only appears if the file is ran with the -h (or invalid) argument
 */
void printHelp() {
    std::cout << "--- CISC361 Term Project ---\n"
                 "Options:\n"
                 "-h : Prints this help message\n"
                 "-f <file> : Runs the given input file\n"
                 "-f <number> : Runs the given input file based on its number\n"
                 "-i : Interactive mode\n" << std::endl;
}

/**
 * Get the value passed after a command-line option
 * @param argc Number of arguments
 * @param argv List of arguments
 * @param option The argument before the passed value
 * @param default_ A value to return if the option is not present
 * @return The option value if present, else default_
 */
[[nodiscard]] std::string_view getOptionValue(const int argc, const char* const argv[], std::string_view option, std::string_view default_ = "") {
    auto end = argv + argc;
    auto it = std::find(argv, end, option);
    if (it != end && ++it != end) {
        return *it;
    }
    return default_;
}

/**
 * Does an option exist on the command line?
 * @param argc Number of arguments
 * @param argv List of arguments
 * @param option The argument to check for
 * @return True if the argument/option exists
 */
[[nodiscard]] bool optionExists(const int argc, const char* const argv[], std::string_view option) {
    return std::find(argv, argv + argc, option) != (argv + argc);
}

/**
 * Figure out what input type to use to read input with
 * @param argc Number of arguments
 * @param argv List of arguments
 * @return The object to read input from
 */
[[nodiscard]] std::unique_ptr<IInput> getInput(const int argc, const char* const argv[]) {
    if (optionExists(argc, argv, "-h")) {
        // Print help and exit
        printHelp();
        exit(EXIT_SUCCESS);
    } else if (optionExists(argc, argv, "-i")) {
        // Interactive mode
        return std::make_unique<TerminalInput>();
    } else if (optionExists(argc, argv, "-f")) {
        // File mode
        auto value = getOptionValue(argc, argv, "-f");
        if (value == "0" || value == "1" || value == "2") {
            // Read a prepackaged file
            return std::make_unique<FileInput>(string::format("../inputs/i%d.txt", std::stoi(value.data())));
        }
        return std::make_unique<FileInput>(value);
    }
    return nullptr;
}

/**
 * Checks if the system will be in a safe state after a device request is granted. Basically runs Banker's algorithm for the request.
 * @param s The system that will be checked
 * @param info The device request info (i.e. how many devices are being requested)
 * @return True if the system is safe, and the request can be granted.
 */
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

/**
 * Checks if the ready queue is safe if a job is added to the wait queue
 * @param s The system
 * @param j The job to be added
 * @return True if safe
 */
[[nodiscard]] bool isSystemSafeAfterAddingJob(const System& s, Job j) {
    int freeDevices = s.totalDevices - s.cpuQueue.getTotalDevicesHeld() - s.readyQueue.getTotalDevicesHeld() - j.devicesHeld;

    if (freeDevices < 0) {
        return false;
    }

    std::vector<int> max;
    std::vector<int> available;

    for (Job job : s.readyQueue) {
        available.push_back(job.devicesRequired);
        max.push_back(job.devicesRequired);
    }
    available.push_back(0);
    max.push_back(j.devicesRequired);

    return reducedBankers(max, available, freeDevices);
}

} // namespace

int main(const int argc, const char* const argv[]) {
    System s{}; // initializes a System with all default fields

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

            // assign command parameters to the System
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
            if (!s.waitQueue.isEmpty()){ // there are jobs in the wait queue - look here first
                auto wQCopy = s.waitQueue;
                for (auto job : wQCopy){ // loop through all jobs in the wait queue
                    if (isSystemSafeAfterAddingJob(s, job)) { // can we add this wait queue job to the ready queue?
                        s.readyQueue.push(job);
                        s.availableDevices -= job.devicesHeld;
                        s.waitQueue.remove(job.id);
                    } else {
                        s.readyQueue.remove(job.id);
                    }
                }
            }
            if (!s.hq1.isEmpty() || !s.hq2.isEmpty()) { // there are jobs in the hold queue(s)
                if (!s.hq1.isEmpty()) { // SJF
                    Job pushFromHq1 = s.hq1.pop(); // get the first job from here (should be the shortest job)
                    if (s.availableMemory >= pushFromHq1.memoryRequired) { // enough memory to run, just put it in RQ
                        s.readyQueue.push(pushFromHq1);
                        s.availableMemory -= pushFromHq1.memoryRequired;
                    } else { // if not, move it to the back of the line
                        s.hq1.push(pushFromHq1);
                    }
                } else { // FIFO. runs in the same manner
                    Job pushFromHq2 = s.hq2.pop();
                    if (s.availableMemory >= pushFromHq2.memoryRequired) {
                        s.readyQueue.push(pushFromHq2);
                        s.availableMemory -= pushFromHq2.memoryRequired;
                    } else {
                        s.hq2.push(pushFromHq2);
                    }
                }
            }
            if (s.cpuQueue.isEmpty() && !s.readyQueue.isEmpty()) { // there are jobs in the RQ, and an open slot for a job to run
                Job job = s.readyQueue.pop();
                s.cpuQueue.push(job);
            }
            if (!s.cpuQueue.isEmpty()) { // there is something currently running
                Job job = s.cpuQueue.peek();
                if (job.currentTime < job.runningTime && job.quantumLeft < s.quantum) { // we can still run the job now
                    job = s.cpuQueue.pop();
                    job.currentTime += 1;
                    job.quantumLeft += 1;
                    s.cpuQueue.push(job);
                }
                if (job.quantumLeft == s.quantum && !s.cpuQueue.isEmpty() && job.currentTime != job.runningTime) { // quantum has run out, but we still need to run the job
                    Job putBack = s.cpuQueue.pop();
                    putBack.quantumLeft = 0;
                    putBack.currentTime = job.currentTime;
                    s.readyQueue.push(putBack);
                }
                if (s.cpuQueue.isEmpty() && !s.readyQueue.isEmpty()) { // there are jobs in the RQ, and an open slot for a job to run
                    Job j = s.readyQueue.pop();
                    s.cpuQueue.push(j);
                }
                if (job.currentTime == job.runningTime && !s.cpuQueue.isEmpty()) { // this job has just finished
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
                if (!s.cpuQueue.isEmpty()) { // this is the job requesting devices. It will only request if it is running
                    process = s.cpuQueue.pop();
                }
                bool safe = isSystemSafeAfterDeviceRequest(s, info); // banker's
                if (safe) {
                    process.quantumLeft = 0; // quantum instantly runs out, give it the devices and move it to ready
                    process.devicesHeld += info.devicesRequested;
                    s.availableDevices -= info.devicesRequested;
                    s.readyQueue.push(process);
                }
                else { // cannot grant devices just yet, it needs to wait
                    process.devicesHeld = info.devicesRequested;
                    s.waitQueue.push(process);
                }
                break;
            }
            case CommandType::DEVICE_RELEASE: {
                auto info = std::get<CommandDeviceReleaseInfo>(command.info);
                if (!s.cpuQueue.isEmpty()) { // the job releasing devices will always be running
                    Job process = s.cpuQueue.pop();
                    process.devicesHeld -= info.devicesReleased;
                    s.availableDevices += info.devicesReleased;
                    s.readyQueue.push(process); // move it back to ready
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
