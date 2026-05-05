#include "constrained/codebook.h"
#include "constrained/generator.h"
#include "constrained/fsm.h"

#include <algorithm>
#include <random>
#include <stdexcept>

BlockSplit getBlockSplit(int n, int d) {
    if (d < 2) {
        throw std::invalid_argument("Division factor d must be >= 2.");
    }
    if (d > n) {
        throw std::invalid_argument("Division factor d must not exceed n.");
    }

    BlockSplit split;

    // First block gets the ceiling of n/d to avoid zero-length blocks
    // when n is not evenly divisible by d.
    split.firstLength = (n + d - 1) / d;   // ceiling division
    split.secondLength = n - split.firstLength;

    // Guard: second block must be at least 1 bit long
    if (split.secondLength < 1) {
        throw std::invalid_argument(
            "Division factor d is too large: second block would be empty.");
    }

    return split;
}

std::vector<CodewordRecord> generateCodewordRecords(int n,
    int maxRunLength,
    int divisionFactor) {
    BlockSplit split = getBlockSplit(n, divisionFactor);

    std::vector<std::string> firstBlocks;
    std::vector<std::string> secondBlocks;
    std::vector<CodewordRecord> records;

    generateBinaryWords(split.firstLength, "", firstBlocks);
    generateBinaryWords(split.secondLength, "", secondBlocks);

    for (const std::string& b1 : firstBlocks) {
        std::string s1 = applyFirstBlock(b1, maxRunLength);
        if (s1 == "-") {
            continue;
        }

        for (const std::string& b2 : secondBlocks) {
            std::string s2 = applyBlockFromState(s1, b2, maxRunLength);
            if (s2 == "-") {
                continue;
            }

            std::string codeword = b1 + b2;

            if (static_cast<int>(codeword.size()) != n) {
                continue;
            }

            if (!isValidWord(codeword, maxRunLength)) {
                continue;
            }

            // Deduplication
            bool duplicate = false;
            for (const CodewordRecord& rec : records) {
                if (rec.codeword == codeword) {
                    duplicate = true;
                    break;
                }
            }

            if (!duplicate) {
                CodewordRecord rec;
                rec.firstBlock = b1;
                rec.secondBlock = b2;
                rec.stateAfterFirst = s1;
                rec.stateAfterSecond = s2;
                rec.codeword = codeword;
                records.push_back(rec);
            }
        }
    }

    return records;
}

bool codeExists(int n,
    int k,
    int maxRunLength,
    int divisionFactor) {
    int requiredCodewords = 1 << k;
    std::vector<CodewordRecord> records =
        generateCodewordRecords(n, maxRunLength, divisionFactor);
    return static_cast<int>(records.size()) >= requiredCodewords;
}

std::vector<CodewordRecord> selectCodebook(int n,
    int k,
    int maxRunLength,
    int divisionFactor) {
    int requiredCodewords = 1 << k;

    std::vector<CodewordRecord> records =
        generateCodewordRecords(n, maxRunLength, divisionFactor);

    std::random_device rd;
    std::mt19937 generator(rd());

    std::shuffle(records.begin(), records.end(), generator);

    if (static_cast<int>(records.size()) > requiredCodewords) {
        records.resize(requiredCodewords);
    }

    return records;
}