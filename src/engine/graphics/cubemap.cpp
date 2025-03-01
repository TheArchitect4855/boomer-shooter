#include "cubemap.hpp"

#include <glad/gl.h>
#include <stb_image.h>

Cubemap::Cubemap(Texture texture) : m_Texture{texture} {}

const Texture& Cubemap::texture() const { return m_Texture; }

// Must be ordered right, left, top, bottom, back, front
Cubemap Cubemap::load(const Array<const char*>& filenames) {
	ASSERT(filenames.length() == 6);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	Texture t = Texture(texture);
	int width, height;
	for (unsigned int i = 0; i < filenames.length(); i += 1) {
		unsigned char* pixels = stbi_load(filenames[i], &width, &height, NULL, 3);
		ASSERT(pixels != nullptr);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		stbi_image_free(pixels);
	}

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	return t;
}
