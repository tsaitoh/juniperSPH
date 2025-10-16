//
// Created by Andrew on 2025-09-16.
//

#ifndef SIMDATA_H
#define SIMDATA_H
#include <string>
#include <vector>


class SimData {
public:
    explicit SimData(const std::string& filename);
    explicit SimData() : SimData("") {};

    static inline const std::vector<std::string> posCols{"x", "y", "z", "h"};
    static inline const std::vector<std::string> velCols{"vx", "vy", "vz"};
    static inline const std::vector<std::string> varCols{"fx", "fy", "fz"};

    double time;
    float m;
    std::vector<float> xyzh;
    std::vector<float> vxyzv;
    std::vector<float> fxyz;

    int getParticleCount() const;

    void toCSV(const std::string& filename);
};


#endif //SIMDATA_H