// tu_gc_singlethreaded_marksweep.h  -- Thatcher Ulrich <http://tulrich.com> 2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A simple but useful singlethreaded mark/sweep garbage collector,
// for use with tu_gc::gc_ptr<>.  It is not thread-safe.
//
// See tu_gc.h for basic usage.  See the documented methods below for
// additional (optional) interfaces.
//
// This implementation is very vanilla C++ and hopefully portable to
// any runtime environment, at the expense of not being especially
// optimized for space or time.
//
// Performance: it's much slower and uses much more space than manual
// explicit memory management.  I did some sanity tests using
// GCBench.cpp (from
// http://www.hpl.hp.com/personal/Hans_Boehm/gc/gc_bench/) which
// creates large binary trees.  It's a small synthetic benchmark that
// probably doesn't resemble your program but it does highlight the
// overheads of GC.
//
// Sample results, on a 2.13GHz Pentium M laptop running Windows XP:
//
// Collector                            Peak Mem    Typical Mem     Time
// ---------------------------------------------------------------------
// explicit (no gc)                         13 M          6.3 M    5.0 s
// singlethreaded refcount                  17 M          8   M    5.5 s
// singlethreaded marksweep                 95 M         30   M   56   s


#include "tu_gc.h"

namespace tu_gc {

	class singlethreaded_marksweep {
	public:
		typedef singlethreaded_marksweep this_class;

		// This is a base class of gc_object_base.  Different
		// collectors can use it to add properties to gc_object_base.
		//
		// We don't do anything with it.
		class gc_object_collector_base : public tu_gc::gc_object_generic_base {
		};
		
		// Client interfaces.
		struct stats {
			size_t live_heap_bytes;
			size_t garbage_bytes;
			size_t root_pointers;
			size_t live_pointers;
			size_t root_containers;
		};
		// Gets basic stats.  garbage_bytes will be zero
		// (since we don't know what is garbage) and
		// live_heap_bytes will include everything including
		// potential garbage.
		static void get_stats(stats* s);

		// Collects all garbage.
		//
		// If s is not NULL, fills it with interesting
		// statistics.
		static void collect_garbage(stats* s);

		// Determines how often to automatically collect
		// garbage.  Automatic collection is triggered within
		// the allocator, when the total number of heap bytes
		// exceeds the last-known live byte count by the given
		// percentage.  E.g. if the arg is 100, collection is
		// initiated when the heap size has doubled since the
		// previous collection.
		//
		// 0 will force a collection before every allocation.
		//
		// The default is 100.
		static void set_collection_rate(int percent_growth_before_next_collection);

		// Semi-private interfaces.  TODO: figure out how to
		// protect these.

		// Called by the block construction locker after the
		// new() expression completes (presumably after the
		// new block has been assigned to a gc_ptr).
		static void block_construction_finished(void* block);

		// Called from gc_object_base constructor.  Helps us
		// associate a gc_object_generic_base* with its
		// containing heap block (in case of gnarly multiple
		// inheritance).
		static void constructing_gc_object_base(gc_object_generic_base* obj);
		
		// Called by the smart ptr during construction.
		//
		// This collector doesn't care, until the pointer
		// takes a non-null value, which is notified via
		// write_barrier().
		template<class T>
		static void construct_pointer(gc_ptr<T, this_class>* gc_ptr_p) {}
		
		// Called by the smart ptr during destruction.
		//
		// This collector doesn't care.  What it does care
		// about is when the pointer is cleared, which is
		// notified via write_barrier().
		template<class T>
		static void destruct_pointer(gc_ptr<T, this_class>* gc_ptr_p) {}
		
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
			if (gc_ptr_p->get()) {
				if (!new_val_p) {
					clearing_pointer(gc_ptr_p);
				} else {
					changing_pointer(gc_ptr_p, new_val_p);
				}
			} else if (new_val_p) {
				initing_pointer(gc_ptr_p, new_val_p);
			}
			gc_ptr_p->raw_set_ptr_gc_access_only(new_val_p);
		}

		// Containers
		
		template<class T>
		static void contained_pointer_write_barrier(contained_gc_ptr<T, this_class>* gc_ptr_p, T* new_val_p) {
			gc_ptr_p->raw_set_ptr_gc_access_only(new_val_p);
		}
		
		template<class T>
		static void construct_contained_pointer(contained_gc_ptr<T, this_class>* gc_ptr_p) {}
		template<class T>
		static void destruct_contained_pointer(contained_gc_ptr<T, this_class>* gc_ptr_p) {}
		
		class gc_container_base {
		    public:
			gc_container_base() {
				construct_container(this);
			}
			virtual ~gc_container_base() {
				destruct_container(this);
			}
		
			virtual void visit_contained_ptrs() = 0;
		};

		// gc_container, for collections of pointers
		template<class container_type>
		class gc_container : public gc_container_base, public container_type {
		public:
			// visit contained pointers
			virtual void visit_contained_ptrs() {
				for (typename container_type::const_iterator it = this->begin();
				     it != this->end();
				     ++it) {
					visit_contained_ptr(it->get());
				}
			}
		};

		template<class container_type>
		class gc_pair_container : public gc_container_base, public container_type {
		public:
			// Visit values.
			virtual void visit_contained_ptrs() {
				for (typename container_type::const_iterator it = this->begin();
				     it != this->end();
				     ++it) {
					visit_contained_value(it->first);
					visit_contained_value(it->second);
				}
			}
		};

	private:
		friend class gc_object_base<singlethreaded_marksweep>;

		// Used by gc_object_base new/delete.
		static void* allocate(size_t sz, block_construction_locker_base* lock);
		static void deallocate(void* p);

		// Notifications from write_barrier().
		static void clearing_pointer(void* address_of_gc_ptr);
		static void initing_pointer(void* address_of_gc_ptr, gc_object_generic_base* object_pointed_to);
		static void changing_pointer(void* address_of_gc_ptr, gc_object_generic_base* object_pointed_to);

		// Notifications from gc_container
		static void construct_container(gc_container_base* c);
		static void destruct_container(gc_container_base* c);

		static void visit_contained_ptr(const gc_object_generic_base* obj);

		template<class T>
		static void visit_contained_value(T val) {
			// Default action: do nothing.
		}
		// Specialize for gc objects.
		template<typename T>
		static void visit_contained_value(const contained_gc_ptr<T, this_class>& val) {
			visit_contained_ptr(val.get());
		}
	};
}  // tu_gc
