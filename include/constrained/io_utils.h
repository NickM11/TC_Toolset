#ifndef IO_UTILS_H
#define IO_UTILS_H

#include <string>
#include <vector>
#include "constrained/types.h"

std::string makeRunFolderName(int n, int k, int maxRunLength);

bool createRunFolder(const std::string& folderName);

void saveFSMTableCSV(const std::vector<std::string>& states,
    int maxRunLength,
    const std::string& filepath);

void saveAdjacencyMatrixCSV(const std::vector<std::vector<double>>& A,
    const std::vector<std::string>& states,
    const std::string& filepath);

void saveCapacityResultsCSV(const CapacityResult& result,
    const std::string& filepath);

void saveCodewordRecordsCSV(const std::vector<CodewordRecord>& records,
    const std::string& filepath);

void saveSelectedCodebookCSV(const std::vector<CodewordRecord>& records,
    int k,
    const std::string& filepath);

void saveSummaryCSV(int n,
    int k,
    int maxRunLength,
    int firstBlockLength,
    int secondBlockLength,
    int requiredCodewords,
    int foundCodewords,
    const CapacityResult& capacityResult,
    const std::string& filepath);

#endif