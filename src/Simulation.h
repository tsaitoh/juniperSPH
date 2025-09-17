#include <vector>
#include <string>

#include "kernel.h"
#include "SimData.h"
#include "TreeNode.h"

#ifndef SIMULATION_H
#define SIMULATION_H

class Simulation {
    // The order of these arguments determines the initialize order in the
    // constructor. So don't change this order!
    SimData simData;
    ParticleSet globalSet;
    TreeNode baseNode;

    void buildTree();

public:
    explicit Simulation(const std::string& filename);
    explicit Simulation() : Simulation("") {};

    float xmin, xmax, ymin, ymax, zmin, zmax;

    float distBetween(int part1, int part2) const;
    void densityIterate(Kernel kernel);
    std::vector<int> getNeighbours(int part, Kernel kernel);
    float densityAt(int part, Kernel kernel);
    void setLimits();
    void setLimits(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
    int getParticleCount() const;

    SimData& getSimData();
    TreeNode& getBaseNode();
};

#endif //SIMULATION_H
