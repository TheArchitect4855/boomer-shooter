#pragma once

#include "entity.hpp"
#include "../hash_set.hpp"
#include "../bit_set.hpp"
#include "../list.hpp"

template <typename T, const unsigned int P = 64> class ComponentStorage {
	BitSet m_Occupied;
	List<T*> m_Pages;
public:
	ComponentStorage() : m_Pages{List<T*>()} {}

	ComponentStorage(const ComponentStorage& copy) = delete;

	~ComponentStorage() {
		for (unsigned int i = 0; i < m_Pages.count(); i += 1) {
			if (m_Pages[i] != nullptr) delete[] m_Pages[i];
		}
	}

	void add(Entity entity, const T& component) {
		if (entity >= m_Occupied.size()) m_Occupied.resize(entity + 1);

		unsigned int page = entity / P;
		while (page >= m_Pages.count()) m_Pages.push(nullptr);
		if (m_Pages[page] == nullptr) m_Pages[page] = new T[P];

		unsigned int index = entity % P;
		m_Pages[page][index] = component;
		m_Occupied.set(entity);
	}

	T* get(Entity entity) {
		if (entity >= m_Occupied.size() || !m_Occupied[entity]) return nullptr;
		return m_Pages[entity / P] + (entity % P);
	}

	HashSet<Entity, hashEntity> getEntities() {
		HashSet<Entity, hashEntity> entities;
		for (unsigned int i = 0; i < m_Occupied.size(); i += 1) {
			if (m_Occupied[i]) entities.add(i);
		}

		return entities;
	}

	void remove(Entity entity) {
		if (entity >= m_Occupied.size() || !m_Occupied[entity]) return;
		m_Occupied.clear(entity);
	}

	ComponentStorage& operator=(const ComponentStorage& copy) = delete;
};
