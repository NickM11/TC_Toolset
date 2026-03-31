#include <iostream>
#include <string>
#include <vector>

#include "constrained/types.h"
#include "constrained/states.h"
#include "constrained/fsm.h"
#include "constrained/generator.h"
#include "constrained/capacity.h"
#include "constrained/codebook.h"
#include "constrained/io_utils.h"

int main() {
    int n = 0;
    int k = 0;
    int maxRunLength = 0;

    std::cout << "Enter n (codeword length): ";
    std::cin >> n;

    std::cout << "Enter k (message length): ";
    std::cin >> k;

    std::cout << "Enter maximum run length constraint: ";
    std::cin >> maxRunLength;

    if (n <= 0 || k < 0 || maxRunLength <= 0) {
        std::cerr << "Invalid parameters.\n";
        return 1;
    }

    BlockSplit split = getBlockSplit(n);
    int requiredCodewords = 1 << k;

    std::string runFolder = makeRunFolderName(n, k, maxRunLength);

    if (!createRunFolder(runFolder)) {
        std::cerr << "Could not create output folder: " << runFolder << "\n";
        return 1;
    }

    std::vector<std::string> states = buildStates(maxRunLength);
    CapacityResult capacityResult = computeCapacity(states, maxRunLength);

    std::vector<CodewordRecord> allRecords = generateCodewordRecords(n, maxRunLength);
    std::vector<CodewordRecord> selectedCodebook = selectCodebook(n, k, maxRunLength);

    std::cout << "\nParameters\n";
    std::cout << "=====================================================\n";
    std::cout << "n = " << n << "\n";
    std::cout << "k = " << k << "\n";
    std::cout << "maxRunLength = " << maxRunLength << "\n";
    std::cout << "firstBlockLength = " << split.firstLength << "\n";
    std::cout << "secondBlockLength = " << split.secondLength << "\n";
    std::cout << "requiredCodewords = " << requiredCodewords << "\n";

    std::cout << "\nResults\n";
    std::cout << "=====================================================\n";
    std::cout << "Found valid codewords = " << allRecords.size() << "\n";
    std::cout << "Lambda = " << capacityResult.lambda << "\n";
    std::cout << "Capacity = " << capacityResult.capacity << " bits/symbol\n";
    std::cout << "Code (" << n << "," << k << ") "
        << (codeExists(n, k, maxRunLength) ? "exists" : "does NOT exist")
        << "\n";

    std::vector<std::vector<double>> adjacencyMatrix =
        buildAdjacencyMatrix(states, maxRunLength);

    saveFSMTableCSV(states,
        maxRunLength,
        runFolder + "/fsm_transitions.csv");

    saveAdjacencyMatrixCSV(adjacencyMatrix,
        states,
        runFolder + "/fsm_adjacency_matrix.csv");

    saveCapacityResultsCSV(capacityResult,
        runFolder + "/capacity_results.csv");

    saveCodewordRecordsCSV(allRecords,
        runFolder + "/all_valid_codewords.csv");

    saveSelectedCodebookCSV(selectedCodebook,
        k,
        runFolder + "/selected_codebook.csv");

    saveSummaryCSV(n,
        k,
        maxRunLength,
        split.firstLength,
        split.secondLength,
        requiredCodewords,
        static_cast<int>(allRecords.size()),
        capacityResult,
        runFolder + "/summary.csv");

    std::cout << "\nSaved results in folder: " << runFolder << "\n";

    return 0;
}