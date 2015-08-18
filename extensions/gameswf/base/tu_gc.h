// tu_gc.h  -- Thatcher Ulrich <http://tulrich.com> 2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A simple smart pointer and framework for garbage collection.  A
// variety of different collectors may be plugged in, so the gc_ptr is
// parameterized by the collector.
//
// This differs from the boost gc_ptr proposals I've seen in that it
// is "intrusive" -- any classes that you want garbage-collected must
// derive from gc_object_base.  All gc_objects must be allocated on
// the heap via gc_object_base::new.  C++ will handle this
// automatically when you use ordinary 'new' syntax.  The
// implementation may assert if you accidentally try to create a
// gc_object on the stack or inside another object.
//
// gc_ptr's can live anywhere; on the stack, on the garbage-collected
// heap, on the regular heap.  gc_ptr's outside the garbage-collected
// heap are "roots"; i.e. they cannot be collected by the garbage
// collector.  Objects on the gc heap are collected automatically,
// according to the policy of the specific collector being used.
//
// gc_ptr's are parameterized by garbage collector type.  Different gc
// types have different characteristics.
//
// Note: if you feel the desire to point a gc_ptr at a type that is
// not derived from gc_object_base, you probably want a scoped_ptr
// instead.
//
// Usage:
//
// #include <base/tu_gc.h>
// #include <base/tu_gc_singlethreaded_marksweep.h>  // specific collector
//
// // Declare the types gc_ptr<T>, gc_object, and gc_collector,
// // parameterized by the specific garbage collector you are using.
// DECLARE_GC_TYPES(tu_gc::singlethreaded_marksweep);
//
// ...
// class my_object : public gc_object {
//   gc_ptr<my_object> m_ptr;
//   gc_ptr<other_object> m_ptr2;
//
//   ...
//     m_ptr = new my_object(args);
//     m_ptr->some_method();
// };
//
// ...
//   gc_ptr<my_object> obj1 = new my_object;
//   gc_ptr<my_object> obj2 = new my_object;
//   obj1->m_ptr2 = obj2;
//   obj2->m_ptr = obj2;
//   obj2->m_ptr2 = obj1;
// ...
//   gc_collector::collect_garbage();  // optional
//
// // etc.


// Here's an interesting article on C++ garbage collection:
// http://home.elka.pw.edu.pl/~pkolaczk/papers/gc-prodialog.pdf


// TODO:
//
// implement GC_CONTAINER for associative containers like map<>.
//
// improve type-inference & safety for GC_CONTAINER, it's pretty ugly
// right now.  Need it to be less verbose and more foolproof.

#ifndef TU_GC_H
#define TU_GC_H

#include <assert.h>
#include <stddef.h>

#include "weak_ptr.h"


namespace tu_gc {

	class collector_access;
	
	// Used by the collector to help keep blocks alive during
	// construction (between allocation and when they are finally
	// assigned to a gc_ptr).
	//
	// Relies on standard C++ guarantee: temporary objects are not
	// destroyed until the end of the full statement in which they
	// appear.
	//
	// Thanks to cppgc (boost gc_ptr proposal by ...) for the idea.
	class block_construction_locker_base {
	public:
		block_construction_locker_base() : m_block(0) {
		}
		block_construction_locker_base* get_this() {
			return this;
		}
	protected:
		friend class collector_access;
		void* m_block;
	};

	template<class garbage_collector>
	class block_construction_locker : public block_construction_locker_base {
	public:
		~block_construction_locker() {
			garbage_collector::block_construction_finished(m_block);
		}
	};

	class gc_object_generic_base {
	public:
		virtual ~gc_object_generic_base() {}
	};

	// (The collector defines gc_object_generic_base which
	// inherits from gc_object_generic_base and optionally adds
	// collector-specific properties.)

	template<class garbage_collector>
	class gc_object_base : public garbage_collector::gc_object_collector_base {
	public:
		gc_object_base() {
			garbage_collector::constructing_gc_object_base(this);
		}

