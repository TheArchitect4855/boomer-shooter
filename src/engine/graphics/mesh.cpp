#include "mesh.hpp"

#include <glad/gl.h>
#include "../../common/log.hpp"

Mesh::Mesh() : m_RefCount{nullptr}, m_Id{0} {}

Mesh::Mesh(unsigned int id, unsigned int indexCount, unsigned int indexOffset, unsigned int indexType, unsigned int mode)
: m_RefCount{new int}, m_Id{id}, m_IndexCount{indexCount}, m_IndexOffset{indexOffset}, m_IndexType{indexType}, m_Mode{mode}
{
	TRACE("created mesh with ID %d", m_Id);
	*m_RefCount = 1;
}

Mesh::Mesh(const Mesh& copy)
: m_RefCount{copy.m_RefCount}, m_Id{copy.m_Id}, m_IndexCount{copy.m_IndexCount},
	m_IndexOffset{copy.m_IndexOffset}, m_IndexType{copy.m_IndexType},
	m_Mode{copy.m_Mode}, Material{copy.Material}
{
	if (m_RefCount != nullptr) *m_RefCount += 1;
}

Mesh::~Mesh() {
	if (m_RefCount == nullptr || --*m_RefCount > 0) return;
	TRACE("deleting mesh %d", m_Id);
	glDeleteVertexArrays(1, &m_Id);
	delete m_RefCount;
}

void Mesh::draw() const {
	glBindVertexArray(m_Id);
	glDrawElements(m_Mode, m_IndexCount, m_IndexType, (char*)(std::size_t)m_IndexOffset);
}

Mesh& Mesh::operator=(const Mesh& copy) {
	this->~Mesh();
	m_RefCount = copy.m_RefCount;
	m_Id = copy.m_Id;
	m_IndexCount = copy.m_IndexCount;
	m_IndexOffset = copy.m_IndexOffset;
	m_IndexType = copy.m_IndexType;
	m_Mode = copy.m_Mode;
	Material = copy.Material;
	if (m_RefCount != nullptr) *m_RefCount += 1;
	return *this;
}
