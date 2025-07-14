//
// Created by Andrew on 2025-07-01.
//

#include "SimData.h"

#include <cmath>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ranges>

SimData::SimData(const std::string& filename) {
    time = 0.0;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << std::filesystem::current_path().string() << std::endl;
        std::cerr << "Error opening file " << filename << std::endl;
        return;
    }

    std::vector<std::vector<std::string>> data;
    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        data.push_back(row);
    }

    for (const auto& row : data | std::views::drop(1)) {
        int i = 0;

        for (const auto& cell : row) {
            if (std::ranges::find(posCols, data[0][i]) != std::end(posCols)) {
                xyzh.push_back(std::stof(cell));
            }
            if (std::ranges::find(velCols, data[0][i]) != std::end(velCols)) {
                vxyzv.push_back(std::stof(cell));
            }
            if (std::ranges::find(varCols, data[0][i]) != std::end(varCols)) {
                fxyz.push_back(std::stof(cell));
            }

            i++;
        }
    }
}


std::vector<int> SimData::getNeighbours(int part, Kernel kernel) {
    // Naive strategy for neighbour finding, will replace with kd-tree
    float tarX = xyzh[4 * part], tarY = xyzh[4 * part+1], tarZ = xyzh[4 * part+2], tarH = xyzh[4 * part+3];
    std::vector<int> neighbours;

    for (int i = 0; i < std::floor(xyzh.size() / 4); i++) {
        if (i == part) {
            continue;
        }

        float x = xyzh[4 * i], y = xyzh[4 * i + 1], z = xyzh[4 * i + 2];

        float dist = std::sqrt((tarX - x) * (tarX - x) + (tarY - y) * (tarY - y) + (tarZ - z) * (tarZ - z));
        if (kernel.valueAt(dist / tarH) > 0) {
            neighbours.push_back(i);
        }
    }

    return neighbours;
}


