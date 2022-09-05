#pragma once

#include "./floor.hpp"

class GridFloor : public Floor {
    glm::vec3 color;
    float lineWidth;
    int numSplitX, numSPlitY;

  public:
    GridFloor(float width, float height, float lineWidth, int numSplitX,
              int numSplitY, glm::vec3 color, JointID id);
};