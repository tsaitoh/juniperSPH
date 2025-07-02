#include <vector>
#include <string>

#ifndef SIMDATA_H
#define SIMDATA_H

class SimData {
public:
    double time;
    std::vector<float> xyzh;
    std::vector<float> vxyzv;
    std::vector<float> fxyz;

    static inline const std::vector<std::string> posCols{"x", "y", "z", "h"};
    static inline const std::vector<std::string> velCols{"vx", "vy", "vz"};
    static inline const std::vector<std::string> varCols{"fx", "fy", "fz"};

    SimData(const std::string& filename);
};

#endif //SIMDATA_H
