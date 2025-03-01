#pragma once

#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float3.hpp>

struct Camera {
	glm::vec3 Position;
	glm::quat Rotation;
};
