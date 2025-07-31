#include <vector>
#include <string>

#include "kernel.h"

#ifndef SIMDATA_H
#define SIMDATA_H

class SimData {
public:
    double time;
    float m;
    std::vector<float> xyzh;
    std::vector<float> vxyzv;
    std::vector<float> fxyz;

    static inline const std::vector<std::string> posCols{"x", "y", "z", "h"};
    static inline const std::vector<std::string> velCols{"vx", "vy", "vz"};
    static inline const std::vector<std::string> varCols{"fx", "fy", "fz"};

    explicit SimData(const std::string& filename);
    explicit SimData() : SimData("") {};

    float distBetween(int part1, int part2) const;
    void densityIterate(Kernel kernel);
    std::vector<int> getNeighbours(int part, Kernel kernel);
    float densityAt(int part, Kernel kernel);
};

#endif //SIMDATA_H
