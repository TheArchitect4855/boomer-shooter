#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include "shader.hpp"
#include "../../common/ecs/system.hpp"
#include "../context.hpp"
#include "../../common/camera.hpp"

class Renderer : public System {
	Camera& m_Camera;
	glm::mat4 m_Projection;
	Mesh m_QuadMesh;
	ShaderGlobals m_ShaderGlobals;
	StandardShader m_StandardShader;
	SkyboxShader m_SkyboxShader;
public:
	Renderer(const Context& context, Camera& camera);
	Renderer(const Renderer& copy) = delete;
	~Renderer();
	void run(World &ecs, EntityIter iter) override;

	Renderer& operator=(Renderer& copy) = delete;
};
