#pragma once

#include "material.hpp"

class Mesh {
	int* m_RefCount;
	unsigned int m_Id;
	unsigned int m_IndexCount;
	unsigned int m_IndexOffset;
	unsigned int m_IndexType;
	unsigned int m_Mode;
public:
	Material Material;

	Mesh();
	Mesh(unsigned int id, unsigned int indexCount, unsigned int indexOffset, unsigned int indexType, unsigned int mode);
	Mesh(const Mesh& copy);
	~Mesh();

	void draw() const;

	Mesh& operator=(const Mesh& copy);
};