		static void* operator new(size_t sz,
			block_construction_locker_base* lock = block_construction_locker<garbage_collector>().get_this())
		{
			return garbage_collector::allocate(sz, lock);
		}
		static void operator delete(void* p, block_construction_locker_base* lock) {
			return garbage_collector::deallocate(p);
		}
		static void operator delete(void* p) {
			return garbage_collector::deallocate(p);
		}

	private:
		// TODO: are arrays worth implementing?
		static void* operator new[](size_t sz) {
			assert(0);
			return 0;
		}
	};

	// The smart pointer.
	template<class T, class garbage_collector>
	class gc_ptr {
	public:
		gc_ptr() : m_ptr(0) {
			garbage_collector::construct_pointer(this);
		}
		explicit gc_ptr(T* p) : m_ptr(0) {
			garbage_collector::construct_pointer(this);
			reset(p);
		}
		explicit gc_ptr(const gc_ptr& p) : m_ptr(0) {
			garbage_collector::construct_pointer(this);
			reset((T*) p.m_ptr);
		}

		~gc_ptr() {
			reset(0);
			garbage_collector::destruct_pointer(this);
		}

		T* operator->() const {
			return (T*) m_ptr;
		}

		T& operator*() const {
			return *(T*) m_ptr;
		}

		void operator=(const gc_ptr& p) {
			reset(p.get());
		}

		void operator=(T* p) {
			reset(p);
		}

		bool operator==(const gc_ptr& p) const {
			return m_ptr == p.m_ptr;
		}

		bool operator==(const T* p) const {
			return m_ptr == p;
		}

		operator T*() const {
			return m_ptr;
		}

		T* get() const {
			return m_ptr;
		}

		T* get_ptr() const {
			return get();
		}

		void reset(T* p) {
			garbage_collector::write_barrier(this, p);
		}

		// This is only for use by the garbage collector.  Don't use it!
		// TODO: figure out how to protect it.
		void raw_set_ptr_gc_access_only(T* p) {
			m_ptr = p;
		}
	private:
		T* m_ptr;
	};

	// For pointers to gc_object that are kept in containers.
	// NOTE: work in progress, subject to change.
	template<class T, class garbage_collector>
	class contained_gc_ptr {
	public:
		typedef void i_am_a_contained_gc_ptr;  // For asserting type; I gotta re-read Modern C++ Design :(
		contained_gc_ptr() : m_ptr(0) {
			garbage_collector::construct_contained_pointer(this);
		}
		contained_gc_ptr(T* p) : m_ptr(0) {
			garbage_collector::construct_contained_pointer(this);
			reset(p);
		}
		contained_gc_ptr(const contained_gc_ptr& p) : m_ptr(0) {
			garbage_collector::construct_contained_pointer(this);
			reset((T*) p.m_ptr);
		}

		~contained_gc_ptr() {
			reset(0);
			garbage_collector::destruct_contained_pointer(this);
		}

		T* operator->() const {
			return (T*) m_ptr;
		}

		T& operator*() const {
			return *(T*) m_ptr;
		}

		void operator=(const contained_gc_ptr& p) {
			reset(p.get());
		}

		void operator=(T* p) {
			reset(p);
		}

		bool operator==(const contained_gc_ptr& p) const {
			return m_ptr == p.m_ptr;
		}

		bool operator==(const T* p) const {
			return m_ptr == p;
		}

		T* get() const {
			return m_ptr;
		}

		void reset(T* p) {
			garbage_collector::contained_pointer_write_barrier(this, p);
		}

		// This is only for use by the garbage collector.  Don't use it!
		// TODO: figure out how to protect it.
		void raw_set_ptr_gc_access_only(T* p) {
			m_ptr = p;
		}
	private:
		T* m_ptr;
	};

	// Garbage collectors can inherit from this to get access
	// block_construction_locker_base::m_block.
	class collector_access {
	protected:
		static void*& block_ref(block_construction_locker_base* lock) {
			return lock->m_block;
		}
	};

