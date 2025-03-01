#pragma once

#include "../common/ecs/system.hpp"
#include "../common/timer.hpp"

class Player : public System {
	glm::vec2 m_LookRotation;
	Entity m_Player;
	Timer m_Timer;
public:
	Player(World& world);
	void run(World &world, EntityIter iter) override;
};
