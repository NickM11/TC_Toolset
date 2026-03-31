#include "constrained/codebook.h"
#include "constrained/generator.h"
#include "constrained/fsm.h"

BlockSplit getBlockSplit(int n) {
    BlockSplit split;

    if (n % 2 == 0) {
        split.firstLength = n / 2;
        split.secondLength = n / 2;
    }
    else {
        split.firstLength = (n + 1) / 2;
        split.secondLength = (n - 1) / 2;
    }

    return split;
}

std::vector<CodewordRecord> generateCodewordRecords(int n,
    int maxRunLength) {
    BlockSplit split = getBlockSplit(n);

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
    int maxRunLength) {
    int requiredCodewords = 1 << k;
    std::vector<CodewordRecord> records = generateCodewordRecords(n, maxRunLength);
    return static_cast<int>(records.size()) >= requiredCodewords;
}

std::vector<CodewordRecord> selectCodebook(int n,
    int k,
    int maxRunLength) {
    int requiredCodewords = 1 << k;
    std::vector<CodewordRecord> records = generateCodewordRecords(n, maxRunLength);

    if (static_cast<int>(records.size()) > requiredCodewords) {
        records.resize(requiredCodewords);
    }

    return records;
}