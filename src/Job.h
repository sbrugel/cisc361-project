#pragma once

struct Job {
    int arrivalTime;
    int requiredMemory;
    int maximumDevices;
    int runningTime;
    int priority; // can be 1 or 2
};