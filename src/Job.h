#pragma once

#include <string>

struct Job {
    int id;
    int priority;
    int arrivalTime;
    int runningTime;
    int memoryRequired;
    int devicesRequired;
    int currentTime;

    explicit operator std::string() const;
};
