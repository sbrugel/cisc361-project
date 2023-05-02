//
// Created by Simon on 5/2/2023.
//

#include "HoldQueueFIFO.h"

void HoldQueueFIFO::enqueue(Job j) {
    queue.push_back(j);
}

void HoldQueueFIFO::printQueue() {
    std::list<Job>::iterator it;
    std::cout << "Printing contents of FIFO" << std::endl;
    for (it = queue.begin(); it != queue.end(); ++it) {
        std::cout << *it;
    }
}

// todo: implement sort