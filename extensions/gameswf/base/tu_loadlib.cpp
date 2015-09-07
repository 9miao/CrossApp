// tu_loadlib.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// plugin loader

#include "tu_types.h"
#include "container.h"
#include "tu_loadlib.h"

#ifdef _WIN32

#include <windows.h>

tu_loadlib::tu_loadlib(const char* library_name) :
	m_hlib(NULL)
{
	tu_string path = library_name;
	path += ".dll";
	//qiao xin test
	//m_hlib = LoadLibrary(path.c_str());
}

tu_loadlib::~tu_loadlib()
{
	if (m_hlib)
	{ 
		FreeLibrary(m_hlib);
	}
}

void* tu_loadlib::get_function(const char* function_name)
{
	if (m_hlib)
	{
		return (void*) GetProcAddress(m_hlib, function_name);
	}
	return NULL;
}

#else	// not _WIN32


#include <dlfcn.h>

tu_loadlib::tu_loadlib(const char* library_name) :
	m_hlib(NULL)
{
	tu_string path = "lib";
	path += library_name;
	m_hlib = dlopen(path.c_str(), RTLD_LAZY);
	if (m_hlib == NULL)
	{
		// try .so
		path += ".so";
		m_hlib = dlopen(path.c_str(), RTLD_LAZY);
		if (m_hlib == NULL)
		{
			printf("can't load shared library '%s'\n", library_name);
		}
	}
}

tu_loadlib::~tu_loadlib()
{
	if (m_hlib)
	{
		dlclose(m_hlib);
	}
}

void* tu_loadlib::get_function(const char* function_name)
{
	if (m_hlib)
	{
		return dlsym(m_hlib, function_name);
	}
	return NULL;
}

#endif	// not _WIN32


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
