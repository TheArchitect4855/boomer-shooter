#pragma once

#include "context.hpp"
#include "graphics/renderer.hpp"
#include "../common/ecs/world.hpp"
#include "../common/list.hpp"

enum class SystemSchedule {
	render,
};

class Engine {
	const Context& m_Context;
	Renderer m_Renderer;
	World& m_World;
	Array<List<System*>> m_Schedules;
	void runSchedule(SystemSchedule schedule);
public:
	Engine(const Context& context, World& world);
	void addSystem(SystemSchedule schedule, System& system);
	void run();
};
