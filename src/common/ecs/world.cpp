#include "world.hpp"

#include "system.hpp"
#include "../log.hpp"

Entity World::spawn() {
	TRACE("spawning entity. Free count: %d Next: %d", m_FreeEntities.count(), m_NextEntity);
	Entity e;
	if (m_FreeEntities.count() > 0) e = m_FreeEntities.pop();
	else e = m_NextEntity++;
	m_LiveEntities.add(e);
	TRACE("entity spawned. ID %d", e);
	return e;
}

void World::despawn(Entity entity) {
	if (!m_LiveEntities.remove(entity)) return;
	m_FreeEntities.push(entity);
}

void World::runSystem(System& system) {
	system.run(*this, EntityIter(*this, m_LiveEntities));
}

// --- COMPONENTS ---
template <> ComponentStorage<Model>& World::getStorage() {
	return m_ModelStorage;
}

template <> ComponentStorage<Transform>& World::getStorage() {
	return m_TransformStorage;
}
