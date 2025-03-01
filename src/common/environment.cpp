#include "environment.hpp"

Environment::Environment(const Array<const char*>& envFiles, const Array<const char*>& skyboxFiles, float ambientBrightness)
: m_AmbientBrightness{ambientBrightness}
{
	m_EnvironmentMap = Cubemap::load(envFiles);
	m_Skybox = Cubemap::load(skyboxFiles);
}

float Environment::ambientBrightness() const { return m_AmbientBrightness; }

const Cubemap& Environment::environmentMap() const { return m_EnvironmentMap; }

const Cubemap& Environment::skybox() const { return m_Skybox; }
