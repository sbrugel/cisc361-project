#include "Commands.h"

#include "utility/String.h"

CommandType charToCommandType(char c) {
    switch (c) {
        case 'C':
            return CommandType::SYSTEM;
        case 'A':
            return CommandType::NEW_JOB;
        case 'Q':
            return CommandType::DEVICE_REQUEST;
        case 'L':
            return CommandType::DEVICE_RELEASE;
        case 'D':
            return CommandType::DISPLAY;
        default:
            return CommandType::INVALID;
    }
}

CommandSystemInfo::operator std::string() const {
    return string::format("System{ startTime=%d, memoryAmount=%d, deviceAmount=%d, quantum=%d }", this->startTime, this->memoryAmount, this->deviceAmount, this->quantum);
}

CommandNewJobInfo::operator std::string() const {
    return string::format("NewJob{ arrivalTime=%d, jobID=%d, memoryRequired=%d, devicesRequired=%d, executionTimeLength=%d, priority=%d }", this->arrivalTime, this->jobID, this->memoryRequired, this->devicesRequired, this->executionTimeLength, this->priority);
}

CommandDeviceRequestInfo::operator std::string() const {
    return string::format("DeviceRequest{ arrivalTime=%d, jobID=%d, devicesRequested=%d }", this->arrivalTime, this->jobID, this->devicesRequested);
}

CommandDeviceReleaseInfo::operator std::string() const {
    return string::format("DeviceRelease{ arrivalTime=%d, jobID=%d, devicesReleased=%d }", this->arrivalTime, this->jobID, this->devicesReleased);
}

CommandDisplayInfo::operator std::string() const {
    return string::format("Display{ arrivalTime=%d }", this->arrivalTime);
}

CommandInvalid::operator std::string() const {
    return "Invalid command or command arguments!";
}

CommandInfo::operator std::string() const {
    switch (this->type) {
        case CommandType::SYSTEM:
            return std::string{std::get<CommandSystemInfo>(this->info)};
        case CommandType::NEW_JOB:
            return std::string{std::get<CommandNewJobInfo>(this->info)};
        case CommandType::DEVICE_REQUEST:
            return std::string{std::get<CommandDeviceRequestInfo>(this->info)};
        case CommandType::DEVICE_RELEASE:
            return std::string{std::get<CommandDeviceReleaseInfo>(this->info)};
        case CommandType::DISPLAY:
            return std::string{std::get<CommandDisplayInfo>(this->info)};
        default:
            return std::string{std::get<CommandInvalid>(this->info)};
    }
}

namespace {

CommandSystemInfo parseSystemCommand(std::string_view line);
CommandNewJobInfo parseNewJobCommand(std::string_view line);
CommandDeviceRequestInfo parseDeviceRequestCommand(std::string_view line);
CommandDeviceReleaseInfo parseDeviceReleaseCommand(std::string_view line);
CommandDisplayInfo parseDisplayCommand(std::string_view line);

} // namespace

CommandInfo parseCommand(std::string_view line) {
    switch (charToCommandType(line[0])) {
        case CommandType::SYSTEM:
            return {CommandType::SYSTEM, parseSystemCommand(line)};
        case CommandType::NEW_JOB:
            return {CommandType::NEW_JOB, parseNewJobCommand(line)};
        case CommandType::DEVICE_REQUEST:
            return {CommandType::DEVICE_REQUEST, parseDeviceRequestCommand(line)};
        case CommandType::DEVICE_RELEASE:
            return {CommandType::DEVICE_RELEASE, parseDeviceReleaseCommand(line)};
        case CommandType::DISPLAY:
            return {CommandType::DISPLAY, parseDisplayCommand(line)};
        case CommandType::INVALID:
            return {};
    }
    return {};
}

namespace {

CommandSystemInfo parseSystemCommand(std::string_view line) {
    auto strings = string::split(line.data(), ' ');

    CommandSystemInfo info{};
    info.startTime = std::stoi(strings[1]);

    if (strings.size() < 2)
        return info;

    // Iterate over line starting with arguments
    for (auto it = std::begin(strings) + 2; it != std::end(strings); ++it) {
        auto value = std::stoi(it->substr(2));
        switch (it->at(0)) {
            case 'M':
                info.memoryAmount = value;
                break;
            case 'S':
                info.deviceAmount = value;
                break;
            case 'Q':
                info.quantum = value;
                break;
        }
    }

    return info;
}

CommandNewJobInfo parseNewJobCommand(std::string_view line) {
    auto strings = string::split(line.data(), ' ');

    CommandNewJobInfo info{};
    info.arrivalTime = std::stoi(strings[1]);

    if (strings.size() < 2)
        return info;

    // Iterate over line starting with arguments
    for (auto it = std::begin(strings) + 2; it != std::end(strings); ++it) {
        auto value = std::stoi(it->substr(2));
        switch (it->at(0)) {
            case 'J':
                info.jobID = value;
                break;
            case 'M':
                info.memoryRequired = value;
                break;
            case 'S':
                info.devicesRequired = value;
                break;
            case 'R':
                info.executionTimeLength = value;
                break;
            case 'P':
                info.priority = value;
                break;
        }
    }

    return info;
}

CommandDeviceRequestInfo parseDeviceRequestCommand(std::string_view line) {
    auto strings = string::split(line.data(), ' ');

    CommandDeviceRequestInfo info{};
    info.arrivalTime = std::stoi(strings[1]);

    if (strings.size() < 2)
        return info;

    // Iterate over line starting with arguments
    for (auto it = std::begin(strings) + 2; it != std::end(strings); ++it) {
        auto value = std::stoi(it->substr(2));
        switch (it->at(0)) {
            case 'J':
                info.jobID = value;
                break;
            case 'D':
                info.devicesRequested = value;
                break;
        }
    }

    return info;
}

CommandDeviceReleaseInfo parseDeviceReleaseCommand(std::string_view line) {
    auto strings = string::split(line.data(), ' ');

    CommandDeviceReleaseInfo info{};
    info.arrivalTime = std::stoi(strings[1]);

    if (strings.size() < 2)
        return info;

    // Iterate over line starting with arguments
    for (auto it = std::begin(strings) + 2; it != std::end(strings); ++it) {
        auto value = std::stoi(it->substr(2));
        switch (it->at(0)) {
            case 'J':
                info.jobID = value;
                break;
            case 'D':
                info.devicesReleased = value;
                break;
        }
    }

    return info;
}

CommandDisplayInfo parseDisplayCommand(std::string_view line) {
    auto strings = string::split(line.data(), ' ');

    CommandDisplayInfo info{};
    info.arrivalTime = std::stoi(strings[1]);

    // This one has no other arguments
    return info;
}

} // namespace
