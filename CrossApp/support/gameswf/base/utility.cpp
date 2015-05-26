// utility.cpp	-- by Thatcher Ulrich <tu@tulrich.com>

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Various little utility functions, macros & typedefs.

#include "utility.h"
#include "dlmalloc.h"

#ifdef _WIN32
#ifndef NDEBUG

int	tu_testbed_assert_break(const char* filename, int linenum, const char* expression)
{
	// @@ TODO output print error message
	__asm { int 3 }
	return 0;
}

#endif // not NDEBUG
#endif // _WIN32


#ifdef USE_DL_MALLOC

// Overrides of new/delete that use Doug Lea's malloc.  Very helpful
// on certain lame platforms.

void*	operator new(size_t size)
{
	return dlmalloc(size);
}

void	operator delete(void* ptr)
{
	if (ptr) dlfree(ptr);
}

void*	operator new[](size_t size)
{
	return dlmalloc(size);
}

void	operator delete[](void* ptr)
{
	if (ptr) dlfree(ptr);
}

#endif // USE_DL_MALLOC


void dump_memory_stats(const char *from, int line, const char *label) 
// Dump the internal statistics from malloc() so we can track memory leaks
{

// This doesn't compile on Windows.
#if  !defined(_WIN32) && !defined(__APPLE_CC__)
  
// This does not work with DMALLOC, since the internal data structures
// differ.
#ifndef USE_DMALLOC

	struct mallinfo mi;
	static int allocated = 0;
	static int freeb = 0;
  
	mi = mallinfo();
	if (label != 0) {
		printf("Malloc Statistics from %s() (line #%d): %s\n", from, line, label);
	} else { 
		printf("Malloc Statistics from %s() (line #%d):\n", from, line);
	}
  
	//printf("\tnon-mapped space from system:  %d\n", mi.arena);
	printf("\ttotal allocated space:         %d\n", mi.uordblks);
	printf("\ttotal free space:              %d\n", mi.fordblks);
	//printf("\tspace in mmapped regions:      %d\n", mi.hblkhd);
	//printf("\ttop-most, releasable space:    %d\n", mi.keepcost); // Prints 78824
	if (freeb != mi.fordblks) {
		printf("\t%d bytes difference in free space.\n", freeb - mi.fordblks);
		freeb = mi.fordblks;
	}

        UNUSED(allocated);
	//if (allocated != mi.uordblks) {
	//  printf("\t%d bytes difference in allocated space.\n", mi.uordblks - allocated);
	//  allocated = mi.uordblks;
	//}  

#endif  // not USE_DMALLOC

#endif  // not _WIN32
}
