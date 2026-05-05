#include "constrained/io_utils.h"
#include "constrained/fsm.h"
#include "constrained/generator.h"

#include <ctime>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <direct.h>   // _mkdir

std::string makeRunFolderName(int n, int k, int maxRunLength, int divisionFactor) {
    std::time_t now = std::time(nullptr);

    std::tm localTime{};
    localtime_s(&localTime, &now);

    std::stringstream ss;
    ss << "run_n" << n
        << "_k" << k
        << "_r" << maxRunLength
        << "_d" << divisionFactor
        << "_"
        << (1900 + localTime.tm_year)
        << std::setw(2) << std::setfill('0') << (1 + localTime.tm_mon)
        << std::setw(2) << std::setfill('0') << localTime.tm_mday
        << "_"
        << std::setw(2) << std::setfill('0') << localTime.tm_hour
        << std::setw(2) << std::setfill('0') << localTime.tm_min
        << std::setw(2) << std::setfill('0') << localTime.tm_sec;

    return ss.str();
}

bool createRunFolder(const std::string& folderName) {
    return _mkdir(folderName.c_str()) == 0;
}

void saveFSMTableCSV(const std::vector<std::string>& states,
    int maxRunLength,
    const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        return;
    }

    file << "CurrentState,Input,NextState\n";

    for (const std::string& state : states) {
        for (char inputBit : {'0', '1'}) {
            std::string nextState = applyBitToState(state, inputBit, maxRunLength);

            file << state << ","
                << inputBit << ","
                << (nextState.empty() ? "FORBIDDEN" : nextState)
                << "\n";
        }
    }
}

void saveAdjacencyMatrixCSV(const std::vector<std::vector<double>>& A,
    const std::vector<std::string>& states,
    const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        return;
    }

    file << "State";
    for (const std::string& state : states) {
        file << "," << state;
    }
    file << "\n";

    for (std::size_t i = 0; i < A.size(); ++i) {
        file << states[i];
        for (std::size_t j = 0; j < A[i].size(); ++j) {
            file << "," << A[i][j];
        }
        file << "\n";
    }
}

void saveCapacityResultsCSV(const CapacityResult& result,
    const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        return;
    }

    file << "LargestEigenvalue,ChannelCapacityBitsPerSymbol\n";
    file << result.lambda << "," << result.capacity << "\n";
}

void saveCodewordRecordsCSV(const std::vector<CodewordRecord>& records,
    const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        return;
    }

    file << "Index,FirstBlock,StateAfterFirst,SecondBlock,StateAfterSecond,Codeword\n";

    for (std::size_t i = 0; i < records.size(); ++i) {
        file << i << ","
            << records[i].firstBlock << ","
            << records[i].stateAfterFirst << ","
            << records[i].secondBlock << ","
            << records[i].stateAfterSecond << ","
            << records[i].codeword << "\n";
    }
}

void saveSelectedCodebookCSV(const std::vector<CodewordRecord>& records,
    int k,
    const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        return;
    }

    file << "Index,Message,Codeword\n";

    for (std::size_t i = 0; i < records.size(); ++i) {
        file << i << ","
            << toBinary(static_cast<int>(i), k) << ","
            << records[i].codeword << "\n";
    }
}

void saveSummaryCSV(int n,
    int k,
    int maxRunLength,
    int divisionFactor,
    int firstBlockLength,
    int secondBlockLength,
    int requiredCodewords,
    int foundCodewords,
    const CapacityResult& capacityResult,
    const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        return;
    }

    // Compute achieved code rate R = k/n
    double codeRate = (n > 0) ? static_cast<double>(k) / static_cast<double>(n) : 0.0;

    // Compute rate efficiency as percentage of channel capacity used
    double rateEfficiency = (capacityResult.capacity > 0.0)
        ? (codeRate / capacityResult.capacity) * 100.0
        : 0.0;

    file << "n,k,maxRunLength,divisionFactor,firstBlockLength,secondBlockLength,"
        << "requiredCodewords,foundCodewords,codeRate,lambda,capacity,rateEfficiency\n";

    file << n << ","
        << k << ","
        << maxRunLength << ","
        << divisionFactor << ","
        << firstBlockLength << ","
        << secondBlockLength << ","
        << requiredCodewords << ","
        << foundCodewords << ","
        << codeRate << ","
        << capacityResult.lambda << ","
        << capacityResult.capacity << ","
        << rateEfficiency << "\n";
}