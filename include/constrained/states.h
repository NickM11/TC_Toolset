#ifndef STATES_H
#define STATES_H

#include <string>
#include <vector>

std::vector<std::string> buildStates(int maxRunLength);

int findStateIndex(const std::vector<std::string>& states,
    const std::string& state);

#endif