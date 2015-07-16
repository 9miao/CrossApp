// container.h	-- Thatcher Ulrich <tu@tulrich.com> 31 July 2001

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Generic C++ containers.  Problem: STL is murder on compile times,
// and is hard to debug.  These are substitutes that compile much
// faster and are somewhat easier to debug.  Not as featureful,
// efficient or hammered-on as STL though.  You can use STL
// implementations if you want; see _TU_USE_STL.

#ifndef CONTAINER_H
#define CONTAINER_H

#include "tu_config.h"
#include "utility.h"
#include <stdlib.h>
#include <string.h>	// for strcmp and friends
#include <new>	// for placement new

// If you prefer STL implementations of array<> (i.e. std::vector) and
// hash<> (i.e. std::hash_map) instead of home cooking, then put
// -D_TU_USE_STL=1 in your compiler flags, or do it in tu_config.h, or do
// it right here:
//#define _TU_USE_STL 1



template<class T>
class fixed_size_hash
// Computes a hash of an object's representation.
{
public:
	long unsigned int	operator()(const T& data) const
	{
		unsigned char*	p = (unsigned char*) &data;
		int	size = sizeof(T);
        
		return sdbm_hash(p, size);
	}
};


template<class T>
class identity_hash
// Hash is just the input value; can use this for integer-indexed hash tables.
{
public:
	long unsigned int	operator()(const T& data) const
	{
		return (long unsigned int) data;
	}
};


#if _TU_USE_STL == 1


//
// Thin wrappers around STL
//


//// @@@ crap compatibility crap
//#define StlAlloc(size) malloc(size)
//#define StlFree(ptr, size) free(ptr)


#include <vector>
#include <hash_map>
#include <string>


// array<> is much like std::vector<>
//
// @@ move this towards a strict subset of std::vector ?  Compatibility is good.
template<class T> class array : public std::vector<T>
{
public:
	int	size() const { return (int) std::vector<T>::size(); }

	void	append(const array<T>& other)
	// Append the given data to our array.
	{
		std::vector<T>::insert(end(), other.begin(), other.end());
	}

	void	append(const T other[], int count)
	{
		// This will probably work.  Depends on std::vector<T>::iterator being typedef'd as T*
		std::vector<T>::insert(end(), &other[0], &other[count]);
	}

	void	remove(int index)
	{
		std::vector<T>::erase(begin() + index);
	}

	void	insert(int index, const T& val = T())
	{
		std::vector<T>::insert(begin() + index, val);
	}

	void	release()
	{
		// Drop all storage.
		std::vector<T>	temp;
		this->swap(temp);
	}
};


// hash<> is similar to std::hash_map<>
//
// @@ move this towards a strict subset of std::hash_map<> ?
template<class T, class U, class hash_functor = fixed_size_hash<T> >
class hash : public stdext::hash_map<T, U, hash_functor>
{
public:
	// extra convenience interfaces
	void	set(const T& key, const U& value)
	// Set a new or existing value under the key, to the value.
	{
		(*this)[key] = value;
	}

	void	add(const T& key, const U& value)
	{
		assert(find(key) == end());
		(*this)[key] = value;
	}

	bool	is_empty() const { return empty(); }

	bool	get(const T& key, U* value) const
	// Retrieve the value under the given key.
	//
	// If there's no value under the key, then return false and leave
	// *value alone.
	//
	// If there is a value, return true, and set *value to the entry's
	// value.
	//
	// If value == NULL, return true or false according to the
	// presence of the key, but don't touch *value.
	{
		const_iterator	it = find(key);
		if (it != end())
		{
			if (value) *value = it->second;
			return true;
		}
		else
		{
			return false;
		}
	}
};


// // tu_string is a subset of std::string, for the most part
// class tu_string : public std::string
// {
// public:
// 	tu_string(const char* str) : std::string(str) {}
// 	tu_string() : std::string() {}
// 	tu_string(const tu_string& str) : std::string(str) {}

// 	int	length() const { return (int) std::string::length(); }
// };


// template<class U>
// class string_hash : public hash<tu_string, U, std::hash<std::string> >
// {
// };


#else // not _TU_USE_STL


//
// Homemade containers; almost strict subsets of STL.
//


#ifdef _WIN32
#pragma warning(disable : 4345)	// in MSVC 7.1, warning about placement new POD default initializer
#endif // _WIN32




template<class T>
class array {
// Resizable array.  Don't put anything in here that can't be moved
// around by bitwise copy.  Don't keep the address of an element; the
// array contents will move around as it gets resized.
//
// Default constructor and destructor get called on the elements as
// they are added or removed from the active part of the array.
public:
	typedef T value_type;

	array() : m_buffer(0), m_size(0), m_buffer_size(0) {}
	array(int size_hint) : m_buffer(0), m_size(0), m_buffer_size(0) { resize(size_hint); }
	array(const array<T>& a)
		:
		m_buffer(0),
		m_size(0),
		m_buffer_size(0)
	{
		operator=(a);
	}
	~array() {
		clear();
	}

	// Basic array access.
	T&	operator[](int index) { assert(index >= 0 && index < m_size); return m_buffer[index]; }
	const T&	operator[](int index) const { assert(index >= 0 && index < m_size); return m_buffer[index]; }
	int	size() const { return m_size; }

	void	push_back(const T& val)
	// Insert the given element at the end of the array.
	{
		// DO NOT pass elements of your own vector into
		// push_back()!  Since we're using references,
		// resize() may munge the element storage!
		// this is irrelevant to MAC OS !!!
//		assert(&val < &m_buffer[0] || &val > &m_buffer[m_buffer_size]);

		int	new_size = m_size + 1;
		resize(new_size);
		(*this)[new_size-1] = val;
	}

	void	pop_back()
	// Remove the last element.
	{
		assert(m_size > 0);
		resize(m_size - 1);
	}

	// Access the first element.
	T&	front() { return (*this)[0]; }
	const T&	front() const { return (*this)[0]; }

	// Access the last element.
	T&	back() { return (*this)[m_size-1]; }
	const T&	back() const { return (*this)[m_size-1]; }

