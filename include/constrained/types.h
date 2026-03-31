#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>

struct CodewordRecord {
    std::string firstBlock;
    std::string secondBlock;
    std::string stateAfterFirst;
    std::string stateAfterSecond;
    std::string codeword;
};

struct CapacityResult {
    double lambda = 0.0;
    double capacity = 0.0;
};

struct BlockSplit {
    int firstLength = 0;
    int secondLength = 0;
};

#endif