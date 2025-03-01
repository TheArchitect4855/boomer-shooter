#pragma once

#include "array.hpp"
#include "bit_set.hpp"

#define LAST_PRIME 1361
static const unsigned int primes[] = {7, 17, 37, 79, 163, 331, 673, 1361};

// TODO: Max filled percent

template <typename T, unsigned int (*H)(const T& value)> class HashSet {
	Array<T> m_Container;
	BitSet m_Occupied;
	unsigned int m_Count = 0;

	void addItem(const T& value) {
		ASSERT(m_Count <= m_Container.length());

		unsigned int i = getIndex(value);
		while (m_Occupied[i]) i = (i + 1) % m_Container.length();
		m_Occupied.set(i);
		m_Container[i] = value;
	}

	unsigned int getIndex(const T& value) const {
		return H(value) % m_Container.length();
	}
public:
	void add(const T& value) {
		unsigned int cap = m_Container.length();
		if (m_Count >= cap) {
			if (cap == 0) cap = primes[0];
			else if (cap >= LAST_PRIME) cap = cap * 2 + 1;
			else {
				unsigned int i = 0;
				while (primes[i] != cap) i += 1;
				cap = primes[i + 1];
			}

			Array<T> container = m_Container;
			BitSet occupied = m_Occupied;
			m_Container = Array<T>(cap);
			m_Occupied = BitSet(cap);
			for (unsigned int i = 0; i < container.length(); i += 1) {
				if (occupied[i]) addItem(container[i]);
			}
		}

		m_Count += 1;
		addItem(value);
	}

	bool contains(const T& value) const {
		if (m_Count == 0) return false;

		unsigned int start = getIndex(value);
		unsigned int i = start;
		do {
			if (m_Occupied[i] && m_Container[i] == value) return true;
			i = (i + 1) % m_Container.length();
		} while (i != start);
		return false;
	}

	unsigned int count() const { return m_Count; }

	void intersect(const HashSet<T, H>& other) {
		for (unsigned int i = 0; i < m_Occupied.size(); i += 1) {
			if (!m_Occupied[i]) continue;
			const T& value = m_Container[i];
			if (!other.contains(value)) remove(value);
		}
	}

	bool remove(const T& value) {
		if (m_Count == 0) return false;

		unsigned int start = getIndex(value);
		unsigned int i = start;
		do {
			if (m_Occupied[i] && m_Container[i] == value) {
				m_Occupied.clear(i);
				m_Count -= 1;
				return true;
			}

			i = (i + 1) % m_Container.length();
		} while (i != start);
		return false;
	}

	Array<T> toArray() {
		Array<T> array = Array<T>(m_Count);
		unsigned int i = 0;
		for (unsigned int j = 0; j < m_Container.length(); j += 1) {
			if (!m_Occupied[j]) continue;
			array[i++] = m_Container[j];
		}

		return array;
	}
};
