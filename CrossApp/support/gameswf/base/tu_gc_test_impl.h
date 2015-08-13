// tu_gc_test_impl.h  -- Thatcher Ulrich <http://tulrich.com> 2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some inline implementation of garbage collector test functions.
// This is designed to be a nested include, parameterized by the macro
// GC_COLLECTOR.

DECLARE_GC_TYPES(GC_COLLECTOR);

void collect_and_dump_stats() {
	gc_collector::stats s;
	gc_collector::collect_garbage(&s);
	printf("%d %d %d %d %d\n", s.live_heap_bytes, s.garbage_bytes, s.root_pointers, s.live_pointers, s.root_containers);
}

void dump_stats() {
	gc_collector::stats s;
	gc_collector::get_stats(&s);
	printf("%d %d %d %d %d\n", s.live_heap_bytes, s.garbage_bytes, s.root_pointers, s.live_pointers, s.root_containers);
}

struct cons : public gc_object {
public:
	cons() : value(0) {
	}
	cons(int val, cons* cdr_in) : value(val), cdr(cdr_in) {
	}
	cons(cons* car_in, cons* cdr_in) : value(0), car(car_in), cdr(cdr_in) {
	}

	int value;
	gc_ptr<cons> car;
	gc_ptr<cons> cdr;
};

gc_ptr<cons> build_list_of_numbers(int low, int high) {
	if (low <= high) {
		return new cons(low, build_list_of_numbers(low + 1, high).get());
	} else {
		return NULL;
	}
}

// Classic cycle; ref-counting would not be able to collect this.
gc_ptr<cons> build_cycle(int size) {
	gc_ptr<cons> head = new cons(-1, NULL);
	gc_ptr<cons> tail = head;
	for (int i = 0; i < size - 1; i++) {
		head = new cons(i, head.get());
	}
	tail->cdr = head;  // close the loop.
	return head;
}

void dump_sub(const gc_ptr<cons>& node, std::set<cons*>* visited) {
	if (node == NULL) {
		printf("nil");
		return;
	}

	if (visited->find(node.get()) != visited->end()) {
		// We've been here before; don't loop infinitely.
		printf("cycle");
		return;
	}
	visited->insert(node.get());
	
	if (node->car == NULL) {
		printf("%d", node->value);
	} else {
		printf("( ");
		dump_sub(node->car, visited);
		printf(" )");
	}
	if (node->cdr == NULL) {
		// nada
	} else {
		printf(" . (");
		dump_sub(node->cdr, visited);
		printf(")");
	}
}

void dump(const gc_ptr<cons>& node) {
	std::set<cons*> visited;
	dump_sub(node, &visited);
	printf("\n");
}

void test_basic_stuff() {
	gc_ptr<cons> root1 = build_list_of_numbers(1, 10);
	gc_ptr<cons> root2 = build_cycle(20);

	dump(root1);
	dump(root2);

	dump_stats();
 	printf("collecting...\n");
	collect_and_dump_stats();

	dump(root1);
	dump(root2);

	root1 = NULL;

 	printf("collecting...\n");
	collect_and_dump_stats();

	dump(root1);
	dump(root2);

	root2 = NULL;
	
	printf("collecting...\n");
	collect_and_dump_stats();
}

// test multiple inheritance (objects & mix-ins virtually inherit from gc_object)

struct actor : virtual public gc_object {
	gc_ptr<actor> m_parent;
};

// a "mix-in"
struct space_physics : virtual public actor {
};

struct vehicle : virtual public actor {
};

// another "mix-in"
struct steerable : virtual public vehicle {
};

struct spaceship : virtual public space_physics, virtual public steerable {
};

struct tractor_beam : virtual public actor {
	gc_ptr<vehicle> m_target;
};

void test_multiple_inheritance() {
	printf("\nmultiple inheritance\n");

	collect_and_dump_stats();

	gc_ptr<actor> ship = new spaceship;
	gc_ptr<tractor_beam> beam = new tractor_beam;
	beam->m_target = dynamic_cast<vehicle*>(ship.get());

	collect_and_dump_stats();

	ship = NULL;

	collect_and_dump_stats();

	beam = NULL;

	collect_and_dump_stats();
}

// test gc_container
SPECIALIZE_GC_CONTAINER(gc_vector, std::vector);
SPECIALIZE_GC_CONTAINER(swf_array, array);

