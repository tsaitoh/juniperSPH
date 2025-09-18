//
// Created by Andrew on 2025-09-12.
//

#include "TreeNode.h"

#include <iostream>

std::shared_ptr<TreeNode> TreeNode::getLeftChild() {
    return leftChild;
}

std::shared_ptr<TreeNode> TreeNode::getRightChild() {
    return rightChild;
}

bool TreeNode::isLeaf() {
    return this->leaf;
}

TreeNode* TreeNode::getParent() {
    return parent;
}

int TreeNode::getParticleCount() {
    return this->contents.getNumberOfParticles();
}

std::vector<int> TreeNode::getParticleIndices() {
    return this->contents.getIndices();
}

void TreeNode::splitLeaf() {
    if (!this->isLeaf()) {
        std::cout << "Warning: attempted splitLeaf() on a non-leaf node!" << std::endl;
        return;
    }

    Box3f box = this->contents.getBoundingBox();
    float xdiff = box.x2 - box.x1;
    float ydiff = box.y2 - box.y1;
    float zdiff = box.z2 - box.z1;

    float centre = this->x;
    int decomposeAxis = 0;
    if (ydiff >= xdiff && ydiff >= zdiff) {
        decomposeAxis = 1;
        centre = y;
    } else if (zdiff >= xdiff && zdiff >= ydiff) {
        decomposeAxis = 2;
        centre = z;
    }

    std::vector<int> leftIndices, rightIndices;
    for (int i : this->contents.getIndices()) {
        if (this->contents.getSourceData().xyzh[4 * i + decomposeAxis] > centre) {
            leftIndices.push_back(i);
        } else {
            rightIndices.push_back(i);
        }
    }

    ParticleSet leftSet(this->contents.getSourceData(), leftIndices);
    ParticleSet rightSet(this->contents.getSourceData(), rightIndices);

    TreeNode leftNode(this, leftSet);
    TreeNode rightNode(this, rightSet);
    this->leftChild = std::make_shared<TreeNode>(this, leftSet);
    this->rightChild = std::make_shared<TreeNode>(this, rightSet);
    this->leaf = false;
}