	void	clear()
	// Empty and destruct all elements.
	{
		resize(0);
	}

	void	operator=(const array<T>& a)
	// Array copy.  Copies the contents of a into this array.
	{
		resize(a.size());
		for (int i = 0; i < m_size; i++) {
			*(m_buffer + i) = a[i];
		}
	}


	void	remove(int index)
	// Removing an element from the array is an expensive operation!
	// It compacts only after removing the last element.
	{
		assert(index >= 0 && index < m_size);

		if (m_size == 1)
		{
			clear();
		}
		else
		{
			m_buffer[index].~T();	// destructor

			memmove(m_buffer+index, m_buffer+index+1, sizeof(T) * (m_size - 1 - index));
			m_size--;
		}
	}


	void	insert(int index, const T& val = T())
	// Insert the given object at the given index shifting all the elements up.
	{
		assert(index >= 0 && index <= m_size);

		resize(m_size + 1);

		if (index < m_size - 1)
		{
			// is it safe to use memmove?
			memmove(m_buffer+index+1, m_buffer+index, sizeof(T) * (m_size - 1 - index));
		}

		// Copy-construct into the newly opened slot.
		new (m_buffer + index) T(val);
	}


	void	append(const array<T>& other)
	// Append the given data to our array.
	{
		append(other.m_buffer, other.size());
	}


	void	append(const T other[], int count)
	// Append the given data to our array.
	{
		if (count > 0)
		{
			int	size0 = m_size;
			resize(m_size + count);
			// Must use operator=() to copy elements, in case of side effects (e.g. ref-counting).
			for (int i = 0; i < count; i++)
			{
				m_buffer[i + size0] = other[i];
			}
		}
	}


	void	resize(int new_size)
	// Preserve existing elements via realloc.
	// 
	// Newly created elements are initialized with default element
	// of T.  Removed elements are destructed.
	{
		assert(new_size >= 0);

		int	old_size = m_size;

		// Destruct old elements (if we're shrinking).
		{for (int i = new_size; i < old_size; i++) {
			(m_buffer + i)->~T();
		}}

		if (new_size == 0) {
			m_buffer_size = 0;
			reserve(0);
		} else if (new_size <= m_buffer_size && new_size > m_buffer_size >> 1) {
			// don't compact yet.
			assert(m_buffer != 0);
		} else {
			int	new_buffer_size = new_size + (new_size >> 1);
			reserve(new_buffer_size);
		}

		// Copy default T into new elements.
		{for (int i = old_size; i < new_size; i++) {
			new (m_buffer + i) T();	// placement new
		}}

		m_size = new_size;
	}

	void	reserve(int rsize)
	// @@ TODO change this to use ctor, dtor, and operator=
	// instead of preserving existing elements via binary copy via
	// realloc?
	{
		assert(m_size >= 0);

		int	old_size = m_buffer_size;
		old_size = old_size;	// don't warn that this is unused.
		m_buffer_size = rsize;

		// Resize the buffer.
		if (m_buffer_size == 0) {
			if (m_buffer) {
				tu_free(m_buffer, sizeof(T) * old_size);
			}
			m_buffer = 0;
		} else {
			if (m_buffer) {
				m_buffer = (T*) tu_realloc(m_buffer, sizeof(T) * m_buffer_size, sizeof(T) * old_size);
			} else {
				m_buffer = (T*) tu_malloc(sizeof(T) * m_buffer_size);
				memset(m_buffer, 0, (sizeof(T) * m_buffer_size));
			}
			assert(m_buffer);	// need to throw (or something) on malloc failure!
		}			
	}

	void	transfer_members(array<T>* a)
	// UNSAFE!  Low-level utility function: replace this array's
	// members with a's members.
	{
		m_buffer = a->m_buffer;
		m_size = a->m_size;
		m_buffer_size = a->m_buffer_size;

		a->m_buffer = 0;
		a->m_size = 0;
		a->m_buffer_size = 0;
	}

	// Iterator API, for STL compatibility.
	typedef T* iterator;
	typedef const T* const_iterator;

	iterator begin() {
		return m_buffer;
	}
	iterator end() {
		return m_buffer + m_size;
	}
	const_iterator begin() const {
		return m_buffer;
	}
	const_iterator end() const {
		return m_buffer + m_size;
	}

private:
	T*	m_buffer;
	int	m_size;
	int	m_buffer_size;
};

// there are problem with STL
#if 0
template<class T>
class shared_array {
	// Resizable array.  Don't put anything in here that can't be moved
	// around by bitwise copy.  Don't keep the address of an element; the
	// array contents will move around as it gets resized.
	//
	// Default constructor and destructor get called on the elements as
	// they are added or removed from the active part of the array.
public:
	shared_array() : m_buffer(0), m_size(0), m_buffer_size(0) {}
	shared_array(int size_hint) : m_buffer(0), m_size(0), m_buffer_size(0) { resize(size_hint); }
	shared_array(const shared_array<T>& a)
		:
		m_buffer(0),
		m_buffer(0),
		m_size(0),
		m_buffer_size(0)
	{
		operator=(a);
	}
	~shared_array() {
		clear();
	}

	// Basic array access.
	T&	operator[](int index) { assert(index >= 0 && index < m_size); create_own_copy(); return m_buffer[index]; }
	const T&	operator[](int index) const { assert(index >= 0 && index < m_size); return m_buffer[index]; }
	int	size() const { return m_size; }

	void	push_back(const T& val)
	// Insert the given element at the end of the array.
	{
		// DO NOT pass elements of your own vector into
		// push_back()!  Since we're using references,
		// resize() may munge the element storage!
		assert(&val < &m_buffer[0] || &val > &m_buffer[m_buffer_size]);

		int	new_size = m_size + 1;
		resize(new_size);
		(*this)[new_size-1] = val;
	}

	void	pop_back()
		// Remove the last element.
	{
		assert(m_size > 0);
		resize(m_size - 1);
	}

	// Access the first element.
	T&	front() { create_own_copy();return (*this)[0]; }
	const T&	front() const { return (*this)[0]; }

	// Access the last element.
	T&	back() { create_own_copy(); return (*this)[m_size-1]; }
	const T&	back() const { return (*this)[m_size-1]; }

