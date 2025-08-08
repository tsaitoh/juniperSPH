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

TEST(KernelTest, GradientValueTest) {
    Kernel kernel = Kernel();

    EXPECT_FLOAT_EQ(kernel.gradientAt(0), 0);
    EXPECT_FLOAT_EQ(kernel.gradientAt(0.5), ((2.25 * 0.5) - 3) * 0.5 / M_PI);
    EXPECT_FLOAT_EQ(kernel.gradientAt(1), -0.75 / M_PI);
    EXPECT_FLOAT_EQ(kernel.gradientAt(1.5), -0.75 * (0.5 * 0.5) / M_PI);
    EXPECT_EQ(kernel.gradientAt(2), 0);
}

