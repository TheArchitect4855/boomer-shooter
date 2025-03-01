#include "context.hpp"

#include "../common/input.hpp"
#include "../common/log.hpp"

static void errorCallback(int error, const char* message) {
	ERROR("GLFW error: %s (%d)", message, error);
}

static void glDebugCallbackHandler(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	INFO("[OPENGL] %s", message);
}

ContextException::ContextException(const char* message) : m_Message{message} {}
const char* ContextException::what() const noexcept { return m_Message; }

Context::Context(ContextConfiguration& cfg) : m_Window{nullptr} {
	TRACE("initializing GLFW...");
	glfwSetErrorCallback(errorCallback);
	if (!glfwInit()) throw ContextException("failed to initialize GLFW");
	TRACE("GLFW initialized.");

	TRACE("creating window...");
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4); // For MSAA

	#ifdef DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	#endif

	m_Window = glfwCreateWindow(cfg.WindowSize.Width, cfg.WindowSize.Height, cfg.WindowTitle, nullptr, nullptr);
	TRACE("window created: %p", m_Window);
	if (!m_Window) throw ContextException("failed to create GLFW window");

	Input::initialize(m_Window);
	glfwMakeContextCurrent(m_Window);
	int version = gladLoadGL(glfwGetProcAddress);
	if (version == 0) throw ContextException("failed to initialize OpenGL");
	TRACE("loaded OpenGL %d.%d", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
	glEnable(GL_MULTISAMPLE); // Enable MSAA

	#ifdef DEBUG
	INFO("enabling OpenGL debugging");
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugCallbackHandler, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	#endif
}

Context::~Context() {
	TRACE("destroying context");
	if (m_Window) glfwDestroyWindow(m_Window);
	glfwTerminate();
	TRACE("GLFW terminated");
}

Size Context::canvasSize() const {
	int width, height;
	glfwGetWindowSize(m_Window, &width, &height);
	return Size{(unsigned int)width, (unsigned int)height};
}

bool Context::shouldTerminate() const { return glfwWindowShouldClose(m_Window); }

void Context::tick() const {
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

void Context::enableVSync() const {
	glfwSwapInterval(1);
	INFO("VSync enabled");
}

void Context::disableVSync() const {
	glfwSwapInterval(0);
	INFO("VSync disabled");
}