	void	clear()
	// Empty and destruct all elements.
	{
		resize( 0 );
	}

	void	operator=(const shared_array<T>& a)
	// Array copy.  Copies the contents of a into this array.
	{

		m_buffer = a.m_buffer;
		m_size = a.m_size;
		m_buffer_size = a.m_buffer_size;
		
		int * counter;
		counter = reinterpret_cast<int*>( m_buffer ) - 1;

		(*counter)++;
	}


	void	remove(int index)
	// Removing an element from the array is an expensive operation!
	// It compacts only after removing the last element.
	{
		create_own_copy();
		assert(index >= 0 && index < m_size);

		if (m_size == 1)
		{
			clear();
		}
		else
		{
			m_buffer[index].~T();	// destructor

			memmove(m_buffer+index, m_buffer+index+1, sizeof(T) * (m_size - 1 - index));
			m_size--;
		}
	}


	void	insert(int index, const T& val = T())
	// Insert the given object at the given index shifting all the elements up.
	{
		create_own_copy();
		assert(index >= 0 && index <= m_size);

		resize(m_size + 1);

		if (index < m_size - 1)
		{
			// is it safe to use memmove?
			memmove(m_buffer+index+1, m_buffer+index, sizeof(T) * (m_size - 1 - index));
		}

		// Copy-construct into the newly opened slot.
		new (m_buffer + index) T(val);
	}


	void	append(const shared_array<T>& other)
	// Append the given data to our array.
	{
		append(other.m_buffer, other.size());
	}


	void	append(const T other[], int count)
	// Append the given data to our array.
	{
		create_own_copy();
		if (count > 0)
		{
			int	size0 = m_size;
			resize(m_size + count);
			// Must use operator=() to copy elements, in case of side effects (e.g. ref-counting).
			for (int i = 0; i < count; i++)
			{
				m_buffer[i + size0] = other[i];
			}
		}
	}


	void	resize(int new_size)
	// Preserve existing elements via realloc.
	// 
	// Newly created elements are initialized with default element
	// of T.  Removed elements are destructed.
	{
		private_resize( new_size );
	}

	void	reserve(int rsize)
		// @@ TODO change this to use ctor, dtor, and operator=
		// instead of preserving existing elements via binary copy via
		// realloc?
	{
		assert(m_size >= 0);
		assert( owns_buffer() );

		int	old_size = m_buffer_size;
		old_size = old_size;	// don't warn that this is unused.
		m_buffer_size = rsize;

		// Resize the buffer.
		if (m_buffer_size == 0) {
			if (m_buffer) {
				tu_free(get_buffer(), sizeof(T) * old_size + sizeof(int));
			}
			m_buffer = 0;
		} else {
			if (m_buffer) {
				set_buffer( tu_realloc( get_buffer(), sizeof(T) * m_buffer_size + sizeof(int), sizeof(T) * old_size  + sizeof(int) ) );
			} else {
				set_buffer( tu_malloc(sizeof(T) * m_buffer_size + sizeof(int)) );
				memset(m_buffer, 0, (sizeof(T) * m_buffer_size));
			}
			assert(m_buffer);	// need to throw (or something) on malloc failure!

			*( reinterpret_cast<int*>( m_buffer ) - 1 ) = 1;
		}			
	}

private:

	inline void *get_buffer() 
	{
		return reinterpret_cast<int*>( m_buffer ) - 1;
	}

	inline void set_buffer( void * buffer ) 
	{
		m_buffer = reinterpret_cast<T*>( reinterpret_cast<int*>( buffer ) + 1 );
	}

	bool	owns_buffer()
	{
		return m_buffer == NULL || ( *( reinterpret_cast<int*>( m_buffer ) - 1 ) == 1 );
	}

	void	create_own_copy()
	{
		if( m_buffer )
		{
			int * counter;

			counter = reinterpret_cast<int*>( m_buffer ) - 1;
			assert( *counter != 0 );

			if( *counter != 1 )
			{
				T*	old_buffer;
				int	old_size;

				old_buffer = m_buffer;
				old_size = m_size;			

				m_buffer = 0;
				m_size = 0;
				m_buffer_size = 0;

				private_resize( old_size );

				// copy
				{for (int i = 0; i < old_size; i++) {
					m_buffer[i] = old_buffer[ i ];
				}}

				( *counter )--;
			}
		}
	}

	void	private_resize(int new_size)
		// Preserve existing elements via realloc.
		// 
		// Newly created elements are initialized with default element
		// of T.  Removed elements are destructed.
	{
		assert( new_size >= 0 );

		if( !owns_buffer() )
		{
			if( new_size )
			{
				create_own_copy();
				private_resize( new_size );
			}
			else
			{
				int * counter;

				counter = reinterpret_cast<int*>( m_buffer ) - 1;

				( *counter )--;

				m_buffer = 0;
				m_buffer_size = 0;
				m_size = 0;
			}
		}
		else
		{
			int	old_size = m_size;
			m_size = new_size;

			// Destruct old elements (if we're shrinking).
			{for (int i = new_size; i < old_size; i++) {
				(m_buffer + i)->~T();
			}}

			if (new_size == 0) {
				m_buffer_size = 0;
				reserve(m_buffer_size);
			} else if (m_size <= m_buffer_size && m_size > m_buffer_size >> 1) {
				// don't compact yet.
				assert(m_buffer != 0);
			} else {
				int	new_buffer_size = m_size + (m_size >> 2);
				reserve(new_buffer_size);
			}

			// Copy default T into new elements.
			{for (int i = old_size; i < new_size; i++) {
				new (m_buffer + i) T();	// placement new
			}}
		}
	}

	T*	m_buffer;
	int	m_size;
	int	m_buffer_size;
};
#endif
namespace gameswf
{
	template<class T, class U, class hash_functor = fixed_size_hash<T> >

