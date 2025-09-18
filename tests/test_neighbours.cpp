#include <gtest/gtest.h>
#include "Simulation.h"
#include "kernel.h"

TEST(NeighboursTest, BasicNeigboursTest) {
    Simulation data = Simulation(".//files//test.csv");
    Kernel kernel = Kernel();

    // The smoothing length of particle 0 is too small to see particle 1,
    // however it is counted as a neighbour anyways, since all particles count themselves as neighbours.
    ASSERT_EQ(data.getNeighbours(0, kernel).size(), 2);
    // The smoothing length of particle 1 is sufficient to see particle 0, and particle 2 via periodic boundaries
    ASSERT_EQ(data.getNeighbours(1, kernel).size(), 3);
    // Particle 2 is neighbours with particle 1 through periodic boundaries.
    ASSERT_EQ(data.getNeighbours(2, kernel).size(), 2);
    // Particle 3 is too small to have any neighbours besides itself.
    ASSERT_EQ(data.getNeighbours(3, kernel).size(), 1);
}


void assertAtDepth(TreeNode* node, int depth, int partCountAtDepth) {
    if (depth > 0) {
        assertAtDepth(node->getLeftChild().get(), depth - 1, partCountAtDepth);
        assertAtDepth(node->getRightChild().get(), depth - 1, partCountAtDepth);
    } else {
        ASSERT_EQ(node->getParticleCount(), partCountAtDepth);
        ASSERT_TRUE(node->isLeaf());
    }
}


TEST(NeighboursTest, TreeBuildTest) {
    Simulation sim = Simulation(".//files//kd_test.csv");
    sim.buildTree();

    assertAtDepth(&sim.getBaseNode(), 6, 5);
}
