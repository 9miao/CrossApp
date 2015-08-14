// tu_gc_singlethreaded_refcount.h  -- Thatcher Ulrich <http://tulrich.com> 2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A simple but useful singlethreaded ref-counting "garbage
// collector", for use with tu_gc::gc_ptr<>.  It is not thread-safe.
//
// See tu_gc.h for basic usage.  See the documented methods below for
// additional (optional) interfaces.
//
// This implementation uses vanilla intrusive ref-counting.  It should
// be similar in performance to common C++ ref-counters like
// boost::shared_ptr and such.
//
// It does NOT collect cycles.

#include "tu_gc.h"

namespace tu_gc {

	class singlethreaded_refcount {
	public:
		typedef singlethreaded_refcount this_class;

		// We put our ref-count inside each gc_object via
		// gc_object_collector_base.
		class gc_object_collector_base : public tu_gc::gc_object_generic_base {
		public:
			gc_object_collector_base() : m_ref_count(0) {
			}
			int ref_count() const {
				return m_ref_count;
			}
		private:
			friend class singlethreaded_refcount;
			
			int m_ref_count;
		};
		
		// Client interfaces.
		struct stats {
			size_t live_heap_bytes;
			size_t garbage_bytes;
			size_t root_pointers;
			size_t live_pointers;
			size_t root_containers;
		};
		// Returns basic stats.  Mostly not applicable to
		// ref-counting.
		static void get_stats(stats* s) {
			s->live_heap_bytes = 0;  // TODO: could track this
			s->garbage_bytes = 0;
			s->root_pointers = 0;
			s->live_pointers = 0;  // TODO: could track this
			s->root_containers = 0;
		}

		// Collects all garbage.
		//
		// Not applicable to ref-counting; this is a no-op
		// (but retained here for testing purposes).
		static void collect_garbage(stats* s) {
			if (s) {
				get_stats(s);
			}
		}

		// Semi-private interfaces.  TODO: figure out how to
		// protect these.

		// Called by the block construction locker after the
		// new() expression completes (presumably after the
		// new block has been assigned to a gc_ptr).
		//
		// This collector doesn't care.
		static void block_construction_finished(void* block) {
		}

		// Called from gc_object_base constructor.  Helps us
		// associate a gc_object_generic_base* with its
		// containing heap block (in case of gnarly multiple
		// inheritance).
		//
		// This collector doesn't care.
		static void constructing_gc_object_base(gc_object_generic_base* obj) {
		}
		
		// Called by the smart ptr during construction.
		//
		// This collector doesn't care, until the pointer
		// takes a non-null value, which is notified via
		// write_barrier().
		template<class T>
		static void construct_pointer(gc_ptr<T, this_class>* gc_ptr_p) {
		}
		
		// Called by the smart ptr during destruction.
		//
		// This collector doesn't care.  What it does care
		// about is when the pointer is cleared, which is
		// notified via write_barrier().
		template<class T>
		static void destruct_pointer(gc_ptr<T, this_class>* gc_ptr_p) {
		}
		
		// Used by the smart ptr to change the value of the
		// pointer.
		//
		// We take notice whenever the pointer changes from
		// null to a value or vice-versa.  The collector
		// doesn't care about null pointers.
		template<class T>
		static void write_barrier(gc_ptr<T, this_class>* gc_ptr_p, T* new_val_p) {
			assert(gc_ptr_p);
			if (new_val_p == gc_ptr_p->get()) {
				return;
			}
			if (new_val_p) {
				increment_ref(new_val_p);
			}
			if (gc_ptr_p->get()) {
				decrement_ref(gc_ptr_p->get());
			}
			gc_ptr_p->raw_set_ptr_gc_access_only(new_val_p);
		}

		template<class T>
		static void contained_pointer_write_barrier(contained_gc_ptr<T, this_class>* gc_ptr_p, T* new_val_p) {
			// TODO de-dupe; this is same as write_barrier()
			assert(gc_ptr_p);
			if (new_val_p == gc_ptr_p->get()) {
				return;
			}
			if (new_val_p) {
				increment_ref(new_val_p);
			}
			if (gc_ptr_p->get()) {
				decrement_ref(gc_ptr_p->get());
			}
			gc_ptr_p->raw_set_ptr_gc_access_only(new_val_p);
		}
		
		template<class T>
		static void construct_contained_pointer(contained_gc_ptr<T, this_class>* gc_ptr_p) {}
		template<class T>
		static void destruct_contained_pointer(contained_gc_ptr<T, this_class>* gc_ptr_p) {}

		template<class container_type>
		class gc_container : public container_type {
		public:
			gc_container()
			{
				// this causes gcc 3.2.2 compilation error
//				(container_type::value_type::i_am_a_contained_gc_ptr) 0;  // assert that we contain contained_gc_ptr.
			}
		};

		template<class container_type>
		class gc_pair_container : public container_type {
		};

		static int debug_get_ref_count(gc_object_collector_base* obj) {
			assert(obj);
			return obj->m_ref_count;
		}
		
		static void debug_decrement_ref_count(gc_object_collector_base* obj) {
			decrement_ref(obj);
		}
		
	private:
		friend class gc_object_base<singlethreaded_refcount>;

		// Used by gc_object_base new/delete.
		static void* allocate(size_t sz, block_construction_locker_base* lock) {
			return operator new(sz);
		}
		static void deallocate(void* p) {
			return operator delete(p);
		}

		// Notifications from write_barrier().
		static void increment_ref(gc_object_collector_base* obj) {
			assert(obj);
			assert(obj->ref_count() >= 0);
			obj->m_ref_count++;
		}
		static void decrement_ref(gc_object_collector_base* obj) {
			assert(obj);
			assert(obj->ref_count() > 0);
			obj->m_ref_count--;
			if (obj->m_ref_count == 0) {
				delete obj;
			}
		}
		
	};
}  // tu_gc
