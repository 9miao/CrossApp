// tu_gc_singlethreaded_marksweep.cpp  -- Thatcher Ulrich <http://tulrich.com> 2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Single-threaded mark-sweep collector.


#include "tu_gc_singlethreaded_marksweep.h"
#include <map>
#include <set>
#include <vector>

namespace tu_gc {

	struct blockinfo {
		bool mark;
		// start of the object memory block
		void* p;
		// size of the object memory block
		size_t sz;
		// This is somewhere inside the block (usually at the
		// very beginning, but not if there is multiple
		// inheritance going on).
		//
		// We need it in order to call the object's virtual
		// destructor.
		gc_object_generic_base* obj;

		blockinfo() : mark(false), p(0), sz(0), obj(0) {
		}
		blockinfo(void* p_in, size_t sz_in) : mark(false), p(p_in), sz(sz_in), obj(0) {
		}

		void* end() const {
			return static_cast<char*>(p) + sz;
		}

		bool operator<(const blockinfo& bi) const {
			return p < bi.p;
		}

		bool is_address_inside(const void* ptr) const {
			return (ptr >= p && ptr < end());
		}
	};

	struct singlethreaded_marksweep_state : public collector_access {
		// Convenience typedefs.
		typedef std::map<void*, gc_object_generic_base*>::iterator ptr_iterator;
		typedef std::set<void*>::iterator floating_blocks_iterator;
		typedef std::map<void*, blockinfo>::iterator heap_block_iterator;
		typedef std::set<singlethreaded_marksweep::gc_container_base*>::iterator container_iterator;

		// roots
		std::map<void*, gc_object_generic_base*> m_roots;

		// A set of blocks that are not yet under the control
		// of a gc_ptr.  Typically this happens between the
		// time gc_object_base::new returns void* and when the
		// value is assigned to a gc_ptr.  We have to treat
		// such blocks as rooted to avoid problems with code
		// like:
		//
		//   gc_ptr<MyObj> p = new MyObj(SomeFunctionThatMightTriggerACollection());
		//
		// since the block for MyObj must be allocated before
		// the MyObj constructor is called, and the MyObj
		// constructor must happen before p is assigned.
		std::set<void*> m_floating_blocks;

		// non-roots
		std::map<void*, gc_object_generic_base*> m_heap_ptrs;

		// heap blocks
		std::map<void*, blockinfo> m_heap_blocks;

		// containers
		std::set<singlethreaded_marksweep::gc_container_base*> m_root_containers;
		std::set<singlethreaded_marksweep::gc_container_base*> m_containers;

		// mark state
		std::vector<const gc_object_generic_base*> m_to_mark;

		// Stats & control values.
		int m_percent_growth;
		size_t m_current_heap_bytes;
		size_t m_next_collection_heap_size;
		size_t m_last_collection_heap_size;

		singlethreaded_marksweep_state() :
			m_percent_growth(100),
			m_current_heap_bytes(0),
			m_next_collection_heap_size(1 << 16),
			m_last_collection_heap_size(1 << 16) {
		}

		void set_collection_rate(int percent) {
			m_percent_growth = percent;
			size_t a = m_last_collection_heap_size;
			double b = m_percent_growth / 100.0;
			double c = a * (1 + b);
			m_next_collection_heap_size = static_cast<size_t>(c);
		}

		void* allocate(size_t sz, block_construction_locker_base* lock) {
			assert(sz > 0);
			assert(lock);

			if (m_current_heap_bytes >= m_next_collection_heap_size) {
				// It's time to collect.
				collect_garbage(NULL);
			}
		
			gc_object_generic_base* block = static_cast<gc_object_generic_base*>(operator new(sz /* + overhead? */));
			m_heap_blocks.insert(std::make_pair(block, blockinfo(block, sz)));
			m_current_heap_bytes += sz;

			// Keep this block from being collected during
			// construction, before it has a chance to be
			// assigned to a gc_ptr.
			m_floating_blocks.insert(block);
			block_ref(lock) = block;

			return block;
		}

		void deallocate(void* p) {
			// assert(no heap pointers inside this block);
		
			// Fill with junk.
			//TODO 0xCAFFE14E

			// Return the memory to the C++ heap.
			operator delete(p);
		}

		void block_construction_finished(void* block) {
			floating_blocks_iterator it = m_floating_blocks.find(block);
			assert(it != m_floating_blocks.end());
			m_floating_blocks.erase(it);
		}

		void constructing_gc_object_base(gc_object_generic_base* obj) {
			heap_block_iterator it = find_containing_block(obj);
			assert(it != m_heap_blocks.end());  // gc_objects must be constructed on the heap
			assert(it->second.obj == NULL);
			it->second.obj = obj;
		}

