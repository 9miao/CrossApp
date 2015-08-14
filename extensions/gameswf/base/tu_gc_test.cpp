// tu_gc_test.cpp  -- Thatcher Ulrich <http://tulrich.com> 2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some test code for garbage collectors.
//
// cl -Zi -GX -GR tu_gc_test.cpp tu_gc_singlethreaded_marksweep.cpp container.cpp utf8.cpp -I.. -DTEST_GC


#ifdef TEST_GC

#include "tu_gc_singlethreaded_marksweep.h"
#include "tu_gc_singlethreaded_refcount.h"
#include <map>
#include <stdio.h>
#include <set>
#include <vector>
#include "container.h"

namespace test_ms {
#define GC_COLLECTOR tu_gc::singlethreaded_marksweep
#include "tu_gc_test_impl.h"
#undef GC_COLLECTOR
}  // test_ms

namespace test_rc {
#define GC_COLLECTOR tu_gc::singlethreaded_refcount
#include "tu_gc_test_impl.h"
#undef GC_COLLECTOR
}  // test_rc
	
int main() {
	printf("\nmark-sweep:\n\n");
	test_ms::run_tests();

	printf("\nref-counting:\n\n");
	test_rc::run_tests();

	return 0;
}


#endif  // TEST_GC
