#include <gtest/gtest.h>
#include "kernel.h"
#include <cmath>

TEST(KernelTest, KernelValueTest) {
    Kernel kernel = Kernel();

    EXPECT_FLOAT_EQ(kernel.valueAt(0), 1 / M_PI);
    EXPECT_FLOAT_EQ(kernel.valueAt(0.5), (1 - (3/ 8.0) + (3 / 32.0)) / M_PI);
    EXPECT_FLOAT_EQ(kernel.valueAt(1), 0.25 / M_PI);
    EXPECT_FLOAT_EQ(kernel.valueAt(1.5), 1 / 32.0 / M_PI);
    EXPECT_EQ(kernel.valueAt(2), 0);
}

