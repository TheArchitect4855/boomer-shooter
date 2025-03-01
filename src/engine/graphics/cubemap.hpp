#pragma once

#include "../../common/array.hpp"
#include "texture.hpp"

class Cubemap {
	Texture m_Texture;
	Cubemap(Texture texture);
public:
	Cubemap() = default;
	const Texture& texture() const;
	static Cubemap load(const Array<const char*>& filenames);
};
