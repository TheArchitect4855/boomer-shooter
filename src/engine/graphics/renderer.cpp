#include "renderer.hpp"

#include <glm/matrix.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../../common/ecs/components.hpp"
#include "shader.hpp"

static Mesh createQuadMesh() {
	unsigned int vao;
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int buffers[2];
	glGenBuffers(2, buffers);

	const float vertices[] = { -1, 1, 1, 1, 1, -1, -1, -1 };
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	const unsigned short indices[] = { 0, 3, 1, 1, 3, 2 };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	return Mesh(vao, sizeof(indices) / sizeof(unsigned short), 0, GL_UNSIGNED_SHORT, GL_TRIANGLES);
}

Renderer::Renderer(const Context& context, Camera& camera) : m_Camera{camera}, m_StandardShader{StandardShader()} {
	m_QuadMesh = createQuadMesh();
	Size canvasSize = context.canvasSize();
	float aspect = canvasSize.Width / (float)canvasSize.Height;
	m_Projection = glm::perspective(1.57f, aspect, 0.01f, 100.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);

	glEnable(GL_CULL_FACE);
	glClearColor(0, 0.75, 1, 1);
}

Renderer::~Renderer() { }

void Renderer::run(World& world, EntityIter iter) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render models
	glm::mat4 view = glm::mat4(1);
	m_ShaderGlobals.AmbientBrightness = world.Environment.ambientBrightness();
	m_ShaderGlobals.ViewProjection = m_Projection * glm::inverse(glm::translate(view, m_Camera.Position) * glm::mat4_cast(m_Camera.Rotation));
	Shader::setGlobals(m_ShaderGlobals);

	iter.withComponent<Model>()
		.withComponent<Transform>();
	iter.begin();
	glUseProgram(m_StandardShader.id());
	m_StandardShader.setEnvironment(world.Environment.environmentMap());
	while (iter.hasNext()) {
		Entity e = iter.next();
		Transform* transform = world.getComponent<Transform>(e);
		m_StandardShader.setTransform(transform->Matrix);

		Model* model = world.getComponent<Model>(e);
		for (unsigned int i = 0; i < model->Meshes.length(); i += 1) {
			m_StandardShader.setMaterial(model->Meshes[i].Material);
			model->Meshes[i].draw();
		}
	}

	// Render skybox
	unsigned int skyboxTextureId = world.Environment.skybox().texture().id();
	if (skyboxTextureId > 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureId);
		glUseProgram(m_SkyboxShader.id());

		glm::mat4 transform = glm::mat4_cast(m_Camera.Rotation) * glm::inverse(m_Projection);
		m_SkyboxShader.setTransform(transform);

		glDepthFunc(GL_LEQUAL);
		m_QuadMesh.draw();
		glDepthFunc(GL_LESS);
	}
}
