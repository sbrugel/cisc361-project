#include "Bankers.h"

bool reducedBankers(const std::vector<int>& max, const std::vector<int>& allocated, int available) {
    auto numProcesses = static_cast<int>(max.size());
    int work = available;
    std::vector<int> need(numProcesses);
    std::vector<bool> finished(numProcesses, false);

    // Calculate the Need of each process
    for (int i = 0; i < numProcesses; ++i) {
        need[i] = max[i] - allocated[i];
    }

    // Check if the system is in a safe state
    std::vector<int> safeSequence;
    int count = 0;

    while (count < numProcesses) {
        bool found = false;

        for (int i = 0; i < numProcesses; ++i) {
            if (finished[i]) {
                continue;
            }

            if (need[i] > work) {
                break;
            }

            work += allocated[i];

            finished[i] = true;
            safeSequence.push_back(i);
            found = true;
            ++count;
        }

        if (!found) {
            break;
        }
    }

    // If all processes finished, the system is in a safe state
    return count == numProcesses;
}
