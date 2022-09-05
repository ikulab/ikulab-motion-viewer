#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "../cube/singleColorCube.hpp"
#include "./bone.hpp"

class StickTetrahedronBone : public Bone {
    std::unique_ptr<SingleColorCube> cube;

  public:
    StickTetrahedronBone(float length, JointID id);

    const std::vector<Vertex> &getVertices() const override;
    const std::vector<uint32_t> &getIndices() const override;

    void setBaseIndex(uint32_t baseIndex) override;
};