	class hash {
		// Hash table, linear probing, internal chaining.  One
		// interesting/nice thing about this implementation is that the table
		// itself is a flat chunk of memory containing no pointers, only
		// relative indices.  If the key and value types of the hash contain
		// no pointers, then the hash can be serialized using raw IO.  Could
		// come in handy.
		//
		// Never shrinks, unless you explicitly clear() it.  Expands on
		// demand, though.  For best results, if you know roughly how big your
		// table will be, default it to that size when you create it.
	public:
		hash() : m_table(NULL) { }
		hash(int size_hint) : m_table(NULL) { set_capacity(size_hint); }
		~hash() { clear(); }

		hash(const hash<T, U, hash_functor>& src)
			:
			m_table(NULL)
		{
			*this = src;
		}

		void	operator=(const hash<T, U, hash_functor>& src)
		{
			clear();
			if (src.is_empty() == false)
			{
				set_capacity(src.size());

				for (const_iterator it = src.begin(); it != src.end(); ++it)
				{
					add(it->first, it->second);
				}
			}
		}

		// For convenience
		U&	operator[](const T& key)
		{
			int	index = find_index(key);
			if (index >= 0)
			{
				return E(index).second;
			}
			add(key, (U)0);
			index = find_index(key);
			if (index >= 0)
			{
				return E(index).second;
			}

			// Doesn't look nice but removes
			// warning on non-void function not returning.
			assert(0);
			return E(index).second;
		}

		void	set(const T& key, const U& value)
			// Set a new or existing value under the key, to the value.
		{
			int	index = find_index(key);
			if (index >= 0)
			{
				E(index).second = value;
				return;
			}

			// Entry under key doesn't exist.
			add(key, value);
		}

		void	add(const T& key, const U& value)
			// Add a new value to the hash table, under the specified key.
		{
			assert(find_index(key) == -1);

			check_expand();
			assert(m_table);
			m_table->m_entry_count++;

			long unsigned int	hash_value = (long unsigned int)compute_hash(key);
			int	index = hash_value & m_table->m_size_mask;

			entry*	natural_entry = &(E(index));

			if (natural_entry->is_empty()) {
				// Put the new entry in.
				new (natural_entry)entry(key, value, -1, hash_value);
			}
			else if (natural_entry->is_tombstone()) {
				// Put the new entry in, without disturbing the rest of the chain.
				int next_in_chain = natural_entry->m_next_in_chain;
				new (natural_entry)entry(key, value, next_in_chain, hash_value);
			}
			else {
				// Find a blank spot.
				int	blank_index = index;
				for (;;)
				{
					blank_index = (blank_index + 1) & m_table->m_size_mask;
					if (E(blank_index).is_empty()) break;	// found it
				}
				entry*	blank_entry = &E(blank_index);

				if (int(natural_entry->m_hash_value & m_table->m_size_mask) == index)
				{
					// Collision.  Link into this chain.

					// Move existing list head.
					// @@ this could invalidate an existing iterator
					new (blank_entry)entry(*natural_entry);	// placement new, copy ctor

					// Put the new info in the natural entry.
					natural_entry->first = key;
					natural_entry->second = value;
					natural_entry->m_next_in_chain = blank_index;
					natural_entry->m_hash_value = hash_value;
				}
				else
				{
					// Existing entry does not naturally
					// belong in this slot.  Existing
					// entry must be moved.

					// Find natural location of collided element (i.e. root of chain)
					int	collided_index = (int)(natural_entry->m_hash_value & m_table->m_size_mask);
					for (;;)
					{
						entry*	e = &E(collided_index);
						if (e->m_next_in_chain == index)
						{
							// Here's where we need to splice.
							// @@ this could invalidate an existing iterator
							new (blank_entry)entry(*natural_entry);
							e->m_next_in_chain = blank_index;
							break;
						}
						collided_index = e->m_next_in_chain;
						assert(collided_index >= 0 && collided_index <= m_table->m_size_mask);
					}

					// Put the new data in the natural entry.
					natural_entry->first = key;
					natural_entry->second = value;
					natural_entry->m_hash_value = hash_value;
					natural_entry->m_next_in_chain = -1;
				}
			}
		}

		void	clear()
			// Remove all entries from the hash table.
		{
			if (m_table)
			{
				// Delete the entries.
				for (int i = 0, n = m_table->m_size_mask; i <= n; i++)
				{
					entry*	e = &E(i);
					if (e->is_empty() == false && e->is_tombstone() == false)
					{
						e->clear();
					}
				}
				tu_free(m_table, sizeof(table)+sizeof(entry)* (m_table->m_size_mask + 1));
				m_table = NULL;
			}
		}

		bool	is_empty() const
			// Returns true if the hash is empty.
		{
			return m_table == NULL || m_table->m_entry_count == 0;
		}


		bool	get(const T& key, U* value) const
			// Retrieve the value under the given key.
			//
			// If there's no value under the key, then return false and leave
			// *value alone.
			//
			// If there is a value, return true, and set *value to the entry's
			// value.
			//
			// If value == NULL, return true or false according to the
			// presence of the key, but don't touch *value.
		{
            int	index = find_index(key);
			if (index >= 0)
			{
				if (value) {
					*value = E(index).second;
				}
				return true;
			}
			return false;
		}


		int	size() const
		{
			return m_table == NULL ? 0 : m_table->m_entry_count;
		}


		void	check_expand()
			// Resize the hash table to fit one more entry.  Often this
			// doesn't involve any action.
		{
			if (m_table == NULL) {
				// Initial creation of table.  Make a minimum-sized table.
				set_raw_capacity(32);
			}
			else if (m_table->m_entry_count * 3 > (m_table->m_size_mask + 1) * 2) {
				// Table is more than 2/3rds full.  Expand.
				set_raw_capacity((m_table->m_size_mask + 1) * 2);
			}
		}

		void check_shrink()
			// Shrink our capacity, if it makes sense.
		{
			if (m_table) {
				if (size() * 3 < m_table->m_size_mask + 1) {
					// Table is less than 1/3 full.  Shrink.
					set_capacity(size());
				}
			}
		}

		void	resize(long unsigned int n)
			// Hint the bucket count to >= n.
		{
			// Not really sure what this means in relation to
			// STLport's hash_map... they say they "increase the
			// bucket count to at least n" -- but does that mean
			// their real capacity after resize(n) is more like
			// n*2 (since they do linked-list chaining within
			// buckets?).
			set_capacity(n);
		}

