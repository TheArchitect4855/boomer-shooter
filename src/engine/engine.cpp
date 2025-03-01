#include "engine.hpp"

Engine::Engine(const Context& context, World& world)
: m_Context{context}, m_Renderer{Renderer(context, world.Camera)},
	m_World{world}
{
	m_Schedules = Array<List<System*>>(1); // NOTE: Keep this updated with the number of schedules
}

void Engine::addSystem(SystemSchedule schedule, System& system) {
	m_Schedules[(unsigned int)schedule].push(&system);
}

void Engine::run() {
	while (!m_Context.shouldTerminate()) {
		runSchedule(SystemSchedule::render);
		m_World.runSystem(m_Renderer);
		m_Context.tick();
	}
}

void Engine::runSchedule(SystemSchedule schedule) {
	List<System*>& systems = m_Schedules[(unsigned int)schedule];
	for (unsigned int i = 0; i < systems.count(); i += 1) m_World.runSystem(*systems[i]);
}
