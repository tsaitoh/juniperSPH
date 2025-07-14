#include <gtest/gtest.h>
#include "SimData.h"
#include "kernel.h"

TEST(NeighboursTest, BasicNeigboursTest) {
    SimData data = SimData(".//files//test.csv");
    Kernel kernel = Kernel();

    // The smoothing length of particle 0 is too small to see particle 1
    ASSERT_EQ(data.getNeighbours(0, kernel).size(), 0);
    // The smoothing length of particle 1 is sufficient to see particle 0.
    ASSERT_EQ(data.getNeighbours(1, kernel).size(), 1);
}