		void	set_capacity(int new_size)
			// Size the hash so that it can comfortably contain the given
			// number of elements.  If the hash already contains more
			// elements than new_size, then this may be a no-op.
		{
			if (new_size < size()) {
				// Make sure requested size is large enough to
				// contain existing elements!
				new_size = size();
			}
			int	new_raw_size = (new_size * 3) / 2;
			set_raw_capacity(new_raw_size);
		}

		// Behaves much like std::pair
		struct entry
		{
			int	m_next_in_chain;	// internal chaining for collisions
			long unsigned int	m_hash_value;		// avoids recomputing.  Worthwhile?
			T	first;
			U	second;

			entry() : m_next_in_chain(-2) {}
			entry(const entry& e)
				: m_next_in_chain(e.m_next_in_chain), m_hash_value(e.m_hash_value), first(e.first), second(e.second)
			{
			}
			entry(const T& key, const U& value, int next_in_chain, long unsigned int hash_value)
				: m_next_in_chain(next_in_chain), m_hash_value(hash_value), first(key), second(value)
			{
			}
			bool is_empty() const { return m_next_in_chain == -2; }
			bool is_end_of_chain() const { return m_next_in_chain == -1; }
			bool is_tombstone() const { return m_hash_value == TOMBSTONE_HASH; }

			void	clear()
			{
				first.~T();	// placement delete
				second.~U();	// placement delete
				m_next_in_chain = -2;
				m_hash_value = ~TOMBSTONE_HASH;
			}

			void make_tombstone() {
				first.~T();
				second.~U();
				m_hash_value = TOMBSTONE_HASH;
			}
		};

		// Iterator API, like STL.

		class const_iterator
		{
		public:
			T	get_key() const { return m_hash->E(m_index).first; }
			U	get_value() const { return m_hash->E(m_index).second; }

			const entry&	operator*() const
			{
				assert(is_end() == false && m_hash->E(m_index).is_empty() == false);
				assert(m_hash->E(m_index).is_tombstone() == false);
				return m_hash->E(m_index);
			}
			const entry*	operator->() const { return &(operator*()); }

			void	operator++()
			{
				assert(m_hash);

				// Find next non-empty entry.
				if (m_index <= m_hash->m_table->m_size_mask)
				{
					m_index++;
					while (m_index <= m_hash->m_table->m_size_mask
						&& (m_hash->E(m_index).is_empty() || m_hash->E(m_index).is_tombstone()))
					{
						m_index++;
					}
				}
			}

			bool	operator==(const const_iterator& it) const
			{
				if (is_end() && it.is_end())
				{
					return true;
				}
				else
				{
					return
						m_hash == it.m_hash
						&& m_index == it.m_index;
				}
			}

			bool	operator!=(const const_iterator& it) const { return !(*this == it); }


			bool	is_end() const
			{
				return
					m_hash == NULL
					|| m_hash->m_table == NULL
					|| m_index > m_hash->m_table->m_size_mask;
			}

		protected:
			friend class hash<T, U, hash_functor>;

			const_iterator(const hash* h, int index)
				:
				m_hash(h),
				m_index(index)
			{
			}

			const hash* m_hash;
			int m_index;
		};
		friend class const_iterator;

		// non-const iterator; get most of it from const_iterator.
		class iterator : public const_iterator
		{
		public:
			// Allow non-const access to entries.
			entry&	operator*() const
			{
				assert(const_iterator::is_end() == false);
				return const_cast<hash*>(const_iterator::m_hash)->E(const_iterator::m_index);
			}
			entry*	operator->() const { return &(operator*()); }

		private:
			friend class hash<T, U, hash_functor>;

			iterator(hash* h, int i0)
				:
				const_iterator(h, i0)
			{
			}
		};
		friend class iterator;

		void erase(const iterator& pos)
			// Removes the element at pos.
		{
			if (pos.is_end() || pos.m_hash != this) {
				// Invalid iterator.
				return;
			}
			assert(m_table);

			int natural_index = (int)(pos->m_hash_value & m_table->m_size_mask);

			if (pos.m_index != natural_index) {
				// We're not the head of our chain, so we can
				// be spliced out of it.

				// Iterate up the chain, and splice out when
				// we get to m_index.
				entry* e = &E(natural_index);
				while (e->m_next_in_chain != pos.m_index) {
					assert(e->is_end_of_chain() == false);
					e = &E(e->m_next_in_chain);
				}
				e->m_next_in_chain = pos->m_next_in_chain;
				pos->clear();
			}
			else if (pos->is_end_of_chain() == false) {
				// We're the head of our chain, and there are
				// additional elements.
				//
				// We need to put a tombstone here.
				//
				// We can't clear the element, because the
				// rest of the elements in the chain must be
				// linked to this position.
				//
				// We can't move any of the succeeding
				// elements in the chain (i.e. to fill this
				// entry), because we don't want to invalidate
				// any other existing iterators.
				pos->make_tombstone();
			}
			else {
				// We're the head of the chain, but we're the
				// only member of the chain.
				pos->clear();
			}

			m_table->m_entry_count--;
		}

		void erase(const T& key)
			// Removes the element with the given key (if any).
		{
			iterator it = find(key);
			if (it != end()) {
				erase(it);
			}
		}

		iterator	begin()
		{
			if (m_table == 0) return iterator(NULL, 0);

			// Scan til we hit the first valid entry.
			int	i0 = 0;
			while (i0 <= m_table->m_size_mask
				&& (E(i0).is_empty() || E(i0).is_tombstone()))
			{
				i0++;
			}
			return iterator(this, i0);
		}
		iterator	end() { return iterator(NULL, 0); }

		const_iterator	begin() const { return const_cast<hash*>(this)->begin(); }
		const_iterator	end() const { return const_cast<hash*>(this)->end(); }

		iterator	find(const T& key)
		{
			int	index = find_index(key);
			if (index >= 0)
			{
				return iterator(this, index);
			}
			return iterator(NULL, 0);
		}

		const_iterator	find(const T& key) const { return const_cast<hash*>(this)->find(key); }

