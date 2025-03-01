#include "entity_iter.hpp"

EntityIter::EntityIter(World& world, HashSet<Entity, hashEntity> entities)
: m_World{world}, m_EntityList{Array<Entity>()}, m_EntitySet{entities},
	m_EntityIndex{0}, m_Begun{false}
{}

void EntityIter::begin() {
	m_EntityList = m_EntitySet.toArray();
	m_EntityIndex = 0;
	m_Begun = true;
}

bool EntityIter::hasNext() {
	ASSERT(m_Begun);
	return m_EntityIndex < m_EntityList.length();
}

Entity EntityIter::next() {
	ASSERT(m_Begun);
	return m_EntityList[m_EntityIndex++];
}
