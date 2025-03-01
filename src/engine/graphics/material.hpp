#pragma once

#include "../../common/color.hpp"
#include "texture.hpp"

struct Material {
	Texture Albedo;
	Color BaseColor;
	// TODO: Emission
	// TODO: Alpha mode
	// TODO: Alpha cutoff
	// TODO: Double sided
	// TODO: Normal map
	// TODO: Occlusion map
	// TODO: Metallic & roughness
};
