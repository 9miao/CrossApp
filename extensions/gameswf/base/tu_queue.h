// tu_queue.h	-- Thatcher Ulrich <tu@tulrich.com> 2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Very simple std::queue work-alike.

#ifndef TU_QUEUE_H
#define TU_QUEUE_H

#include "utility.h"

template<class T>
class tu_queue
{
public:

	tu_queue()
	{
		m_size = 0;
		m_back = NULL;
		m_front = NULL;
	}

	~tu_queue()
	{
		clear();
	}

	void clear()
	{
		while (size() > 0)
		{
			pop();
		}
	}

	const T& back() const {
		assert(m_back);
		return m_back->m_value;
	}

	T& back() {
		assert(m_back);
		return m_back->m_value;
	}

	const T& front() const {
		assert(m_front);
		return m_front->m_value;
	}

	T& front() {
		assert(m_front);
		return m_front->m_value;
	}

	void pop() {
		assert(m_front);
		elem* to_delete = m_front;
		m_front = m_front->m_next;
		m_size--;
		delete to_delete;
		if (m_back == to_delete) {
			m_back = NULL;
			assert(m_front == NULL);
			assert(m_size == 0);
		}
	}

	void push(const T& val) {
		if (!m_back) {
			assert(!m_front);
			assert(m_size == 0);
			m_back = new elem(val);
			m_front = m_back;
		} else {
			assert(m_back->m_next == NULL);
			m_back->m_next = new elem(val);
			m_back = m_back->m_next;
		}
		m_size++;
	}

	size_t size() const {
		return m_size;
	}

private:

	struct elem {
		T m_value;
		elem* m_next;

		elem(const T& v) : m_value(v), m_next(NULL) {
		}
	};

	elem* m_back;
	elem* m_front;
	int m_size;
};

#endif // TU_QUEUE_H