struct bag : public gc_object {
	gc_vector<gc_ptr<bag> > m_ptrs;
};

struct bag2 : public gc_object {
	swf_array<<gc_ptr<bag2> > m_ptrs;
};

template<class bag_type>
void test_gc_container() {
	printf("\ncontainer\n");
	
	gc_ptr<bag_type> b = new bag_type;
	b->m_ptrs.push_back(new bag_type);
	b->m_ptrs.push_back(new bag_type);
	b->m_ptrs.push_back(new bag_type);
	b->m_ptrs[1]->m_ptrs.push_back(new bag_type);
	b->m_ptrs[1]->m_ptrs[0]->m_ptrs.push_back(b.get());

	collect_and_dump_stats();

	// mark-sweep should correctly collect the group of objects here.
	// ref-counted should leak, because there is a cycle.
	b = NULL;

	collect_and_dump_stats();

	// Containers should act like roots when they are not inside a
	// gc_object.
	printf("\ncontainer 2\n");
	gc_vector<gc_ptr<cons> > c;
	c.push_back(build_list_of_numbers(10, 20).get());
	c.push_back(build_list_of_numbers(20, 30).get());

	collect_and_dump_stats();

	c.clear();

	collect_and_dump_stats();
}

// test gc_pair_container

SPECIALIZE_GC_PAIR_CONTAINER(gc_hash_map, hash);
SPECIALIZE_GC_PAIR_CONTAINER(gc_map, std::map);

struct pairbag : public gc_object {
	gc_hash_map<int, gc_ptr<pairbag> > m_ptrs;
};

struct pairbag2 : public gc_object {
	gc_map<int, gc_ptr<pairbag2> > m_ptrs;
};

template<class bag_type>
void test_gc_map_container() {
	printf("\nmap_container\n");

	gc_ptr<bag_type> b = new bag_type;
	b->m_ptrs[100] = new bag_type;
	b->m_ptrs[200] = new bag_type;
	b->m_ptrs[300] = new bag_type;
	b->m_ptrs[200]->m_ptrs[27] = new bag_type;
	b->m_ptrs[200]->m_ptrs[27]->m_ptrs[100] = b.get();

	collect_and_dump_stats();

	b = NULL;

	collect_and_dump_stats();
}

void test_gc_map_container_root() {
	// Containers should act like roots when they are not inside a
	// gc_object.
	printf("\nmap_container_root\n");
	gc_map<int, gc_ptr<cons> > c;
	c[100] = build_cycle(5).get();
	c[200] = build_cycle(10).get();

	collect_and_dump_stats();

	c.clear();

	collect_and_dump_stats();
}

struct pairbag3 : public gc_object {
	gc_hash_map<gc_ptr<pairbag3>, int> m_ptrs;
};

template<class bag_type>
void test_gc_map_container2() {
	printf("\nmap_container2\n");

	gc_ptr<bag_type> b = new bag_type;
	b->m_ptrs[b.get()] = 27;
	assert(b->m_ptrs[b.get()] == 27);

	collect_and_dump_stats();

	b = NULL;

	collect_and_dump_stats();
}

// test weak_ptr

struct weak_pointee : public gc_object, public weak_pointee_mixin {
	weak_pointee() : m_x(0) {
	}
	
	int m_x;
};

void test_weak_ptr() {
	printf("\nweak_ptr\n");

	gc_ptr<weak_pointee> p = new weak_pointee;
	gameswf::weak_ptr<weak_pointee> wp = p.get();
	assert(wp.get_ptr());

	if (gc_ptr<weak_pointee> p2 = wp.get_ptr()) {
		assert(p2->m_x == 0);
	} else {
		assert(0);  // wp should not be NULL because p is a live reference.
	}

	p = NULL;
	gc_collector::collect_garbage(NULL);

	// The object should have been collected; weak pointer should
	// now be NULL.
	if (gc_ptr<weak_pointee> p2 = wp.get_ptr()) {
		assert(0);
	}
}

void run_tests() {
	test_basic_stuff();
	test_multiple_inheritance();
	test_gc_container<bag>();
	test_gc_container<bag2>();
	test_gc_map_container<pairbag>();
	test_gc_map_container<pairbag2>();
	test_gc_map_container2<pairbag3>();
	test_gc_map_container_root();
	test_weak_ptr();
}
