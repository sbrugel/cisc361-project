#include "Job.h"

#include "utility/String.h"

Job::operator std::string() const {
    return string::format("Job{ id=%d, priority=%d, arrivalTime=%d, runningTime=%d, memoryRequired=%d, devicesRequired=%d, currentTime=%d, quantumLeft=%d, finishTime=%d}", this->id, this->priority, this->arrivalTime, this->runningTime, this->memoryRequired, this->devicesRequired, this->currentTime, this->quantumLeft, this->finishTime);
}
