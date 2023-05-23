#include "Job.h"

#include "String.h"

/**
 * A toString function for the Job. Used only for debugging.
 * @return All properties of the Job.
 */
Job::operator std::string() const {
    return string::format("Job{ id=%d, priority=%d, arrivalTime=%d, runningTime=%d, memoryRequired=%d, devicesRequired=%d, currentTime=%d, quantumLeft=%d, finishTime=%d, devicesHeld=%d}", this->id, this->priority, this->arrivalTime, this->runningTime, this->memoryRequired, this->devicesRequired, this->currentTime, this->quantumLeft, this->finishTime, this->devicesHeld);
}
