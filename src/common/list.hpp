#pragma once

#include "array.hpp"

template <typename T> class List {
	Array<T> m_Container;
	unsigned int m_Count;
public:
	List() : m_Count{0} {
		m_Container = Array<T>(0);
	}

	explicit List(unsigned int capacity) : m_Count{0} {
		m_Container = Array<T>(capacity);
	}

	void push(const T& value) {
		if (m_Count >= m_Container.length()) {
			unsigned int len = m_Container.length() * 2;
			if (len == 0) len = 8;

			Array<T> container = Array<T>(len);
			for (unsigned int i = 0; i < m_Count; i += 1) container[i] = m_Container[i];
			m_Container = container;
		}

		m_Container[m_Count++] = value;
	}

	unsigned int count() { return m_Count; }

	T pop() {
		ASSERT(m_Count > 0);
		return m_Container[--m_Count];
	}

	Array<T> toArray() {
		Array<T> array = Array<T>(m_Count);
		array.copyFrom(m_Container);
		return array;
	}

	T& operator[](unsigned int index) {
		ASSERT(index < m_Count);
		return m_Container[index];
	}
};
