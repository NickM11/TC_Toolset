#ifndef GENERATOR_H
#define GENERATOR_H

#include <string>
#include <vector>

void generateBinaryWords(int length,
    const std::string& current,
    std::vector<std::string>& result);

std::string toBinary(int value, int width);

std::vector<std::string> generateAllValidWords(int n,
    int maxRunLength);

#endif