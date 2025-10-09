//
// Created by Andrew on 2025-09-12.
//

#include <numeric>
#include <cmath>

#include "ParticleSet.h"

ParticleSet::ParticleSet(SimData &data, std::vector<int> indices) {
    this->sourceData = &data;
    this->indices = indices;
}

ParticleSet::ParticleSet(SimData &data) {
    std::vector<int> indices(data.getParticleCount());
    std::iota(indices.begin(), indices.end(), 0);
    this->sourceData = &data;
    this->indices = indices;
}

Point3f ParticleSet::getCentreOfMass() {
    Point3f centreOfMass(0, 0, 0);
    for (int i : this->indices) {
        centreOfMass.x += this->sourceData->xyzh[4 * i];
        centreOfMass.y += this->sourceData->xyzh[4 * i + 1];
        centreOfMass.z += this->sourceData->xyzh[4 * i + 2];
    }

    centreOfMass.x /= this->getNumberOfParticles();
    centreOfMass.y /= this->getNumberOfParticles();
    centreOfMass.z /= this->getNumberOfParticles();

    return centreOfMass;
}

Box3f ParticleSet::getBoundingBox() {
    int fi = this->indices[0];

    Box3f box(sourceData->xyzh[4 * fi], sourceData->xyzh[4 * fi + 1], sourceData->xyzh[4 * fi + 2],
             sourceData->xyzh[4 * fi], sourceData->xyzh[4 * fi + 1], sourceData->xyzh[4 * fi + 2]);
    for (int i : this->indices) {
        float x = this->sourceData->xyzh[4 * i];
        float y = this->sourceData->xyzh[4 * i + 1];
        float z = this->sourceData->xyzh[4 * i + 2];

        if (x < box.x1) box.x1 = x;
        if (y < box.y1) box.y1 = y;
        if (z < box.z1) box.z1 = z;
        if (x > box.x2) box.x2 = x;
        if (y > box.y2) box.y2 = y;
        if (z > box.z2) box.z2 = z;
    }

    return box;
}

float ParticleSet::getBoundingRadius() {
    Point3f centre = getCentreOfMass();
    float boundingRadius = 0;

    for (int i : this->indices) {
        float x = this->sourceData->xyzh[4 * i];
        float y = this->sourceData->xyzh[4 * i + 1];
        float z = this->sourceData->xyzh[4 * i + 2];
        float centreDist = std::sqrt(((centre.x - x) * (centre.x - x) +
                            (centre.y - y) * (centre.y - y) +
                            (centre.z - z) * (centre.z - z)));

        if (centreDist > boundingRadius) boundingRadius = centreDist;
    }

    return boundingRadius;
}

float ParticleSet::getMaxSmoothingLength() {
    float maxSmoothingLength = 0;

    for (int i: this->indices) {
        if (this->sourceData->xyzh[4 * i + 3] > maxSmoothingLength) {
            maxSmoothingLength = this->sourceData->xyzh[4 * i + 3];
        }
    }

    return maxSmoothingLength;
}

std::size_t ParticleSet::getNumberOfParticles() {
    return this->indices.size();
}

std::vector<int> ParticleSet::getIndices() {
    return this->indices;
}

SimData& ParticleSet::getSourceData() {
    return *this->sourceData;
}