		void get_stats(singlethreaded_marksweep::stats* s) {
			assert(s);
			s->live_heap_bytes = m_current_heap_bytes;
			s->garbage_bytes = 0;
			s->root_pointers = m_roots.size();
			s->live_pointers = s->root_pointers + m_heap_ptrs.size();
			s->root_containers = m_root_containers.size();
		}

		void collect_garbage(singlethreaded_marksweep::stats* s) {
			size_t precollection_heap_bytes = m_current_heap_bytes;

			mark_live_objects();
			sweep_dead_objects();
		
			m_last_collection_heap_size = m_current_heap_bytes;
			set_collection_rate(m_percent_growth);

			if (s) {
				s->live_heap_bytes = m_current_heap_bytes;
				s->garbage_bytes = precollection_heap_bytes - s->live_heap_bytes;
				s->root_pointers = m_roots.size();
				s->live_pointers = s->root_pointers + m_heap_ptrs.size();
				s->root_containers = m_root_containers.size();
			}
		}


		void mark_live_objects() {
			assert(m_to_mark.size() == 0);
			m_to_mark.reserve(m_heap_blocks.size() /* heuristic */);

			// Mark all blocks pointed to by roots.
			for (ptr_iterator it = m_roots.begin();
			     it != m_roots.end();
			     ++it) {
				gc_object_generic_base* p = it->second;
				assert(p);
				m_to_mark.push_back(p);
				//push_heap(m_to_mark);
			}
			// Mark all blocks pointed to by root containers.
			for (container_iterator it_cnt = m_root_containers.begin();
			     it_cnt != m_root_containers.end();
			     ++it_cnt) {
				(*it_cnt)->visit_contained_ptrs();
			}


			// Mark all the floating blocks.
			for (floating_blocks_iterator it = m_floating_blocks.begin();
			     it != m_floating_blocks.end();
			     ++it) {
				assert(*it);
				m_to_mark.push_back(static_cast<gc_object_generic_base*>(*it));
			}

			// Flood-fill all reachable objects.
			while (m_to_mark.size()) {
				//pop_heap(m_to_mark);
				const gc_object_generic_base* b = m_to_mark.back();
				m_to_mark.resize(m_to_mark.size() - 1);

				mark_object(b);
			}
		}

		void mark_object(const gc_object_generic_base* b) {
			heap_block_iterator it = find_containing_block(b);
			assert(it != m_heap_blocks.end());
			assert(it->second.is_address_inside(b));
			if (it->second.mark == false) {
				// Mark this block.
				it->second.mark = true;

				// Find all gc pointers that are inside this heap block.
				// Mark the blocks that they point to.
				ptr_iterator it_ptr = m_heap_ptrs.lower_bound(it->second.p);
				void* block_end = it->second.end();
				while (it_ptr != m_heap_ptrs.end() && it_ptr->first < block_end) {
					gc_object_generic_base* p = it_ptr->second;
					assert(p);
					m_to_mark.push_back(p);
					//push_heap(m_to_mark);
					++it_ptr;
				}

				// Find all gc containers that are inside this heap block.
				{
					container_iterator it_cnt = m_containers.lower_bound((singlethreaded_marksweep::gc_container_base*) it->second.p);
					void* block_end = it->second.end();
					while (it_cnt != m_containers.end() && *it_cnt < block_end) {
						(*it_cnt)->visit_contained_ptrs();
						++it_cnt;
					}
				}
			}
		}

		// Scan through all the heap blocks.  Anything that is
		// not marked is garbage -- delete it!  Anything that
		// is marked is live, and should have its mark
		// cleared.
		void sweep_dead_objects() {
			size_t heap_bytes = 0;

			for (heap_block_iterator it = m_heap_blocks.begin(); it != m_heap_blocks.end(); ) {
				if (it->second.mark == true) {
					// Ham.
					heap_bytes += it->second.sz;
					it->second.mark = false;
					++it;
				} else {
					// Spam.
					gc_object_generic_base* block = it->second.obj;
					assert(block);
					delete block;
					m_heap_blocks.erase(it++);
				}
			}

			m_current_heap_bytes = heap_bytes;
		}

		void clearing_pointer(void* address_of_gc_ptr) {
			assert(address_of_gc_ptr);
			// remove gcp from pointer list
			ptr_iterator it = m_heap_ptrs.find(address_of_gc_ptr);
			if (it != m_heap_ptrs.end()) {
				m_heap_ptrs.erase(it);
			} else {
				it = m_roots.find(address_of_gc_ptr);
				assert(it != m_roots.end());
				m_roots.erase(it);
			}
		}

