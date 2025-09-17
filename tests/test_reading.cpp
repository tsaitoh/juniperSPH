#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Simulation.h"

TEST(ReaderTest, BasicReaderTest) {
    Simulation data = Simulation(".//files//test.csv");

    ASSERT_THAT(data.getSimData().xyzh, testing::ElementsAre(3, 5, 2, 0.5, 1, 5, 3, 2, 2, 20, 1, 0.5, 5, 10, 2, 0.1));
    ASSERT_THAT(data.getSimData().vxyzv, testing::ElementsAre(1.5, 2.5, 0.25, 4, 1, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5));
    ASSERT_THAT(data.getSimData().fxyz, testing::ElementsAre(1, 2, 3, 3, 2.3, 7.1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5));
    ASSERT_EQ(data.xmin, 1);
    ASSERT_EQ(data.xmax, 5);
    ASSERT_EQ(data.ymin, 5);
    ASSERT_EQ(data.ymax, 20);
    ASSERT_EQ(data.zmin, 1);
    ASSERT_EQ(data.zmax, 3);
}