	// Use this to declare the types gc_ptr<T>, gc_object, and
	// gc_collector using your chosen collector, in whatever
	// namespace is convenient for you.
	//
	// E.g.:
	//
	//   namespace my_namespace {
	//     DECLARE_GC_TYPES(tu_gc::singlethreaded_marksweep);
	//   }
	//
	//   Then use gc_ptr<T> and gc_object in my_namespace.
	//
	#define DECLARE_GC_TYPES(collector_classname) 			\
		typedef collector_classname gc_collector;		\
		typedef tu_gc::gc_object_base<gc_collector> gc_object;  \
									\
		template<class T>					\
		class gc_ptr : public tu_gc::gc_ptr<T, collector_classname> {	\
		public:							\
			typedef collector_classname collector;		\
			gc_ptr() {}					\
			gc_ptr(T* p) : tu_gc::gc_ptr<T, collector_classname>(p) {} \
		};							\
									\
		template<class T>					\
		class gc_container : public collector_classname::gc_container<T> { \
		public:							\
			gc_container() {}				\
		};							\
		template<class T>					\
		class gc_pair_container : public collector_classname::gc_pair_container<T> { \
		public:							\
			gc_pair_container() {}				\
		};

	// For STL containers inside gc_objects whose values point at
	// other gc_objects.
	
	// Use like:
	//
	// SPECIALIZE_GC_CONTAINER(gc_vector, std::vector);
	//
	// class my_class : public gc_object {
	//   ...
	//   gc_vector<gc_ptr<my_class> > m_pointers;
	//   ...
	//     // Use m_pointers just like a regular std::vector.
	//     for (int i = 0; i < m_pointers.size(); i++) {
	//       m_pointers[i]->something();
	//     }
	// };
	#define SPECIALIZE_GC_CONTAINER(generated_name, base_container) \
		template<class T>					\
		class generated_name;					\
									\
		template<class T>					\
		class generated_name<gc_ptr<T> >			\
			: public gc_container<base_container<tu_gc::contained_gc_ptr<T, typename gc_ptr<T>::collector> > > { \
		}

	// Generate a pair container that can correctly contain gc_ptr's.
	//
	// This is important to make sure that contained pointers are
	// known to be owned by their containers; otherwise the
	// contained pointers look like roots, and the GC is too
	// conservative.
	//
	// For example:
	//
	//   SPECIALIZE_GC_PAIR_CONTAINER(gc_map, std::map);
	//
	//   class my_class : public gc_object {
	//     gc_map<int, gc_ptr<X> > m_1;
	//     gc_map<gc_ptr<X>, int > m_2;
	//     gc_map<gc_ptr<X>, gc_ptr<Y> > m_3;
	//     // use the m_'s as if they were std::map.
	//   };
	#define SPECIALIZE_GC_PAIR_CONTAINER(generated_name, base_container)	\
		template<class T, class U>					\
		class generated_name;						\
										\
		template<class T, class U>					\
		class generated_name<T, gc_ptr<U> >				\
			: public gc_pair_container< base_container <T, tu_gc::contained_gc_ptr<U, typename gc_ptr<T>::collector> > > { \
		};								\
		template<class T, class U>					\
		class generated_name<gc_ptr<T>, U >				\
			: public gc_pair_container< base_container <tu_gc::contained_gc_ptr<T, typename gc_ptr<T>::collector>, U> > { \
		};								\
		template<class T, class U>					\
		class generated_name<gc_ptr<T>, gc_ptr<U> >			\
			: public gc_pair_container< base_container <tu_gc::contained_gc_ptr<T, typename gc_ptr<T>::collector>, tu_gc::contained_gc_ptr<U, typename gc_ptr<T>::collector> > > { \
		}


	
	// TODO: incremental gc

	// TODO: incremental generational gc

	// TODO: multithreaded variants

}  // tu_gc

#endif  // TU_GC_H
