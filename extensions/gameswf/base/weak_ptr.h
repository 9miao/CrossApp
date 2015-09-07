// weak_ptr.h -- by Thatcher Ulrich <tu@tulrich.com> 2008

// This code has been donated to the Public Domain.  Do whatever you
// want with it.

// Weak pointer; can be combined with smart_ptr<> or gc_ptr<>.


#ifndef WEAK_PTR_H
#define WEAK_PTR_H

#include "smart_ptr.h"

// Helper for making objects that can have weak_ptr's.
class weak_proxy
{
public:
	weak_proxy()
		:
		m_ref_count(0),
		m_alive(true)
	{
	}

	// weak_ptr's call this to determine if their pointer is valid or not.
	bool	is_alive() const { return m_alive; }

	// Only the actual object should call this.
	void	notify_object_died() { m_alive = false; }

	void	add_ref()
	{
		assert(m_ref_count >= 0);
		m_ref_count++;
	}
	void	drop_ref()
	{
		assert(m_ref_count > 0);

		m_ref_count--;
		if (m_ref_count == 0)
		{
			// Now we die.
			delete this;
		}
	}

private:
	// Don't use these.
	weak_proxy(const weak_proxy& w) { assert(0); }
	void	operator=(const weak_proxy& w) { assert(0); }

	int	m_ref_count;
	bool	m_alive;
};


// A weak pointer points at an object, but the object may be deleted
// at any time, in which case the weak pointer automatically becomes
// NULL.  The only way to use a weak pointer is by converting it to a
// strong pointer (i.e. for temporary use).
//
// The class pointed to must have a "weak_proxy* get_weak_proxy()" method.
//
// Usage idiom:
//
// if (smart_ptr<my_type> ptr = m_weak_ptr_to_my_type) { ... use ptr->whatever() safely in here ... }

namespace gameswf
{
	template<class T>
	class weak_ptr
	{
	public:
		weak_ptr()
			:
			m_ptr(0)
		{
		}

		weak_ptr(T* ptr)
			:
			m_ptr(0)
		{
			operator=(ptr);
		}

		weak_ptr(const smart_ptr<T>& ptr)
		{
			operator=(ptr.get_ptr());
		}

		// Default constructor and assignment from weak_ptr<T> are OK.

		void	operator=(T* ptr)
		{
			m_ptr = ptr;
			if (m_ptr)
			{
				m_proxy = m_ptr->get_weak_proxy();
				assert(m_proxy != NULL);
				assert(m_proxy->is_alive());
			}
			else
			{
				m_proxy = NULL;
			}
		}

		void	operator=(const smart_ptr<T>& ptr) { operator=(ptr.get_ptr()); }

		bool	operator==(const smart_ptr<T>& ptr) const
		{
			check_proxy();
			return m_ptr == ptr.get_ptr();
		}

		bool	operator!=(const smart_ptr<T>& ptr) const
		{
			check_proxy();
			return m_ptr != ptr.get_ptr();
		}

		bool	operator==(T* ptr) const
		{
			check_proxy();
			return m_ptr == ptr;
		}

		bool	operator!=(T* ptr) const
		{
			check_proxy();
			return m_ptr != ptr;
		}

		T*	operator->() const
		{
			check_proxy();
			assert(m_ptr);
			return m_ptr;
		}

		T*	get_ptr() const
		{
			check_proxy();
			return m_ptr;
		}

		// Conversion to smart_ptr.
		operator smart_ptr<T>()
		{
			check_proxy();
			return smart_ptr<T>(m_ptr);
		}

		bool	operator==(T* ptr) { check_proxy(); return m_ptr == ptr; }
		bool	operator==(const smart_ptr<T>& ptr) { check_proxy(); return m_ptr == ptr.get_ptr(); }

		// for hash< weak_ptr<...>, ...>
		bool	operator==(const weak_ptr<T>& ptr) const
		{
			check_proxy();
			ptr.check_proxy();
			return m_ptr == ptr.m_ptr;
		}

	private:
		void check_proxy() const
			// Set m_ptr to NULL if the object died.
		{
			if (m_ptr)
			{
				assert(m_proxy != NULL);
				if (m_proxy->is_alive() == false)
				{
					// Underlying object went away.
					m_proxy = NULL;
					m_ptr = NULL;
				}
			}
		}

		mutable smart_ptr<weak_proxy>	m_proxy;
		mutable T*	m_ptr;
	};
}

// Mix this into classes whose instances you want to point at using
// weak_ptr<>'s.
class weak_pointee_mixin
{
public:
	weak_pointee_mixin() : m_weak_proxy(0)
	{
	}

	virtual ~weak_pointee_mixin()
	{
		if (m_weak_proxy) {
			m_weak_proxy->notify_object_died();
			m_weak_proxy->drop_ref();
			m_weak_proxy = NULL;
		}
	}

	weak_proxy* get_weak_proxy() const
	{
		if (m_weak_proxy == NULL)
		{
			m_weak_proxy = new weak_proxy;
			m_weak_proxy->add_ref();
		}
		return m_weak_proxy;
	}
	
private:
	mutable weak_proxy* m_weak_proxy;
};


#endif  // WEAK_PTR_H
