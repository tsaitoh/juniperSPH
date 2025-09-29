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
    std::vector<TreeNode*> leaves;

    float distBetween(float x1, float x2, float y1, float y2, float z1, float z2) const;
    float findDensityForParticle(int particle, TreeNode& node, Kernel kernel);

public:
    explicit Simulation(const std::string& filename);
    explicit Simulation() : Simulation("") {};

    float xmin, xmax, ymin, ymax, zmin, zmax;

    float distBetween(int part1, int part2) const;
    float distBetweenNodes(TreeNode& node1, TreeNode& node2) const;
    void densityIterate(Kernel kernel);
    std::vector<int> getNeighbours(int part, Kernel kernel);
    std::vector<int> getNeighboursByTree(int target, TreeNode& node, Kernel kernel);
    float densityAt(int part, Kernel kernel);
    void setLimits();
    void setLimits(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
    int getParticleCount() const;

    SimData& getSimData();
    TreeNode& getBaseNode();
    void buildTree();
};

#endif //SIMULATION_H
