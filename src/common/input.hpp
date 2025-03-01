#pragma once

#include <GLFW/glfw3.h>
#include <glm/ext/vector_float2.hpp>
#include "bit_set.hpp"

class Input {
	static GLFWwindow* m_Window;
	static BitSet m_KeyStates;
	static glm::vec2 m_LastMousePos;
	static bool m_IsCursorEnabled;
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
public:
	static void initialize(GLFWwindow* window);

	static bool isKeyJustPressed(unsigned int key);
	static bool isKeyJustReleased(unsigned int key);
	static bool isKeyDown(unsigned int key);
	static bool isKeyUp(unsigned int key);

	static glm::vec2 mouseDelta();

	static void disableCursor();
	static void enableCursor();
	static bool isCursorEnabled();
};