	private:
		// A value of m_hash_value that marks an entry as a
		// "tombstone" -- i.e. a placeholder entry.
		static const long unsigned int TOMBSTONE_HASH = (long unsigned int)-1;

		int	find_index(const T& key) const
			// Find the index of the matching entry.  If no match, then return -1.
		{
			if (m_table == NULL) return -1;

			long unsigned int	hash_value = compute_hash(key);
			int	index = (int)(hash_value & m_table->m_size_mask);

			const entry*	e = &E(index);
			if (e->is_empty()) return -1;
			if (e->is_tombstone() == false
				&& int(e->m_hash_value & m_table->m_size_mask) != index) {
				// occupied by a collider
				return -1;
			}

			for (;;)
			{
				assert(e->is_tombstone()
					|| (e->m_hash_value & m_table->m_size_mask) == (hash_value & m_table->m_size_mask));

				if (e->m_hash_value == hash_value && e->first == key)
				{
					// Found it.
					return index;
				}
				assert(e->is_tombstone() || !(e->first == key));	// keys are equal, but hash differs!


				// Keep looking through the chain.
				index = e->m_next_in_chain;
				if (index == -1) break;	// end of chain

				assert(index >= 0 && index <= m_table->m_size_mask);
				e = &E(index);

				assert(e->is_empty() == false || e->is_tombstone());
			}
			return -1;
		}

		long unsigned int compute_hash(const T& key) const {
			long unsigned int hash_value = hash_functor()(key);
			if (hash_value == TOMBSTONE_HASH) {
				hash_value ^= 0x8000;
			}
			return hash_value;
		}

		// Helpers.
		entry&	E(int index)
		{
			assert(m_table);
			assert(index >= 0 && index <= m_table->m_size_mask);
			return *(((entry*)(m_table + 1)) + index);
		}
		const entry&	E(int index) const
		{
			assert(m_table);
			assert(index >= 0 && index <= m_table->m_size_mask);
			return *(((entry*)(m_table + 1)) + index);
		}


		void	set_raw_capacity(int new_size)
			// Resize the hash table to the given size (Rehash the
			// contents of the current table).  The arg is the number of
			// hash table entries, not the number of elements we should
			// actually contain (which will be less than this).
		{
			if (new_size <= 0) {
				// Special case.
				clear();
				return;
			}

			// Force new_size to be a power of two.
			int	bits = fchop(log2_swf((float)(new_size - 1)) + 1);
			assert((1 << bits) >= new_size);

			new_size = 1 << bits;

			// Minimum size; don't incur rehashing cost when
			// expanding very small tables.
			if (new_size < 16)
			{
				new_size = 16;
			}

			if (m_table && new_size == m_table->m_size_mask + 1) {
				// No change in raw capacity; don't do any work.
				return;
			}

			hash<T, U, hash_functor>	new_hash;
			new_hash.m_table = (table*)tu_malloc(sizeof(table)+sizeof(entry)* new_size);
			assert(new_hash.m_table);	// @@ need to throw (or something) on malloc failure!

			new_hash.m_table->m_entry_count = 0;
			new_hash.m_table->m_size_mask = new_size - 1;
			{for (int i = 0; i < new_size; i++)
			{
				new_hash.E(i).m_next_in_chain = -2;	// mark empty
			}}

			// Copy stuff to new_hash
			if (m_table)
			{
				for (int i = 0, n = m_table->m_size_mask; i <= n; i++)
				{
					entry*	e = &E(i);
					if (e->is_empty() == false && e->is_tombstone() == false)
					{
						// Insert old entry into new hash.
						new_hash.add(e->first, e->second);
						e->clear();	// placement delete of old element
					}
				}

				// Delete our old data buffer.
				tu_free(m_table, sizeof(table)+sizeof(entry)* (m_table->m_size_mask + 1));
			}

			// Steal new_hash's data.
			m_table = new_hash.m_table;
			new_hash.m_table = NULL;
		}

		struct table
		{
			int	m_entry_count;
			int	m_size_mask;
			// entry array goes here!
		};
		table*	m_table;
	};
}

#endif // not _TU_USE_STL

class tu_stringi;

// String-like type.  Attempt to be memory-efficient with small strings.
class tu_string
{
public:
	tu_string() {
		m_union.m_local.m_bufsize_minus_length = sizeof(m_union.m_local.m_buffer);
		m_union.m_local.m_buffer[0] = 0;
	}
	// TODO: add 'explicit' here
	tu_string(int val)
	{
		char str[50];
		snprintf(str, 50, "%d", val);

		m_union.m_local.m_bufsize_minus_length = sizeof(m_union.m_local.m_buffer);
		m_union.m_local.m_buffer[0] = 0;

		int	new_size = (int) strlen(str);
		resize(new_size);
		strcpy(get_buffer(), str);
	}
	// TODO: add 'explicit' here
	tu_string(double val)
	{
		char str[50];
		snprintf(str, 50, "%.14g", val);

		m_union.m_local.m_bufsize_minus_length = sizeof(m_union.m_local.m_buffer);
		m_union.m_local.m_buffer[0] = 0;

		int	new_size = (int) strlen(str);
		resize(new_size);
		strcpy(get_buffer(), str);
	}
	tu_string(const char* str)
	{
		m_union.m_local.m_bufsize_minus_length = sizeof(m_union.m_local.m_buffer);
		m_union.m_local.m_buffer[0] = 0;

		if (str)
		{
			int	new_size = (int) strlen(str);
			resize(new_size);
			strcpy(get_buffer(), str);
		}
	}
	tu_string(const char* buf, int buflen)
	{
		m_union.m_local.m_bufsize_minus_length = sizeof(m_union.m_local.m_buffer);
		m_union.m_local.m_buffer[0] = 0;

		int	new_size = buflen;
		resize(new_size);
		memcpy(get_buffer(), buf, buflen);
		get_buffer()[buflen] = 0;	// terminate.
	}
	tu_string(const tu_string& str)
	{
		m_union.m_local.m_bufsize_minus_length = sizeof(m_union.m_local.m_buffer);
		m_union.m_local.m_buffer[0] = 0;

		resize(str.size());
		memcpy(get_buffer(), str.get_buffer(), size() + 1);
	}
	tu_string(const uint32* wide_char_str)
	{
		m_union.m_local.m_bufsize_minus_length = sizeof(m_union.m_local.m_buffer);
		m_union.m_local.m_buffer[0] = 0;

		*this = wide_char_str;
	}
	tu_string(const uint16* wide_char_str)
	{
		m_union.m_local.m_bufsize_minus_length = sizeof(m_union.m_local.m_buffer);
		m_union.m_local.m_buffer[0] = 0;

		*this = wide_char_str;
	}

