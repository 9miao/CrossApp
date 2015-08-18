// tu_memdebug.h	-- by Thatcher Ulrich <tu@tulrich.com>

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#ifndef TU_MEMDEBUG_H
#define TU_MEMDEBUG_H

#if _DEBUG && _WIN32

	// Only valid in the following environment: Intel platform, MS VC++ 5/6/7/7.1/8
	#ifdef USE_STACKWALKER

	#include <windows.h>
	#include "Stackwalker.h"

	namespace tu_memdebug
	{
		void open()
		{
			InitAllocCheck(ACOutput_XML);
		}

		void close()
		{
			DeInitAllocCheck();
		}
	}

	#else

	// use VC++ memory leaks debugger

	#ifndef _CRTDBG_MAP_ALLOC
	#	define _CRTDBG_MAP_ALLOC
	#endif
	#include <stdlib.h>
	#include <crtdbg.h>

	namespace tu_memdebug
	{
		void open()
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	
	//		_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
		}

		void close()
		{
	//		_CrtDumpMemoryLeaks();
		}
	}

	#endif	// USE_STACKWALKER

#else	// _DEBUG && _WIN32

// no memory leaks debugger

namespace tu_memdebug
{
	void open() {}
	void close() {}
}

#endif

#endif	// TU_MEMDEBUG_H

