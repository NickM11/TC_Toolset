#include "constrained/generator.h"
#include "constrained/fsm.h"

void generateBinaryWords(int length,
    const std::string& current,
    std::vector<std::string>& result) {
    if (static_cast<int>(current.size()) == length) {
        result.push_back(current);
        return;
    }

    generateBinaryWords(length, current + "0", result);
    generateBinaryWords(length, current + "1", result);
}

std::string toBinary(int value, int width) {
    std::string result(width, '0');

    for (int i = width - 1; i >= 0; --i) {
        if (value & 1) {
            result[i] = '1';
        }
        value >>= 1;
    }

    return result;
}

std::vector<std::string> generateAllValidWords(int n,
    int maxRunLength) {
    std::vector<std::string> allWords;
    std::vector<std::string> validWords;

    generateBinaryWords(n, "", allWords);

    for (const std::string& word : allWords) {
        if (isValidWord(word, maxRunLength)) {
            validWords.push_back(word);
        }
    }

    return validWords;
}