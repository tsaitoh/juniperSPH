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
    m = 0.01;

    if (filename.empty()) {
        return;
    }

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
    float tarH = xyzh[4 * part+3];
    std::vector<int> neighbours;

    for (int i = 0; i < std::floor(xyzh.size() / 4); i++) {
        if (i == part) {
            continue;
        }
        float dist = distBetween(part, i);
        if (kernel.valueAt(dist / tarH) > 0) {
            neighbours.push_back(i);
        }
    }

    return neighbours;
}

float SimData::distBetween(int part1, int part2) const {
    float x1 = xyzh[4 * part1], y1 = xyzh[4 * part1 + 1], z1 = xyzh[4 * part1 + 2];
    float x2 = xyzh[4 * part2], y2 = xyzh[4 * part2 + 1], z2 = xyzh[4 * part2 + 2];

    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
}


float SimData::densityAt(int part, Kernel kernel) {
    float thisX = xyzh[4 * part], thisY = xyzh[4 * part+1], thisZ = xyzh[4 * part+2], thisH = xyzh[4 * part+3];
    std::vector<int> neighbours = getNeighbours(part, kernel);

    float density = 0.0;
    for (int i : neighbours) {
        float dist = distBetween(part, i);
        float tarH = xyzh[4 * part+3];
        density += kernel.valueAt(dist / tarH) * m;
    }

    return density;
}


void SimData::densityIterate(Kernel kernel) {
    int totalIterationCount = 0;
    int iterationCount = 0;
    int maxIterations = 1000;

    for (int i = 0; i < std::floor(xyzh.size() / 4); i++) {
        float oldH = MAXFLOAT;
        float newH = xyzh[i * 4 + 3];

        while (std::abs(newH - oldH) / xyzh[i * 4 + 3] > 0.0001) {
            std::vector<int> neighbours = getNeighbours(i, kernel);
            float hfact = 1.2;
            float pressure = m * (hfact / newH) * (hfact / newH) * (hfact / newH);
            float grad = 3 * (newH / pressure);
            float omega = 1 - grad * (m * neighbours.size());

            float pressure_sum = 0;
            for (int neighbour : neighbours) {
                pressure_sum += m * kernel.valueAt(distBetween(i, neighbour) / newH);
            }

            oldH = newH;
            newH = newH - pressure_sum / ((-3 * pressure * omega) / newH);
            totalIterationCount++;
            iterationCount++;

            if (iterationCount > 1) {
                std::cout << "At particle " << i << ", iteration " << iterationCount << std::endl;
            }

            if (iterationCount > maxIterations) {
                break;
            }
        }
        iterationCount = 0;
    }

    std::cout << "Finished with " << totalIterationCount << " iterations on " << xyzh.size() / 4 << " particles." << std::endl;
    std::cout << "Average of " << totalIterationCount / (xyzh.size() / 4.0) << " iterations per particle." << std::endl;
}


