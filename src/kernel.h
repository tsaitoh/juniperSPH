#pragma once

class Kernel
{
    public:
        float valueAt(const float) const;
        float gradientAt(const float) const;
        float getRadius() const;
};
