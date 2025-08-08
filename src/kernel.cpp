#include <cmath>
#include "kernel.h"

float Kernel::getRadius() const
{
    return 2;
}

float Kernel::valueAt(const float q) const
{
    float aq = abs(q);

    float norm = 1 / M_PI;

    if (aq >= 2) 
    {
        return 0;
    }

    if (aq >= 1)
    {
        return norm * 0.25 * (2 - aq) * (2 - aq) * (2 - aq);
    }

    return norm * (1 - 1.5 * aq * aq + 0.75 * aq * aq * aq);
}

float Kernel::gradientAt(const float q) const {
    float aq = abs(q);
    float norm = 1 / M_PI;

    if (aq >= 2) {
        return 0;
    }

    if (aq >= 1) {
        return norm * -0.75 * (aq - 2) * (aq - 2);
    }

    return norm * aq * (2.25 * aq - 3);
}

