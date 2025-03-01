#pragma once

#include "array.hpp"

class BitSet {
	Array<unsigned char> m_Bytes;
	unsigned int m_Length;
public:
	BitSet();
	explicit BitSet(unsigned int length);

	void clear(unsigned int index);
	void resize(unsigned int length);
	void set(unsigned int index);

	unsigned int size() const;
	bool operator[](unsigned int index) const;
};
