#pragma once

#include <string>
#include <string_view>
#include <variant>

enum class CommandType {
    SYSTEM,         // C
    NEW_JOB,        // A
    DEVICE_REQUEST, // Q
    DEVICE_RELEASE, // L
    DISPLAY,        // D
    INVALID,
};

CommandType charToCommandType(char c);

struct CommandSystemInfo {
    int startTime;
    int memoryAmount;
    int deviceAmount;
    int quantum;

    explicit operator std::string() const;
};

struct CommandNewJobInfo {
    int arrivalTime;
    int jobID;
    int memoryRequired;
    int devicesRequired;
    int executionTimeLength;
    int priority;

    explicit operator std::string() const;
};

struct CommandDeviceRequestInfo {
    int arrivalTime;
    int jobID;
    int devicesRequested;

    explicit operator std::string() const;
};

struct CommandDeviceReleaseInfo {
    int arrivalTime;
    int jobID;
    int devicesReleased;

    explicit operator std::string() const;
};

struct CommandDisplayInfo {
    int arrivalTime;

    explicit operator std::string() const;
};

struct CommandInvalid {
    explicit operator std::string() const;
};

struct CommandInfo {
    CommandType type = CommandType::INVALID;
    std::variant<
            CommandSystemInfo,
            CommandNewJobInfo,
            CommandDeviceRequestInfo,
            CommandDeviceReleaseInfo,
            CommandDisplayInfo,
            CommandInvalid
    > info;

    // For debugging
    explicit operator std::string() const;
};

CommandInfo parseCommand(std::string_view line);
