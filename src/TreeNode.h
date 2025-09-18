//
// Created by Andrew on 2025-09-12.
//

#ifndef JUNIPEREXE_TREENODE_H
#define JUNIPEREXE_TREENODE_H
#include <memory>

#include "ParticleSet.h"


class TreeNode {
    bool leaf = true;
    std::shared_ptr<TreeNode> leftChild;
    std::shared_ptr<TreeNode> rightChild;
    TreeNode* parent;
    ParticleSet contents;
public:
    TreeNode(TreeNode* parent, ParticleSet contents): contents(contents){
        Point3f centreOfMass = contents.getCentreOfMass();
        this->x = centreOfMass.x;
        this->y = centreOfMass.y;
        this->z = centreOfMass.z;

        this->size = contents.getBoundingRadius();
        this->hmax = contents.getMaxSmoothingLength();
        this->parent = parent;
    }

    float x;
    float y;
    float z;
    float size;
    float hmax;

    TreeNode* getParent();
    std::shared_ptr<TreeNode> getLeftChild();
    std::shared_ptr<TreeNode> getRightChild();
    bool isLeaf();
    void splitLeaf();
    int getParticleCount();
    std::vector<int> getParticleIndices();
};


#endif //JUNIPEREXE_TREENODE_H