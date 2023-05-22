#include "System.h"

#include "utility/String.h"

std::string System::prettyDisplay(int currentTime) const {
    std::string out;

    out += string::format("At time %d:\n", currentTime);
    out += string::format("Currently available main memory = %d:\n", this->availableMemory);
    out += string::format("Currently available devices = %d:\n", this->availableDevices);

    out += std::string{this->completeQueue} + '\n';
    out += std::string{this->hq1} + '\n';
    out += std::string{this->hq2} + '\n';
    out += std::string{this->readyQueue} + '\n';

    out += std::string{this->cpuQueue} + "\n";
    out += std::string{this->waitQueue} + '\n';

    // Only print turnaround at the very end of a simulation
    if (currentTime == 9999) {
        out += string::format("System turnaround time: %.2f", static_cast<float>(this->completeQueue.getTurnarounds()) / static_cast<float>(this->completeQueue.getNumJobs()));
    }

    return out;
}
