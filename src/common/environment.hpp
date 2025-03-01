#pragma once

#include "../engine/graphics/cubemap.hpp"

class Environment {
	Cubemap m_EnvironmentMap;
	Cubemap m_Skybox;
	float m_AmbientBrightness;
public:
	Environment() = default;
	Environment(const Array<const char*>& envFiles, const Array<const char*>& skyboxFiles, float ambientBrightness);

	float ambientBrightness() const;
	const Cubemap& environmentMap() const;
	const Cubemap& skybox() const;
};
