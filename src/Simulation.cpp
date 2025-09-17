//
// Created by Andrew on 2025-07-01.
//

#include "Simulation.h"

#include <cmath>
#include <filesystem>
#include <string>
#include <ranges>
#include <limits>
#include <stack>

constexpr int NODE_PARTICLE_MIN = 10;

Simulation::Simulation(const std::string& filename) : simData(filename), globalSet(simData), baseNode(nullptr, globalSet)  {
    // Do not build tree if data is not supplied, like in unit tests.
    if (filename == "") {
        return;
    }
    this->setLimits();

    std::stack<TreeNode*> nodeStack;
    nodeStack.push(&this->baseNode);

    while (!nodeStack.empty()) {
        TreeNode* node = nodeStack.top();
        nodeStack.pop();
        if (node->getParticleCount() >= NODE_PARTICLE_MIN) {
            node->splitLeaf();
            nodeStack.push(node->getLeftChild().get());
            nodeStack.push(node->getRightChild().get());
        }
    }
}

std::vector<int> Simulation::getNeighbours(int part, Kernel kernel) {
    // Naive strategy for neighbour finding, will replace with kd-tree
    float tarH = simData.xyzh[4 * part+3];
    std::vector<int> neighbours;

    for (int i = 0; i < this->getParticleCount(); i++) {
        float dist = distBetween(part, i);
        if (kernel.valueAt(dist / tarH) > 0 || kernel.valueAt(dist / simData.xyzh[4 * i+3]) > 0) {
            neighbours.push_back(i);
        }
    }

    return neighbours;
}

float Simulation::distBetween(int part1, int part2) const {
    float x1 = simData.xyzh[4 * part1], y1 = simData.xyzh[4 * part1 + 1], z1 = simData.xyzh[4 * part1 + 2];
    float x2 = simData.xyzh[4 * part2], y2 = simData.xyzh[4 * part2 + 1], z2 = simData.xyzh[4 * part2 + 2];

    float dx = std::abs(x1 - x2);
    float dy = std::abs(y1 - y2);
    float dz = std::abs(z1 - z2);

    if (dx > (this->xmax - this->xmin) / 2) {
        dx = (this->xmax - this->xmin) - dx;
    }
    if (dy > (this->ymax - this->ymin) / 2) {
        dy = (this->ymax - this->ymin) - dy;
    }
    if (dz > (this->zmax - this->zmin) / 2) {
        dz = (this->zmax - this->zmin) - dz;
    }

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}


float Simulation::densityAt(int part, Kernel kernel) {
    std::vector<int> neighbours = getNeighbours(part, kernel);

    float density = 0.0;
    for (int i : neighbours) {
        float dist = distBetween(part, i);
        float tarH = simData.xyzh[4 * part+3];
        density += kernel.valueAt(dist / tarH) * simData.m;
    }

    return density;
}


void Simulation::densityIterate(Kernel kernel) {
    int iterationCount = 0;
    int maxIterations = 40;

    for (int i = 0; i < this->getParticleCount(); i++) {
        float oldH = std::numeric_limits<float>::max();
        float newH = simData.xyzh[i * 4 + 3];

        while (std::abs(newH - oldH) / simData.xyzh[i * 4 + 3] > 0.00001) {
            std::vector<int> neighbours = getNeighbours(i, kernel);
            float hfact = 1.2;
            float density = simData.m * (hfact / newH) * (hfact / newH) * (hfact / newH);
            float grad = -3 * (newH / density);
            float omega = 1 - grad * (neighbours.size() * (simData.m * (kernel.gradientAt(newH) / (newH * newH * newH * newH))));

            float density_sum = 0;
            for (int neighbour : neighbours) {
                density_sum += simData.m * kernel.valueAt(distBetween(i, neighbour) / newH) / (newH * newH * newH);
            }

            oldH = newH;
            newH = newH - (density_sum - density) / ((-3 * density * omega) / newH);
            iterationCount++;

            if (newH > 1.4 * oldH) {
                newH = 1.4 * oldH;
            }
            else if (newH < 0.7 * oldH) {
                newH = 0.7 * oldH;
            }

            if (iterationCount > maxIterations) {
                break;
            }
        }
        simData.xyzh[i * 4 + 3] = newH;
        iterationCount = 0;
    }
}

void Simulation::setLimits() {
    xmin = xmax = simData.xyzh[0];
    ymin = ymax = simData.xyzh[1];
    zmin = zmax = simData.xyzh[2];

    for (int i = 0; i < this->getParticleCount(); i++) {
        float x = simData.xyzh[4 * i], y = simData.xyzh[4 * i + 1], z = simData.xyzh[4 * i + 2];

        if (xmin > x) { xmin = x; }
        if (ymin > y) { ymin = y; }
        if (zmin > z) { zmin = z; }
        if (xmax < x) { xmax = x; }
        if (ymax < y) { ymax = y; }
        if (zmax < z) { zmax = z; }
    }
}

void Simulation::setLimits(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) {
    this->xmin = xmin;
    this->xmax = xmax;
    this->ymin = ymin;
    this->ymax = ymax;
    this->zmin = zmin;
    this->zmax = zmax;
}

int Simulation::getParticleCount() const {
    return this->simData.getParticleCount();
}

SimData& Simulation::getSimData() {
    return this->simData;
}

TreeNode& Simulation::getBaseNode() {
    return this->baseNode;
}
