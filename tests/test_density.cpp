#include <gtest/gtest.h>
#include "SimData.h"
#include "kernel.h"

TEST(DensityTest, SimpleDensityTest) {
    // Note: This test currently depends on the default m=0.01.
    SimData data = SimData();
    Kernel kernel = Kernel();

    int parts1d = 2;
    float L = 1.0f;
    float increment = L / parts1d;

    for (int x = 0; x < parts1d; x++) {
        for (int y = 0; y < parts1d; y++) {
            for (int z = 0; z < parts1d; z++) {
                data.xyzh.push_back(x * increment);
                data.xyzh.push_back(y * increment);
                data.xyzh.push_back(z * increment);
                // A large h value, so the distance between neighbouring particles is effectively zero.
                data.xyzh.push_back(100000000000000);
            }
        }
    }

    for (int part = 0; part < parts1d * parts1d * parts1d; part++) {
        ASSERT_FLOAT_EQ(data.densityAt(part, kernel), 0.0222816920329);
    }
}