	~tu_string()
	{
		if (using_heap())
		{
			tu_free(m_union.m_heap.m_buffer, m_heap.m_capacity);
		}
	}

	exported_module operator const char*() const
	{
		return get_buffer();
	}

	exported_module const char*	c_str() const
	{
		return (const char*) (*this);
	}

	// If you need a const tu_stringi, don't create a new object;
	// these things have the same internal representation.
	exported_module const tu_stringi&	to_tu_stringi() const { return *(tu_stringi*) this; }

	// operator= returns void; if you want to know why, ask Charles Bloom :)
	// (executive summary: a = b = c is an invitation to bad code)
	exported_module void	operator=(const char* str)
	{
		if (str)
		{
			resize((int) strlen(str));
			memcpy(get_buffer(), str, size() + 1);
			assert(get_buffer()[size()] == 0);
		}
	}

	exported_module void	operator=(const tu_string& str)
	{
		if (this != &str) {
			resize(str.size());
			memcpy(get_buffer(), str.get_buffer(), size() + 1);
			assert(get_buffer()[size()] == 0);
		}
	}

	exported_module bool	operator==(const char* str) const
	{
		return strcmp(*this, str) == 0;
	}

	exported_module bool	operator!=(const char* str) const
	{
		return strcmp(*this, str) != 0;
	}

	exported_module bool	operator==(const tu_string& str) const
	{
		if (size() != str.size()) {
			return false;
		}
		return memcmp(*this, str, size()) == 0;
	}

	exported_module bool	operator!=(const tu_string& str) const
	{
		return !(*this == str);
	}

	exported_module int	length() const
	{
		if (using_heap() == false)
		{
			return sizeof(m_union.m_local.m_buffer) -
				m_union.m_local.m_bufsize_minus_length;
		}
		else
		{
			return m_union.m_heap.m_size;
		}
	}

	exported_module int	size() const {
		return length();
	}

	// index >=0
	exported_module void erase(int index, int count)
	{
		assert(index + count <= length());
		memmove(get_buffer() + index, get_buffer() + index + count,
			size() - index - count);
		resize(size() - count);
	}

	// insert char before index
	exported_module void insert(int index, char ch)
	{
		assert(index >= 0 && index <= size());
		int len = length();
		resize(len + 1);
		char* buf = get_buffer();
		// actual memory size of the empty string is 1 (zero at the end)
		memmove(buf + index + 1, buf + index, len + 1 - index);
		*(buf + index) = ch;
	}

	exported_module char&	operator[](int index)
	{
		assert(index >= 0 && index <= size());
		return get_buffer()[index];
	}

	exported_module const char&	operator[](int index) const
	{
		return (*(const_cast<tu_string*>(this)))[index];
	}

	exported_module void	operator+=(const char* str)
	{
		int	str_length = (int) strlen(str);
		int	old_length = length();
		assert(old_length >= 0);
		resize(old_length + str_length);
		memcpy(get_buffer() + old_length, str, str_length + 1);
	}

	exported_module void	operator+=(char ch)
	{
		int old_length = length();
		assert(old_length >= 0);
		resize(old_length + 1);
		*(get_buffer() + old_length) = ch;
	}

	// Append wide char.  Both versions of wide char.
	exported_module void	append_wide_char(uint16 ch);
	exported_module void	append_wide_char(uint32 ch);

	exported_module void	operator+=(const tu_string& str)
	{
		int str_length = str.length();
		int old_length = length();
		assert(old_length >= 0);
		int new_length = old_length + str_length;
		resize(new_length);
		memcpy(get_buffer() + old_length, str.c_str(), str_length);
		get_buffer()[new_length] = 0;
	}

	exported_module tu_string	operator+(const tu_string& str) const
	{
		tu_string	new_string(*this);
		new_string += str;
		return new_string;
	}

	exported_module tu_string	operator+(const char* str) const
	// NOT EFFICIENT!  But convenient.
	{
		tu_string	new_string(*this);
		new_string += str;
		return new_string;
	}

	exported_module bool	operator<(const char* str) const
	{
		return strcmp(c_str(), str) < 0;
	}

	exported_module bool operator<(const tu_string& str) const
	{
		const char* p0 = get_buffer();
		const char* p1 = str.get_buffer();
		int min_len = length();
		bool str_longer = true;
		if (str.length() < min_len) {
			min_len = str.length();
			str_longer = false;
		}
		int cmp = memcmp(p0, p1, min_len);
		if (cmp < 0) {
			return true;
		} else if (cmp > 0) {
			return false;
		}
		return str_longer;
	}

	exported_module bool	operator>(const char* str) const
	{
		return strcmp(c_str(), str) > 0;
	}

	exported_module bool	operator>(const tu_string& str) const
	{
		return str < *this;
	}

	exported_module void clear()
	{
		resize(0);
	}
	
	// Sets buffer size to new_size+1 (i.e. enough room for
	// new_size chars, plus terminating 0).
	exported_module void	resize(int new_size);

	// Set *result to the UTF-8 encoded version of wstr[].
	// Both version of wchar_t.
	//
	// Could add operator= overloads, but maybe it's better to
	// keep this very explicit.
	exported_module static void	encode_utf8_from_wchar(tu_string* result, const uint32* wstr);
	exported_module static void	encode_utf8_from_wchar(tu_string* result, const uint16* wstr);

	// Utility: case-insensitive string compare.  stricmp() is not
	// ANSI or POSIX, doesn't seem to appear in Linux.
	exported_module static int	stricmp(const char* a, const char* b);

