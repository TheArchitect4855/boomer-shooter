#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <exception>
#include "../common/size.hpp"

struct ContextConfiguration {
	const char* WindowTitle;
	Size WindowSize;
};

class ContextException : std::exception {
	const char* m_Message;
public:
	ContextException(const char* message);
	const char* what() const noexcept override;
};

class Context {
	GLFWwindow* m_Window;
public:
	Context(ContextConfiguration& config);
	Context(const Context& copy) = delete;
	~Context();
	Size canvasSize() const;
	bool shouldTerminate() const;
	void tick() const;

	void enableVSync() const;
	void disableVSync()const ;

	Context& operator=(const Context& copy) = delete;
};
