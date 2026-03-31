#ifndef FSM_H
#define FSM_H

#include <string>
#include <vector>

std::string applyBitToState(const std::string& currentState,
    char inputBit,
    int maxRunLength);

std::string applyBlockFromState(const std::string& startState,
    const std::string& block,
    int maxRunLength);

std::string applyFirstBlock(const std::string& block,
    int maxRunLength);

std::string applyPrefixFromState(const std::string& startState,
    const std::string& prefix,
    int maxRunLength);

bool isValidWord(const std::string& word,
    int maxRunLength);

std::vector<std::vector<std::string>> buildFWTable(
    const std::vector<std::string>& states,
    const std::vector<std::string>& prefixes,
    int maxRunLength);

#endif