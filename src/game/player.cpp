#include "player.hpp"

#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/geometric.hpp>
#include "../common/input.hpp"

#define MAX_LOOK 1.57
#define MOVE_SPEED 10.0f

Player::Player(World& world) : m_LookRotation{glm::vec2(0)} {
	Input::disableCursor();
	m_Player = world.spawn();
}

void Player::run(World& world, EntityIter iter) {
	float deltaTime = m_Timer.reset();

	if (Input::isKeyJustPressed(GLFW_KEY_ESCAPE)) {
		if (Input::isCursorEnabled()) Input::disableCursor();
		else Input::enableCursor();
	}

	// Mouse look
	glm::vec2 mouseDelta = Input::mouseDelta();
	m_LookRotation.x -= mouseDelta.y * 0.001;
	m_LookRotation.y -= mouseDelta.x * 0.001;
	if (m_LookRotation.x < -MAX_LOOK) m_LookRotation.x = -MAX_LOOK;
	else if (m_LookRotation.x > MAX_LOOK) m_LookRotation.x = MAX_LOOK;

	glm::quat lookY = glm::angleAxis(m_LookRotation.y, glm::vec3(0, 1, 0));
	world.Camera.Rotation = lookY * glm::angleAxis(m_LookRotation.x, glm::vec3(1, 0, 0));

	// Movement
	glm::vec3 move(0);
	if (Input::isKeyDown(GLFW_KEY_A)) move.x -= 1;
	if (Input::isKeyDown(GLFW_KEY_D)) move.x += 1;
	if (Input::isKeyDown(GLFW_KEY_W)) move.z -= 1;
	if (Input::isKeyDown(GLFW_KEY_S)) move.z += 1;

	glm::vec3& pos = world.Camera.Position;
	float moveLen = glm::length(move);
	if (moveLen > 0) pos += lookY * (move / moveLen) * MOVE_SPEED * deltaTime;
}
