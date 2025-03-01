#pragma once

#include "component_storage.hpp"
#include "world.hpp"
#include "../hash_set.hpp"

class EntityIter {
	World& m_World;
	Array<Entity> m_EntityList;
	HashSet<Entity, hashEntity> m_EntitySet;
	unsigned int m_EntityIndex;
	bool m_Begun;
public:
	EntityIter(World& world, HashSet<Entity, hashEntity> entities);

	void begin();
	bool hasNext();
	Entity next();

	template <typename T> EntityIter& withComponent() {
		ComponentStorage<T>& storage = m_World.getStorage<T>();
		HashSet<Entity, hashEntity> storedEntities = storage.getEntities();
		m_EntitySet.intersect(storedEntities);
		return *this;
	}
};
