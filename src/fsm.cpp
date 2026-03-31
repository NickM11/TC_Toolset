#include "constrained/fsm.h"

std::string applyBitToState(const std::string& currentState,
    char inputBit,
    int maxRunLength) {
    if (currentState.back() == inputBit) {
        if (static_cast<int>(currentState.size()) >= maxRunLength) {
            return "";
        }
        return currentState + inputBit;
    }

    return std::string(1, inputBit);
}

std::string applyBlockFromState(const std::string& startState,
    const std::string& block,
    int maxRunLength) {
    std::string currentState = startState;

    for (char bit : block) {
        currentState = applyBitToState(currentState, bit, maxRunLength);
        if (currentState.empty()) {
            return "-";
        }
    }

    return currentState;
}

std::string applyFirstBlock(const std::string& block,
    int maxRunLength) {
    if (block.empty()) {
        return "-";
    }

    std::string currentState(1, block[0]);

    for (std::size_t i = 1; i < block.size(); ++i) {
        currentState = applyBitToState(currentState, block[i], maxRunLength);
        if (currentState.empty()) {
            return "-";
        }
    }

    return currentState;
}

std::string applyPrefixFromState(const std::string& startState,
    const std::string& prefix,
    int maxRunLength) {
    std::string currentState = startState;

    for (char bit : prefix) {
        currentState = applyBitToState(currentState, bit, maxRunLength);
        if (currentState.empty()) {
            return "-";
        }
    }

    return currentState;
}

bool isValidWord(const std::string& word,
    int maxRunLength) {
    if (word.empty()) {
        return true;
    }

    int runLength = 1;

    for (std::size_t i = 1; i < word.size(); ++i) {
        if (word[i] == word[i - 1]) {
            runLength++;
            if (runLength > maxRunLength) {
                return false;
            }
        }
        else {
            runLength = 1;
        }
    }

    return true;
}

std::vector<std::vector<std::string>> buildFWTable(
    const std::vector<std::string>& states,
    const std::vector<std::string>& prefixes,
    int maxRunLength) {
    std::vector<std::vector<std::string>> table(
        states.size(),
        std::vector<std::string>(prefixes.size())
    );

    for (std::size_t i = 0; i < states.size(); ++i) {
        for (std::size_t j = 0; j < prefixes.size(); ++j) {
            table[i][j] = applyPrefixFromState(states[i], prefixes[j], maxRunLength);
        }
    }

    return table;
}