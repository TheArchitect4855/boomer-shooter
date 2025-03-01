#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include "material.hpp"
#include "cubemap.hpp"

struct ShaderGlobals {
	glm::mat4 ViewProjection;
	float AmbientBrightness;
};

class Shader {
protected:
	unsigned int m_Id;
	void attachShader(const char* source, unsigned int type);
	void linkProgram();
public:
	Shader();
	Shader(const Shader& copy) = delete;
	~Shader();

	unsigned int id();

	Shader& operator=(const Shader& copy) = delete;

	static void setGlobals(const ShaderGlobals& value);
};

class StandardShader : public Shader {
	unsigned int m_AlbedoLocation;
	unsigned int m_BaseColorLocation;
	unsigned int m_EnvironmentLocation;
	unsigned int m_TransformLocation;
public:
	StandardShader();
	void setMaterial(const Material& value);
	void setEnvironment(const Cubemap& value);
	void setTransform(const glm::mat4& value);
};

class SkyboxShader : public Shader {
	unsigned int m_TransformLocation;
public:
	SkyboxShader();
	void setTransform(const glm::mat4& value);
};
