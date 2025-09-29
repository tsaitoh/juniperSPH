//
// Created by Andrew on 2025-07-01.
//

#include "Simulation.h"

#include <cmath>
#include <filesystem>
#include <iostream>
#include <string>
#include <ranges>
#include <limits>
#include <stack>

constexpr int NODE_PARTICLE_MIN = 10;
constexpr int MAX_DENSITY_ITERATIONS = 400;

Simulation::Simulation(const std::string& filename) : simData(filename), globalSet(simData), baseNode(nullptr, globalSet)  {
    // Do not set limits if data is not supplied, like in unit tests.
    if (filename == "") {
        return;
    }
    this->setLimits();
}

void Simulation::buildTree() {
    TreeNode newNode(nullptr, this->globalSet);
    baseNode = newNode;

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

std::vector<int> Simulation::getNeighboursByTree(int target, TreeNode& targetNode, Kernel kernel) {
    std::vector<int> neighbours;
    std::stack<TreeNode*> nodeStack;
    nodeStack.push(&this->baseNode);

    while (!nodeStack.empty()) {
        TreeNode* nextNode = nodeStack.top();
        nodeStack.pop();

        float distance = distBetweenNodes(*nextNode, targetNode);
        float targetBounds = nextNode->size + targetNode.size + (kernel.getRadius() * std::max(targetNode.hmax, nextNode->hmax));

        if (distance * distance < targetBounds * targetBounds) {
            if (nextNode->isLeaf()) {
                for (int candidate : nextNode->getParticleIndices()) {
                    float dist = distBetween(target, candidate);
                    float targetH = this->getSimData().xyzh[target * 4 + 3];
                    float candidateH = this->getSimData().xyzh[candidate * 4 + 3];
                    if (kernel.valueAt(dist / targetH) > 0 || kernel.valueAt(dist / candidateH) > 0) {
                        neighbours.push_back(candidate);
                    }
                }
            } else {
                nodeStack.push(nextNode->getLeftChild().get());
                nodeStack.push(nextNode->getRightChild().get());
            }
        }
    }

    return neighbours;
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

float Simulation::distBetween(float x1, float x2, float y1, float y2, float z1, float z2) const {
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


float Simulation::distBetween(int part1, int part2) const {
    float x1 = simData.xyzh[4 * part1], y1 = simData.xyzh[4 * part1 + 1], z1 = simData.xyzh[4 * part1 + 2];
    float x2 = simData.xyzh[4 * part2], y2 = simData.xyzh[4 * part2 + 1], z2 = simData.xyzh[4 * part2 + 2];

    return distBetween(x1, x2, y1, y2, z1, z2);
}

float Simulation::distBetweenNodes(TreeNode& node1, TreeNode& node2) const {
    float x1 = node1.x, y1 = node1.y, z1 = node1.z;
    float x2 = node2.x, y2 = node2.y, z2 = node2.z;

    return distBetween(x1, x2, y1, y2, z1, z2);
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

float Simulation::findDensityForParticle(int particle, TreeNode& node, Kernel kernel) {
    float oldH = std::numeric_limits<float>::max();
    float newH = simData.xyzh[particle * 4 + 3];
    int iterationCount = 0;

    while (std::abs(newH - oldH) / simData.xyzh[particle * 4 + 3] > 0.00001) {
        std::vector<int> neighbours = getNeighboursByTree(particle, node, kernel);

        float hfact = 1.2;
        float density = simData.m * (hfact / newH) * (hfact / newH) * (hfact / newH);
        float grad = -3 * (newH / density);
        float omega = 1 - grad * (neighbours.size() * (simData.m * (kernel.gradientAt(newH) / (newH * newH * newH * newH))));

        float density_sum = 0;
        for (int neighbour : neighbours) {
            density_sum += simData.m * kernel.valueAt(distBetween(particle, neighbour) / newH) / (newH * newH * newH);
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

        if (iterationCount > MAX_DENSITY_ITERATIONS) {
            break;
        }
    }
    return newH;

}

void Simulation::densityIterate(Kernel kernel) {
    buildTree();
    std::vector<int> neighbours;
    std::stack<TreeNode*> nodeStack;
    nodeStack.push(&this->baseNode);

    while (!nodeStack.empty()) {
        TreeNode* node = nodeStack.top();
        nodeStack.pop();
        if (!node->isLeaf()) {
            nodeStack.push(node->getLeftChild().get());
            nodeStack.push(node->getRightChild().get());
            continue;
        }
        for (int i : node->getParticleIndices()) {
            simData.xyzh[i * 4 + 3] = findDensityForParticle(i, *node, kernel);
        }
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
