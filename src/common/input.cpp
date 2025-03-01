#include "input.hpp"


GLFWwindow* Input::m_Window;
BitSet Input::m_KeyStates;
glm::vec2 Input::m_LastMousePos;
bool Input::m_IsCursorEnabled;

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (window != m_Window) return;
	if (action == GLFW_PRESS) m_KeyStates.set(key);
	else if (action == GLFW_RELEASE) m_KeyStates.set(key + GLFW_KEY_LAST);
}

void Input::initialize(GLFWwindow* window) {
	m_Window = window;
	m_KeyStates = BitSet(GLFW_KEY_LAST * 2);
	m_IsCursorEnabled = true;
	glfwSetKeyCallback(window, keyCallback);
}

bool Input::isKeyJustPressed(unsigned int key) {
	bool pressed = m_KeyStates[key];
	m_KeyStates.clear(key);
	return pressed;
}

bool Input::isKeyJustReleased(unsigned int key) {
	bool released = m_KeyStates[key + GLFW_KEY_LAST];
	m_KeyStates.clear(key + GLFW_KEY_LAST);
	return released;
}

bool Input::isKeyDown(unsigned int key) {
	return glfwGetKey(m_Window, key) == GLFW_PRESS;
}

bool Input::isKeyUp(unsigned int key) {
	return glfwGetKey(m_Window, key) == GLFW_RELEASE;
}

glm::vec2 Input::mouseDelta() {
	double x, y;
	glfwGetCursorPos(m_Window, &x, &y);
	glm::vec2 pos = glm::vec2(x, y);
	glm::vec2 delta = pos - m_LastMousePos;
	m_LastMousePos = pos;
	return delta;
}

void Input::disableCursor() {
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	m_IsCursorEnabled = false;
}

void Input::enableCursor() {
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	m_IsCursorEnabled = true;
}

bool Input::isCursorEnabled() { return m_IsCursorEnabled; }
