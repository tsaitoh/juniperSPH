//
// Created by Andrew on 2025-07-01.
//

#include "SimData.h"

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