	// Return the Unicode char at the specified character
	// position.  index is in UTF-8 chars, NOT bytes.
	exported_module uint32	utf8_char_at(int index) const;

	// Return the string in this container as all upper case letters
	exported_module tu_string utf8_to_upper() const;

	// Return the string in this container as all lower case letters
	exported_module tu_string utf8_to_lower() const;

	// Return the number of UTF-8 characters in the given
	// substring buffer.  You must pass in a valid buffer length;
	// this routine does not look for a terminating \0.
	exported_module static int	utf8_char_count(const char* buf, int buflen);

	exported_module int	utf8_length() const { return utf8_char_count(get_buffer(), length()); }

	// Returns a tu_string that's a substring of this.  start and
	// end are in UTF-8 character positions (NOT bytes).
	//
	// start is the index of the first character you want to include.
	//
	// end is the index one past the last character you want to include.
	exported_module tu_string	utf8_substring(int start, int end) const;

private:
	char*	get_buffer()
	{
		if (using_heap() == false)
		{
			return m_union.m_local.m_buffer;
		}
		else
		{
			return m_union.m_heap.m_buffer;
		}
	}

	const char*	get_buffer() const
	{
		return const_cast<tu_string*>(this)->get_buffer();
	}


	bool	using_heap() const
	{
		bool heap = (m_union.m_local.m_bufsize_minus_length == (char) ~0);
		return heap;
	}

	// The idea here is that tu_string is an N-byte structure,
	// which uses internal storage for strings of N-1 characters
	// or less.  For longer strings, it allocates a heap buffer,
	// and keeps the buffer-tracking info in the same bytes that
	// would be used for internal string storage.
	//
	// A string that's implemented like vector<char> is typically
	// 12 bytes plus heap storage, so storing short strings
	// directly in the string metadata is usually a big win.
	// Also, a zero-length string still needs a terminator
	// character, which with vector<char> means an unfortunate
	// heap alloc just to hold a single '0'.
	union
	{
		// Heap storage.
		struct heap_metadata
		{
			int m_size;  // not counting terminating \0
			char* m_buffer;
			int m_capacity;  // TODO: move this into the heap data, a la Won Chun
			// m_local uses the last byte of the struct to
			// signal when it's active, so m_heap can't
			// put anything there.
			int m_dummy;
		} m_heap;

		// Internal storage.
		struct local_data
		{
			char m_buffer[sizeof(heap_metadata) - 1];
			char m_bufsize_minus_length;
			// m_bufsize_minus_length holds
			// ((sizeof(m_buffer) - length).  This is so
			// that a string of length sizeof(m_buffer)
			// has a zero byte in the length field, that
			// does double-duty as the string terminator.
			// This trick allows us to hold one more byte
			// than we would otherwise be able to.
			//
			// if the m_bufsize_minus_length byte is all
			// ones, then that's a flag indicating the
			// active string data is in m_heap.
		} m_local;
	} m_union;
};


// String-like type; comparisons are CASE INSENSITIVE.
// Uses tu_string for implementation.
class tu_stringi
{
public:
	tu_stringi() {}
	tu_stringi(const char* str) : m_string(str) {}
	tu_stringi(const tu_string& str) : m_string(str) {}
	tu_stringi(const tu_stringi& stri) : m_string(stri.c_str()) {}

	~tu_stringi() {}

	operator const char*() const { return (const char*) m_string; }
	const char*	c_str() const { return m_string.c_str(); }
	void	operator=(const char* str) { m_string = str; }
	void	operator=(const tu_string& str) { m_string = str; }
	void	operator=(const tu_stringi& str) { m_string = str.m_string; }
	int	length() const { return m_string.length(); }
	int	size() const { return length(); }
	char&	operator[](int index) { return m_string[index]; }
	const char&	operator[](int index) const { return m_string[index]; }
	void	operator+=(const char* str) { m_string += str; }
	void	operator+=(const tu_string& str) { m_string += str; }
	void	operator+=(const tu_stringi& str) { m_string += str.m_string; }
	tu_stringi	operator+(const char* str) const { return tu_stringi(m_string + str); }

	// The special stuff.
	tu_string& to_tu_string() { return m_string; }
	const tu_string& to_tu_string() const { return m_string; }

	bool	operator==(const char* str) const
	{
		return tu_string::stricmp(*this, str) == 0;
	}
	bool	operator==(const tu_stringi& str) const
	{
		return tu_string::stricmp(*this, str) == 0;
	}
	bool	operator<(const char* str) const
	{
		return tu_string::stricmp(c_str(), str) < 0;
	}
	bool	operator<(const tu_stringi& str) const
	{
		return *this < str.c_str();
	}
	bool	operator>(const char* str) const
	{
		return tu_string::stricmp(c_str(), str) > 0;
	}
	bool	operator>(const tu_stringi& str) const
	{
		return *this > str.c_str();
	}

	void	resize(int new_size) { m_string.resize(new_size); }

private:
	tu_string	m_string;
};


template<class T>
class string_hash_functor
// Computes a hash of a string-like object (something that has
// ::length() and ::[int]).
{
public:
	long unsigned int	operator()(const T& data) const
	{
		int	size = data.length();

		return bernstein_hash((const char*) data, size);
	}
};


template<class U>
class string_hash : public gameswf::hash<tu_string, U, string_hash_functor<tu_string> >
{
};


template<class T>
class stringi_hash_functor
// Computes a case-insensitive hash of a string-like object (something that has
// ::length() and ::[int] and tolower(::[])).
{
public:
	long unsigned int	operator()(const T& data) const
	{
		int	size = data.length();

		return bernstein_hash_case_insensitive((const char*) data, size);
	}
};


// Case-insensitive string hash.
template<class U>
class stringi_hash : public gameswf::hash<tu_stringi, U, stringi_hash_functor<tu_stringi> >
{
};


// Utility: handy sprintf wrapper.
tu_string string_printf(const char* fmt, ...)
#ifdef __GNUC__
	// use the following to catch errors: (only with gcc)
	__attribute__((format (printf, 1, 2)))
#endif	// not __GNUC__
;


#endif // CONTAINER_H

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
