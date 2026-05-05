#ifndef CODEBOOK_H
#define CODEBOOK_H

#include <string>
#include <vector>
#include "types.h"

// Returns the block split for a given codeword length n and division factor d.
// Standard Freiman-Wyner uses d = 2.
// For even n: firstLength = n/d, secondLength = n - n/d
// For odd n:  firstLength = (n+1)/d rounded up, secondLength = n - firstLength
BlockSplit getBlockSplit(int n, int d = 2);

std::vector<CodewordRecord> generateCodewordRecords(int n,
    int maxRunLength,
    int divisionFactor = 2);

bool codeExists(int n,
    int k,
    int maxRunLength,
    int divisionFactor = 2);

std::vector<CodewordRecord> selectCodebook(int n,
    int k,
    int maxRunLength,
    int divisionFactor = 2);

#endif