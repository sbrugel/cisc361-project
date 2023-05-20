#include "Bankers.h"

bool reducedBankers(std::vector<int>& max, std::vector<int>& allocated, int available) {
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

// TEST CODE -- TO RUN REMOVE Main.cpp FROM PROJECT AND UNCOMMENT
/*
#include <iostream>
using namespace std;

int main() {
    int numProcesses;

    cout << "Enter the number of processes: ";
    cin >> numProcesses;

    int numResources = 0;

    vector<int> max(numProcesses);
    vector<int> allocated(numProcesses);
    vector<bool> finish(numProcesses, false);

    // Input the maximum resource requirement for each process
    cout << "Enter the maximum resource requirement for each process:\n";
    for (int i = 0; i < numProcesses; ++i) {
        cout << "Process P" << i << ":\n";
        cin >> max[i];
    }

    // Input the allocated resources for each process
    cout << "Enter the allocated resources for each process:\n";
    for (int i = 0; i < numProcesses; ++i) {
        cout << "Process P" << i << ":\n";
        cin >> allocated[i];
    }

    // Input the available resources
    cout << "Enter the number of available resources:\n";
    cin >> numResources;

    // Check if the system is in a safe state
    if (reducedBankers(max, allocated, numResources)) {
        cout << "The system is in a safe state.\n";
    } else {
        cout << "The system is in an unsafe state. Deadlock may occur.\n";
    }
}
*/
