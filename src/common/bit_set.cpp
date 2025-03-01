#include "bit_set.hpp"

#include <cstring>

BitSet::BitSet() : m_Length{0} {
	m_Bytes = Array<unsigned char>();
}

BitSet::BitSet(unsigned int length) : m_Length{length} {
	unsigned int byteCount = (length + 7) / 8;
	m_Bytes = Array<unsigned char>(byteCount);
	std::memset(m_Bytes.elements(), 0, m_Bytes.length());
}

void BitSet::clear(unsigned int index) {
	ASSERT(index < m_Length);
	unsigned int bit = index % 8;
	unsigned int byte = index / 8;
	m_Bytes[byte] &= ~(1 << bit);
}

void BitSet::resize(unsigned int length) {
	unsigned int byteCount = (length + 7) / 8;
	if (byteCount <= m_Bytes.length()) {
		m_Length = length;
		return;
	}

	Array<unsigned char> bytes = Array<unsigned char>(byteCount);
	bytes.copyFrom(m_Bytes);
	memset(bytes.elements() + m_Bytes.length(), 0, bytes.length() - m_Bytes.length());

	m_Bytes = bytes;
	m_Length = length;
}

void BitSet::set(unsigned int index) {
	ASSERT(index < m_Length);
	unsigned int bit = index % 8;
	unsigned int byte = index / 8;
	m_Bytes[byte] |= (1 << bit);
}

unsigned int BitSet::size() const {
	return m_Length;
}

bool BitSet::operator[](unsigned int index) const {
	ASSERT(index < m_Length);
	unsigned int bit = index % 8;
	unsigned int byte = index / 8;
	return (m_Bytes[byte] & (1 << bit)) > 0;
}
