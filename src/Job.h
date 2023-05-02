#pragma once

#include <iostream>

struct Job {
    int id;
    int priority;
    int arrivalTime;
    int runningTime;
    int memoryRequired;
    int devicesRequired;
    int currentTime;

    friend std::ostream& operator<<(std::ostream& os, const Job& j) { //tostring
        os << "This job has ID " << j.id << std::endl;
        return os;
    }
};
