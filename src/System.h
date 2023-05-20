#pragma once

#include <string>

#include "JobQueue.h"

struct System {
    int time = -1; // the current time; gets incremented
    int totalMemory = -1;
    int availableMemory = -1;
    int totalDevices = -1;
    int availableDevices = -1;
    int quantum = -1;

    JobQueue hq1{JobQueueSortType::SJF, "Hold Queue 1"};
    JobQueue hq2{JobQueueSortType::FIFO, "Hold Queue 2"};

    JobQueue readyQueue{JobQueueSortType::RR, "Ready Queue"};
    JobQueue waitQueue{JobQueueSortType::RR, "Wait Queue"};
    JobQueue cpuQueue{JobQueueSortType::NONE, "CPU Queue"};

    JobQueue completeQueue{JobQueueSortType::COMPLETE, "Complete Queue"};

    [[nodiscard]] std::string prettyDisplay(int currentTime) const;
};
