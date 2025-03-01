#pragma once

#include "entity_iter.hpp"
#include "world.hpp"

class System {
public:
	virtual void run(World& world, EntityIter iter) = 0;
};
