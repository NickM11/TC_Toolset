#ifndef CAPACITY_H
#define CAPACITY_H

#include <string>
#include <vector>
#include "types.h"

std::vector<std::vector<double>> buildAdjacencyMatrix(
    const std::vector<std::string>& states,
    int maxRunLength);

double largestEigenvalue(const std::vector<std::vector<double>>& A,
    int maxIterations = 1000,
    double tolerance = 1e-12);

CapacityResult computeCapacity(const std::vector<std::string>& states,
    int maxRunLength);

#endif