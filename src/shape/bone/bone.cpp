#include <cmath>
#include <vector>
#include <glm/glm.hpp>

#include "./bone.hpp"
#include "../sphere.hpp"

Bone::Bone(float length, JointID id) : Shape(id), length(length) {}