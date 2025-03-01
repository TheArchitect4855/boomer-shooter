#pragma once

#include "component_storage.hpp"
#include "components.hpp"
#include "entity.hpp"
#include "../camera.hpp"
#include "../environment.hpp"
#include "../hash_set.hpp"
#include "../list.hpp"

class System;

class World {
	List<Entity> m_FreeEntities;
	HashSet<Entity, hashEntity> m_LiveEntities;
	Entity m_NextEntity = 0;

	// Component stores
	ComponentStorage<Model> m_ModelStorage;
	ComponentStorage<Transform> m_TransformStorage;
public:
	Camera Camera;
	Environment Environment;

	// Entities
	Entity spawn();
	void despawn(Entity entity);

	// Components
	template <typename T> ComponentStorage<T>& getStorage();

	template <typename T> void addComponent(Entity entity, const T& component) {
		ASSERT(m_LiveEntities.contains(entity));
		getStorage<T>().add(entity, component);
	}

	template <typename T> T* getComponent(Entity entity) {
		ASSERT(m_LiveEntities.contains(entity));
		return getStorage<T>().get(entity);
	}

	template <typename T> void removeComponent(Entity entity) {
		ASSERT(m_LiveEntities.contains(entity));
		getStorage<T>().remove(entity);
	}

	// Systems
	void runSystem(System& system);
};
