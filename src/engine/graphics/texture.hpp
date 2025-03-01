#pragma once

class Texture {
	int* m_RefCount;
	unsigned int m_Id;
public:
	Texture();
	explicit Texture(unsigned int id);
	Texture(const Texture& copy);
	~Texture();

	unsigned int id() const;

	Texture& operator=(const Texture& copy);
};
