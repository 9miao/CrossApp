// smart_ptr.h	-- by Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Smart (ref-counting) pointer classes.  Uses "intrusive" approach:
// the types pointed to must have add_ref() and drop_ref() methods.
// Typically this is done by inheriting from a ref_counted class,
// although the nice thing about templates is that no particular
// ref-counted class is mandated.


#ifndef SMART_PTR_H
#define SMART_PTR_H


#include "tu_config.h"
#include "utility.h"


// A smart (strong) pointer asserts that the pointed-to object will
// not go away as long as the strong pointer is valid.  "Owners" of an
// object should keep strong pointers; other objects should use a
// strong pointer temporarily while they are actively using the
// object, to prevent the object from being deleted.
template<class T>
class smart_ptr
{
public:
	smart_ptr(T* ptr)
		:
		m_ptr(ptr)
	{
		if (m_ptr)
		{
			m_ptr->add_ref();
		}
	}

	smart_ptr() : m_ptr(NULL) {}
	smart_ptr(const smart_ptr<T>& s)
		:
		m_ptr(s.m_ptr)
	{
		if (m_ptr)
		{
			m_ptr->add_ref();
		}
	}

	~smart_ptr()
	{
		if (m_ptr)
		{
			m_ptr->drop_ref();
		}
	}

	void	operator=(const smart_ptr<T>& s) { set_ref(s.m_ptr); }
	void	operator=(T* ptr) { set_ref(ptr); }
	T*	operator->() const { assert(m_ptr); return m_ptr; }
	T*	get_ptr() const { return m_ptr; }
	bool	operator==(const smart_ptr<T>& p) const { return m_ptr == p.m_ptr; }
	bool	operator!=(const smart_ptr<T>& p) const { return m_ptr != p.m_ptr; }
	bool	operator==(T* p) const { return m_ptr == p; }
	bool	operator!=(T* p) const { return m_ptr != p; }

	// Provide work-alikes for static_cast, dynamic_cast, implicit up-cast?  ("gentle_cast" a la ajb?)

private:
	void	set_ref(T* ptr)
	{
		if (ptr != m_ptr)
		{
			if (m_ptr)
			{
				m_ptr->drop_ref();
			}
			m_ptr = ptr;

			if (m_ptr)
			{
				m_ptr->add_ref();
			}
		}
	}

	T*	m_ptr;
};

// Example ref_counted class:
// 
#if 0
ref_counted : public weak_pointee_mixin
{
	mutable int	m_ref_count

	ref_counted()
		:
		m_ref_count(0)
	{
	}

	// @@ usual ref-counted stuff here for add_ref() and drop_ref()

	virtual ~ref_counted()
	{
		assert(m_ref_count == 0);
	}
};
#endif // 0


#endif // SMART_PTR_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
