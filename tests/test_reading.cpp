#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "SimData.h"

TEST(ReaderTest, BasicReaderTest) {
    SimData data = SimData(".//files//test.csv");

    ASSERT_THAT(data.xyzh, testing::ElementsAre(3, 5, 2, 0.5, 1, 5, 3, 2));
    ASSERT_THAT(data.vxyzv, testing::ElementsAre(1.5, 2.5, 0.25, 4, 1, -0.5));
    ASSERT_THAT(data.fxyz, testing::ElementsAre(1, 2, 3, 3, 2.3, 7.1));
    ASSERT_EQ(data.xmin, 1);
    ASSERT_EQ(data.xmax, 3);
    ASSERT_EQ(data.ymin, 5);
    ASSERT_EQ(data.ymax, 5);
    ASSERT_EQ(data.zmin, 2);
    ASSERT_EQ(data.zmax, 3);
}