		heap_block_iterator find_containing_block(const void* p) {
			heap_block_iterator it(m_heap_blocks.upper_bound((void*) p));
			if (it != m_heap_blocks.begin()) {
				--it;
				if (p < it->second.end()) {
					assert(p >= it->second.p);
					// p is inside this heap block.
					return it;
				}
			}
			return m_heap_blocks.end();
		}
		
		// Return true if gcp is inside a heap block.
		bool inside_heap_block(void* gcp) {
			heap_block_iterator it = find_containing_block(gcp);
			if (it != m_heap_blocks.end()) {
				return true;
			}
			return false;
		}

		void initing_pointer(void* address_of_gc_ptr, gc_object_generic_base* object_pointed_to) {
			assert(address_of_gc_ptr);
			assert(object_pointed_to);
			assert(m_roots.find(address_of_gc_ptr) == m_roots.end());
			assert(m_heap_ptrs.find(address_of_gc_ptr) == m_heap_ptrs.end());
		
			// Inside a heap block?
			if (inside_heap_block(address_of_gc_ptr)) {
				m_heap_ptrs.insert(std::make_pair(address_of_gc_ptr, object_pointed_to));
			} else {
				m_roots.insert(std::make_pair(address_of_gc_ptr, object_pointed_to));
			}
		}

		void changing_pointer(void* address_of_gc_ptr, gc_object_generic_base* object_pointed_to) {
			assert(address_of_gc_ptr);
			// remove gcp from pointer list
			ptr_iterator it = m_heap_ptrs.find(address_of_gc_ptr);
			if (it != m_heap_ptrs.end()) {
				it->second = object_pointed_to;
			} else {
				it = m_roots.find(address_of_gc_ptr);
				assert(it != m_roots.end());
				it->second = object_pointed_to;
			}
		}

		void construct_container(singlethreaded_marksweep::gc_container_base* c) {
			if (inside_heap_block(c)) {
				m_containers.insert(c);
			} else {
				m_root_containers.insert(c);
			}
		}

		void destruct_container(singlethreaded_marksweep::gc_container_base* c) {
			container_iterator it = m_containers.find(c);
			if (it != m_containers.end()) {
				m_containers.erase(it);
			} else {
				it = m_root_containers.find(c);
				assert(it != m_root_containers.end());
				m_root_containers.erase(it);
			}
		}

		void visit_contained_ptr(const gc_object_generic_base* obj) {
			m_to_mark.push_back(obj);
		}

	} sm_state;
	
	/*static*/ void singlethreaded_marksweep::get_stats(singlethreaded_marksweep::stats* s) {
		sm_state.get_stats(s);
	}

	/*static*/ void singlethreaded_marksweep::collect_garbage(singlethreaded_marksweep::stats* s) {
		sm_state.collect_garbage(s);
	}
	
	/*static*/ void singlethreaded_marksweep::set_collection_rate(
		int percent_growth_before_next_collection) {
		sm_state.set_collection_rate(percent_growth_before_next_collection);
	}

	/*static*/ void* singlethreaded_marksweep::allocate(size_t sz, block_construction_locker_base* lock) {
		return sm_state.allocate(sz, lock);
	}

	/*static*/ void singlethreaded_marksweep::deallocate(void* p) {
		sm_state.deallocate(p);
	}

	/*static*/ void singlethreaded_marksweep::block_construction_finished(void* block) {
		sm_state.block_construction_finished(block);
	}

	/*static*/ void singlethreaded_marksweep::constructing_gc_object_base(gc_object_generic_base* obj) {
		sm_state.constructing_gc_object_base(obj);
	}

	/*static*/ void singlethreaded_marksweep::clearing_pointer(void* address_of_gc_ptr) {
		sm_state.clearing_pointer(address_of_gc_ptr);
	}

	/*static*/ void singlethreaded_marksweep::initing_pointer(void* address_of_gc_ptr, gc_object_generic_base* object_pointed_to) {
		sm_state.initing_pointer(address_of_gc_ptr, object_pointed_to);
	}

	/*static*/ void singlethreaded_marksweep::changing_pointer(void* address_of_gc_ptr, gc_object_generic_base* object_pointed_to) {
		sm_state.changing_pointer(address_of_gc_ptr, object_pointed_to);
	}

	/*static*/ void singlethreaded_marksweep::construct_container(gc_container_base* c) {
		sm_state.construct_container(c);
	}

	/*static*/ void singlethreaded_marksweep::destruct_container(gc_container_base* c) {
		sm_state.destruct_container(c);
	}

