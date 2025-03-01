#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include "../array.hpp"
#include "../../engine/graphics/mesh.hpp"

/* STEPS TO ADD A NEW COMPONENT:
 * 1. Create component struct here.
 * 2. In world.hpp, add a ComponentStorage for the new type.
 * 3. In world.cpp, add a `getStorage` specialization for the new type.
 */

struct Model {
	Array<Mesh> Meshes;
};

struct Transform {
	glm::mat4 Matrix;
};
