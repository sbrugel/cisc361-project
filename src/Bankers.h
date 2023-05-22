#pragma once

#include <vector>

/**
 * Computes a simplified version of the Banker's Algorithm for one type of shared resource.
 * @param max The amount of the resource each process needs
 * @param allocated The amount of the resource each process already has
 * @param available The amount of the resource that is not dedicated to a particular process
 * @return If the system is in a safe state in the current configuration
 */
[[nodiscard]] bool reducedBankers(const std::vector<int>& max, const std::vector<int>& allocated, int available);
