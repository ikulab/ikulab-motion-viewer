#include <cmath>
#include <glm/glm.hpp>
#include <vector>

#include "../sphere.hpp"
#include "./bone.hpp"

Bone::Bone(float length, JointID id) : Shape(id), length(length) {}