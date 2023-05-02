//
// Created by Simon on 5/2/2023.
//

#include "HoldQueueSJF.h"

void HoldQueueSJF::enqueue(Job j) {
    queue.push_back(j);
}

void HoldQueueSJF::printQueue() {
    std::list<Job>::iterator it;
    for (it = queue.begin(); it != queue.end(); ++it) {
        std::cout << *it << std::endl;
    }
}
