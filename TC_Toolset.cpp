#include "constrained/states.h"
#include "constrained/fsm.h"
#include "constrained/capacity.h"
#include "constrained/generator.h"
#include "constrained/codebook.h"
#include "constrained/io_utils.h"

#include <iostream>
#include <stdexcept>

int main() {
    int n = 0;
    int k = 0;
    int maxRunLength = 0;
    int divisionFactor = 0;

    // ----------------------------------------------------------------
    // Parameter input
    // ----------------------------------------------------------------
    std::cout << "=== Translation Code Construction Toolset ===\n\n";

    std::cout << "Enter codeword length         (n): ";
    std::cin >> n;

    std::cout << "Enter information block length (k): ";
    std::cin >> k;

    std::cout << "Enter max run-length constraint (r): ";
    std::cin >> maxRunLength;

    std::cout << "Enter division factor           (d): ";
    std::cin >> divisionFactor;

    std::cout << "\n";

    // ----------------------------------------------------------------
    // Basic validation
    // ----------------------------------------------------------------
    if (n < 2) {
        std::cerr << "Error: n must be at least 2.\n";
        return 1;
    }
    if (k < 1 || k >= n) {
        std::cerr << "Error: k must satisfy 1 <= k < n.\n";
        return 1;
    }
    if (maxRunLength < 1) {
        std::cerr << "Error: max run-length must be at least 1.\n";
        return 1;
    }
    if (divisionFactor < 2 || divisionFactor > n) {
        std::cerr << "Error: division factor d must satisfy 2 <= d <= n.\n";
        return 1;
    }

    // ----------------------------------------------------------------
    // Step 1: Build state space
    // ----------------------------------------------------------------
    std::cout << "Building FSM state space...\n";
    std::vector<std::string> states = buildStates(maxRunLength);
    std::cout << "  States: " << states.size() << "\n";

    // ----------------------------------------------------------------
    // Step 2: Compute channel capacity
    // ----------------------------------------------------------------
    std::cout << "Computing channel capacity...\n";
    CapacityResult capacityResult = computeCapacity(states, maxRunLength);
    std::cout << "  Lambda max : " << capacityResult.lambda << "\n";
    std::cout << "  Capacity C : " << capacityResult.capacity << " bits/symbol\n";

    // Check that the requested code rate does not exceed capacity
    double codeRate = static_cast<double>(k) / static_cast<double>(n);
    if (codeRate > capacityResult.capacity) {
        std::cerr << "\nWarning: requested code rate R = " << codeRate
            << " exceeds channel capacity C = " << capacityResult.capacity
            << ".\nA valid codebook of this size may not exist.\n\n";
    }

    // ----------------------------------------------------------------
    // Step 3: Get block split
    // ----------------------------------------------------------------
    BlockSplit split;
    try {
        split = getBlockSplit(n, divisionFactor);
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error in block split: " << e.what() << "\n";
        return 1;
    }
    std::cout << "Block split (d=" << divisionFactor << "): "
        << split.firstLength << " + " << split.secondLength << " = " << n << "\n";

    // ----------------------------------------------------------------
    // Step 4: Generate codeword records
    // ----------------------------------------------------------------
    std::cout << "Generating codeword records...\n";
    std::vector<CodewordRecord> records;
    try {
        records = generateCodewordRecords(n, maxRunLength, divisionFactor);
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    std::cout << "  Valid codewords found: " << records.size() << "\n";

    int requiredCodewords = 1 << k;
    if (static_cast<int>(records.size()) < requiredCodewords) {
        std::cerr << "Error: not enough valid codewords found ("
            << records.size() << ") to build a codebook of size 2^k = "
            << requiredCodewords << ".\n";
        return 1;
    }

    // ----------------------------------------------------------------
    // Step 5: Select codebook
    // ----------------------------------------------------------------
    std::cout << "Selecting codebook (2^k = " << requiredCodewords << " codewords)...\n";
    std::vector<CodewordRecord> codebook = selectCodebook(n, k, maxRunLength, divisionFactor);
    std::cout << "  Codebook selected.\n";

    // ----------------------------------------------------------------
    // Step 6: Create output folder and save results
    // ----------------------------------------------------------------
    std::string folderName = makeRunFolderName(n, k, maxRunLength, divisionFactor);
    std::cout << "Creating output folder: " << folderName << "\n";

    if (!createRunFolder(folderName)) {
        std::cerr << "Error: could not create output folder.\n";
        return 1;
    }

    std::string base = folderName + "/";

    // Build adjacency matrix for CSV export
    std::vector<std::vector<double>> adjMatrix = buildAdjacencyMatrix(states, maxRunLength);

    saveFSMTableCSV(states, maxRunLength, base + "fsm_table.csv");
    saveAdjacencyMatrixCSV(adjMatrix, states, base + "adjacency_matrix.csv");
    saveCapacityResultsCSV(capacityResult, base + "capacity.csv");
    saveCodewordRecordsCSV(records, base + "codeword_records.csv");
    saveSelectedCodebookCSV(codebook, k, base + "codebook.csv");
    saveSummaryCSV(
        n, k, maxRunLength, divisionFactor,
        split.firstLength, split.secondLength,
        requiredCodewords,
        static_cast<int>(records.size()),
        capacityResult,
        base + "summary.csv"
    );

    // ----------------------------------------------------------------
    // Done
    // ----------------------------------------------------------------
    std::cout << "\n=== Run complete ===\n";
    std::cout << "  n              : " << n << "\n";
    std::cout << "  k              : " << k << "\n";
    std::cout << "  Max run-length : " << maxRunLength << "\n";
    std::cout << "  Division factor: " << divisionFactor << "\n";
    std::cout << "  Code rate R    : " << codeRate << "\n";
    std::cout << "  Capacity C     : " << capacityResult.capacity << "\n";
    std::cout << "  Output folder  : " << folderName << "\n";

    return 0;
}