#include "constrained/states.h"

std::vector<std::string> buildStates(int maxRunLength) {
    std::vector<std::string> states;

    for (int len = maxRunLength; len >= 1; --len) {
        states.push_back(std::string(len, '0'));
    }

    for (int len = 1; len <= maxRunLength; ++len) {
        states.push_back(std::string(len, '1'));
    }

    return states;
}

int findStateIndex(const std::vector<std::string>& states,
    const std::string& state) {
    for (std::size_t i = 0; i < states.size(); ++i) {
        if (states[i] == state) {
            return static_cast<int>(i);
        }
    }
    return -1;
}