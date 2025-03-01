#include "shader.hpp"

#include <glad/gl.h>
#include <cstdlib>
#include "shader_sources.hpp"
#include "../../common/assert.hpp"
#include "../../common/log.hpp"

static bool isInitialized = false;
static unsigned int globalsBuffer, whiteTexture;

static void createGlobalsBuffer() {
	glGenBuffers(1, &globalsBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, globalsBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(ShaderGlobals), nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, globalsBuffer);
}

static void createWhiteTexture() {
	unsigned char white[] = {255,255,255};
	glGenTextures(1, &whiteTexture);
	glBindTexture(GL_TEXTURE_2D, whiteTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &white);
}

static void init() {
	isInitialized = true;
	createGlobalsBuffer();
	createWhiteTexture();
}

Shader::Shader() {
	if (!isInitialized) init();

	m_Id = glCreateProgram();
	TRACE("creating shader program with ID %d", m_Id);
}

StandardShader::StandardShader() {
	attachShader(standardVertexSource, GL_VERTEX_SHADER);
	attachShader(standardFragmentSource, GL_FRAGMENT_SHADER);
	linkProgram();

	m_AlbedoLocation = glGetUniformLocation(m_Id, "albedo");
	ASSERT(m_AlbedoLocation >= 0);

	m_BaseColorLocation = glGetUniformLocation(m_Id, "baseColor");
	ASSERT(m_BaseColorLocation >= 0);

	m_EnvironmentLocation = glGetUniformLocation(m_Id, "environment");
	ASSERT(m_EnvironmentLocation >= 0);

	m_TransformLocation = glGetUniformLocation(m_Id, "transform");
	ASSERT(m_TransformLocation >= 0);
}

SkyboxShader::SkyboxShader() {
	attachShader(skyboxVertexSource, GL_VERTEX_SHADER);
	attachShader(skyboxFragmentSource, GL_FRAGMENT_SHADER);
	linkProgram();

	m_TransformLocation = glGetUniformLocation(m_Id, "transform");
	ASSERT(m_TransformLocation >= 0);
}

Shader::~Shader() {
	TRACE("deleting shader program with ID %d", m_Id);
	glDeleteProgram(m_Id);
}

unsigned int Shader::id() { return m_Id; }

void Shader::setGlobals(const ShaderGlobals& value) {
	glNamedBufferData(globalsBuffer, sizeof(ShaderGlobals), &value, GL_STATIC_DRAW);
}

void Shader::attachShader(const char* source, unsigned int type) {
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	int i;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &i);
	if (!i) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &i);
		char* log = new char[i];
		glGetShaderInfoLog(shader, i, nullptr, log);
		ERROR("failed to compile shader: %s", log);
		std::exit(1);
	}

	glAttachShader(m_Id, shader);
	glDeleteShader(shader);
}

void Shader::linkProgram() {
	glLinkProgram(m_Id);

	int i;
	glGetProgramiv(m_Id, GL_LINK_STATUS, &i);
	if (!i) {
		glGetProgramiv(m_Id, GL_INFO_LOG_LENGTH, &i);
		char* log = new char[i];
		glGetProgramInfoLog(m_Id, i, nullptr, log);
		ERROR("failed to link shader program: %s", log);
		std::exit(1);
	}
}

void StandardShader::setMaterial(const Material& value) {
	glActiveTexture(GL_TEXTURE0);
	if (value.Albedo.id() == 0) glBindTexture(GL_TEXTURE_2D, whiteTexture);
	else glBindTexture(GL_TEXTURE_2D, value.Albedo.id());
	glUniform1i(m_AlbedoLocation, 0);
	glUniform4f(m_BaseColorLocation, value.BaseColor.R, value.BaseColor.G, value.BaseColor.B, value.BaseColor.A);
}

void StandardShader::setEnvironment(const Cubemap& value) {
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, value.texture().id());
	glUniform1i(m_EnvironmentLocation, 1);
}

void StandardShader::setTransform(const glm::mat4& value) {
	glUniformMatrix4fv(m_TransformLocation, 1, GL_FALSE, &value[0][0]);
}

void SkyboxShader::setTransform(const glm::mat4& value) {
	glUniformMatrix4fv(m_TransformLocation, 1, GL_FALSE, &value[0][0]);
}
