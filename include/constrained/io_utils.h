#ifndef IO_UTILS_H
#define IO_UTILS_H

#include <string>
#include <vector>
#include "constrained/types.h"

// Folder name now includes division factor d for experiment traceability.
// Example: run_n20_k4_r4_d2_20250501_143022
std::string makeRunFolderName(int n, int k, int maxRunLength, int divisionFactor);

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

// Summary now includes divisionFactor and codeRate for easier batch analysis.
void saveSummaryCSV(int n,
    int k,
    int maxRunLength,
    int divisionFactor,
    int firstBlockLength,
    int secondBlockLength,
    int requiredCodewords,
    int foundCodewords,
    const CapacityResult& capacityResult,
    const std::string& filepath);

#endif