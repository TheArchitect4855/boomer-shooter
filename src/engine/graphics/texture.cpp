#include "texture.hpp"

#include <glad/gl.h>
#include "../../common/log.hpp"

Texture::Texture() : m_RefCount{nullptr}, m_Id{0} {}

Texture::Texture(unsigned int id)
: m_RefCount{new int}, m_Id{id}
{
	*m_RefCount = 1;
	TRACE("created texture with ID %d", id);
}

Texture::Texture(const Texture& copy)
: m_RefCount{copy.m_RefCount}, m_Id{copy.m_Id}
{
	if (m_RefCount != nullptr) *m_RefCount += 1;
}

Texture::~Texture() {
	if (m_RefCount == nullptr || --*m_RefCount > 0) return;
	TRACE("deleting texture %d", m_Id);
	glDeleteTextures(1, &m_Id);
	delete m_RefCount;
}

unsigned int Texture::id() const { return m_Id; }

Texture& Texture::operator=(const Texture& copy) {
	TRACE("copying texture %d", copy.m_Id);
	this->~Texture();
	m_RefCount = copy.m_RefCount;
	m_Id = copy.m_Id;
	if (m_RefCount != nullptr) *m_RefCount += 1;
	return *this;
}
