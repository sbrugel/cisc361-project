#pragma once

#include <string>

struct Job {
    int id;
    int priority;
    int arrivalTime;
    int runningTime;
    int memoryRequired;
    int devicesRequired;
    int devicesHeld;
    int currentTime;
    int quantumLeft;
    int finishTime;

    explicit operator std::string() const;
};
