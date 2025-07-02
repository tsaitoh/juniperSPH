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

    SimData(const std::string& filename);
};

#endif //SIMDATA_H
