//
// Created by Andrew on 2025-09-12.
//

#ifndef JUNIPEREXE_PARTICLESET_H
#define JUNIPEREXE_PARTICLESET_H
#include <vector>

#include "SimData.h"

struct Point3f {
    float x, y, z;
};

struct Box3f {
    float x1, y1, z1, x2, y2, z2;
};

class ParticleSet {
    SimData* sourceData;
    std::vector<int> indices;

public:
    Point3f getCentreOfMass();
    Box3f getBoundingBox();
    float getBoundingRadius();
    float getMaxSmoothingLength();
    float getNumberOfParticles();
    std::vector<int> getIndices();
    SimData& getSourceData();
    ParticleSet(SimData& data, std::vector<int> indices);
    ParticleSet(SimData& data);
};


#endif //JUNIPEREXE_PARTICLESET_H