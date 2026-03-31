#ifndef CODEBOOK_H
#define CODEBOOK_H

#include <string>
#include <vector>
#include "types.h"

BlockSplit getBlockSplit(int n);

std::vector<CodewordRecord> generateCodewordRecords(int n,
    int maxRunLength);

bool codeExists(int n,
    int k,
    int maxRunLength);

std::vector<CodewordRecord> selectCodebook(int n,
    int k,
    int maxRunLength);

#endif