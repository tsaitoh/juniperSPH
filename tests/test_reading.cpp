#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "SimData.h"

TEST(ReaderTest, BasicReaderTest) {
    SimData data = SimData(".//files//test.csv");

    ASSERT_THAT(data.xyzh, testing::ElementsAre(3, 1, 2));
    ASSERT_THAT(data.vxyzv, testing::ElementsAre(5, 5, 4));
    ASSERT_THAT(data.fxyz, testing::ElementsAre(2, 3, 1));
}
