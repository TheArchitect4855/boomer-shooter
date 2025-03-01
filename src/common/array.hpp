#pragma once

#include "assert.hpp"

template <typename T> class Array {
	T* m_Elements;
	unsigned int m_Length;
public:
	Array() : m_Elements{nullptr}, m_Length{0} {}

	explicit Array(unsigned int length) : m_Length{length} {
		if (length > 0) m_Elements = new T[length];
		else m_Elements = nullptr;
	}

	Array(const Array& copy) : m_Length{copy.m_Length} {
		if (m_Length > 0) {
			m_Elements = new T[m_Length];
			for (unsigned int i = 0; i < m_Length; i += 1) m_Elements[i] = copy[i];
		} else m_Elements = nullptr;
	}

	~Array() {
		if (m_Elements != nullptr) delete[] m_Elements;
	}

	void copyFrom(const Array<T>& src) {
		for (unsigned int i = 0; i < m_Length && i < src.m_Length; i += 1) m_Elements[i] = src[i];
	}

	T* elements() const { return m_Elements; }
	unsigned int length() const { return m_Length; }

	Array& operator=(const Array& copy) {
		unsigned int length = copy.length();
		T* elements = nullptr;
		if (length > 0) {
			elements = new T[length];
			for (unsigned int i = 0; i < length; i += 1) elements[i] = copy[i];
		}

		if (m_Elements != nullptr) delete[] m_Elements;
		m_Elements = elements;
		m_Length = length;
		return *this;
	}

	T& operator[](unsigned int index) {
		ASSERT(index < m_Length);
		return m_Elements[index];
	}

	const T& operator[](unsigned int index) const {
		ASSERT(index < m_Length);
		return m_Elements[index];
	}
};