	/*static*/ void singlethreaded_marksweep::visit_contained_ptr(const gc_object_generic_base* obj) {
		sm_state.visit_contained_ptr(obj);
	}

	// In the initial implementation, overhead is pretty high.
	//
	// Tempting optimizations:
	//
	// Use root ref-counts on the blocks (i.e. if a pointer is a
	// root, inc/dec ref counts on the pointed-to block; if a
	// pointer is not a root, don't do ref-counting.  This skips
	// the first step of mark-sweep: no need to mark the blocks
	// pointed to by roots -- just scan through the heap looking
	// for blocks that have a positive ref-count; mark their
	// descendents.
	//
	// * maybe not much of a win -- have to scan the heap to get
	//   the root blocks, instead of walking an internal set.
	//   Though we could keep a linked list of root blocks, and
	//   add/remove blocks when their ref counts change to/from
	//   zero.
	//
	// * not free -- adds a ref count to each block, adds
	//   ref-count inc/dec to each root ptr change, and we still
	//   need a fast way to decide if a gc_ptr is a root.
	//
	// Decide if a gc_ptr is a root at construction time and cache
	// the info -- if it's inside a heap block, it's a heap ptr
	// until the block is deleted; otherwise it's a root.
	//
	// * adds a bit to the gc_ptr.  Bloats the ptr, or else we
	//   stash it in the low bit or something; requires unmasking
	//   on pointer use, and re-masking on pointer change.
	//
	// * still need a list of roots for initial marking.  Either
	//   that, or combine with the ref-counting scheme.
	//
	// Conservative determination of root ptrs -- bloom filter?
	// Still need a way for initial marking.
	//
	// Use doubly-linked list for m_roots?  Should be an easy win.
	// Intrudes into the gc_ptr (ideally), so the thing to do is
	// inject some fields into gc_ptr via a collector-specific
	// base class.
	//
	// How to represent m_heap_ptrs?  The main problem with
	// m_heap_ptrs is: when marking, how to quickly find all heap
	// ptrs inside a newly-marked block?  Currently we use
	// set::lower_bound, which wouldn't work with the linked-list.
	// One good approach is to have a type spec for each block,
	// with a list of pointer offsets for each type spec.  The
	// type spec could be built during object construction and
	// stored in a hash map.  Most type specs would be reused by
	// many object instances, so the storage should be much
	// smaller than currently.  The trick would be making sure
	// it's cheap to create the type specs, though I suppose it
	// would be hard to do worse than what we're already doing.
	//
	// Put the mark bits in the block headers (duh).
	//
	// Put the floating_block bits in the block headers (duh).
	//
	// Replace the m_heap_blocks map with links in the block
	// headers (e.g. maybe a skip list).  This probably saves a
	// good bit of memory compared to using std::map<>.  Also,
	// then some of the searches can cache the last position and
	// proceed from there; this could be a huge win if done right.


	// Sketch of an alternative design:
	//
	// set<memblock*> m_heap_blocks;
	// set<pointer_offset_list> m_pointer_lists;
	// memblock* m_root_list;
	//
	// class memblock {
	//   bool m_mark;
	//   bool m_under_construction;
	//   int m_root_ref_count;
	//   memblock* m_prev_root;
	//   memblock* m_next_root;
	//   pointer_offset_list* m_internal_ptrs;
	//   char data[0];
	// };
	//
	// class gc_ptr {
	//   bool m_is_root;
	//   T* m_ptr;
	// };
	//
	// write_barrier(ptr, new_val) {
	//   if (ptr->m_is_root) {
	//     increment_ref_count(new_val);
	//     decrement_ref_count(ptr->get());
	//   }
	//   ptr->m_ptr = new_val;
	// }
	//
	// block_construction_finished(block) {
	//   add m_internal_ptrs to m_pointer_lists;  // share identical instances
	//   block->m_under_construction = false;
	// }
	//
	// increment_ref_count(block) {
	//   if (block->m_root_ref_count == 0) {
	//     link into root blocks;
	//   }
	//   block->m_root_ref_count++;
	// }
	//
	// decrement_ref_count(block) {
	//   block->m_root_ref_count--;
	//   if (block->m_root_ref_count == 0) {
	//     unlink from root blocks;
	//   }
	// }
	//
	// mark() {
	//   to_mark = addresses of root blocks;
	//   while (to_mark) {
	//     memblock* b = get_containing_block(to_mark.pop_back());
	//     if (!b->m_mark) {
	//       b->m_mark = true;
	//       for (p in b->m_internal_ptrs(b)) {
	//         if (*p) {
	//           to_mark.push_back(*p);
	//         }
	//       }
	//     }
	//   }
	// }
	
}  // tu_gc


