#include "entity.hpp"

unsigned int hashEntity(const Entity& value) {
		constexpr unsigned int salt = 0xcd760f6d;
		return value ^ salt;
}
