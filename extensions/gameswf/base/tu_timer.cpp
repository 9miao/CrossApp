// tu_timer.cpp	-- by Thatcher Ulrich <tu@tulrich.com>

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Utility/profiling timer.

#include <sys/timeb.h>	// for ftime()
#include <assert.h>
#include "tu_timer.h"

static timeb s_start_time;
void tu_timer::init_timer()
{
	ftime(&s_start_time);
}

Uint32 tu_timer::get_ticks()
{
	struct timeb tv;
	ftime(&tv);
	return  (tv.time - s_start_time.time) * 1000 + (tv.millitm - s_start_time.millitm);
}

tu_datetime::tu_datetime()
{
	m_time = (time_t) tu_timer::get_systime();
}

// Returns the date for the specified Date object in milliseconds since midnight on January 1, 1970, 
double tu_datetime::get_time() const
{
	return (double) m_time * 1000;	// *1000 means that time in milliseconds
}

// Sets the date for the specified Date object in milliseconds since midnight on January 1, 1970, 
void tu_datetime::set_time(double t)
{
	m_time = (time_t) (t / 1000);
}

int tu_datetime::get(part part)
{
	struct tm* gmt = localtime(&m_time);
	if (gmt)
	{
		switch (part)
		{
		case FULLYEAR:
			return gmt->tm_year + 1900;
			break;

		case YEAR:
			return gmt->tm_year;
			break;

		case MON:
			return gmt->tm_mon;
			break;

		case MDAY:
			// Returns the day of the month (an integer from 1 to 31)
			return gmt->tm_mday;

		case WDAY:
			// Returns the day of the month (an integer from 1 to 31)
			return gmt->tm_wday;

		case HOUR:
			// Returns the hour (an integer from 0 to 23)
			return gmt->tm_hour;
			break;

		case MIN:
			return gmt->tm_min;
			break;

		case SEC:
			return gmt->tm_sec;
			break;

		default:
			assert(0);
		}
	}
	return -1;
}

void tu_datetime::set(part part, int val)
{
	struct tm* gmt = localtime(&m_time);
	if (gmt)
	{
		switch (part)
		{
		case FULLYEAR:
			gmt->tm_year = val - 1900;	// (current year minus 1900).
			break;

		case YEAR:
			gmt->tm_year = val;
			break;

		case MON:
			gmt->tm_mon = val;
			break;

		case MDAY:
			gmt->tm_mday = val;
			break;

		case WDAY:
			gmt->tm_wday = val;
			break;

		case HOUR:
			gmt->tm_hour = val;
			break;

		case MIN:
			gmt->tm_min = val;
			break;

		case SEC:
			gmt->tm_sec = val;
			break;

		default:
			assert(0);
		}
		m_time = mktime(gmt);
	}
}


Uint64 tu_timer::get_systime()
{
	time_t ltime;
	time(&ltime);
	return (Uint64) ltime;
}

#ifdef _WIN32

#include <windows.h>

void tu_timer::sleep(int milliseconds)
{
	::Sleep(milliseconds);
}


uint64	tu_timer::get_profile_ticks()
{
	// @@ use rdtsc?

	LARGE_INTEGER	li;
	QueryPerformanceCounter(&li);

	return li.QuadPart;
}


double	tu_timer::profile_ticks_to_seconds(uint64 ticks)
{
	LARGE_INTEGER	freq;
	QueryPerformanceFrequency(&freq);

	double	seconds = (double) ticks;
	seconds /= (double) freq.QuadPart;

	return seconds;
}

#else	// not _WIN32


#include <sys/time.h>
#include <unistd.h>


// The profile ticks implementation is just fine for a normal timer.

void tu_timer::sleep(int milliseconds)
{
	usleep(milliseconds * 1000);
}


uint64	tu_timer::get_profile_ticks()
{
	// @@ TODO prefer rdtsc when available?

	// Return microseconds.
	struct timeval tv;
	uint64 result;
	
	gettimeofday(&tv, 0);

	result = tv.tv_sec * 1000000;
	result += tv.tv_usec;
	
	return result;
}


double	tu_timer::profile_ticks_to_seconds(uint64 ticks)
{
	// ticks is microseconds.  Convert to seconds.
	return ticks / 1000000.0;
}

double	tu_timer::profile_ticks_to_milliseconds(uint64 ticks)
{
	// ticks is microseconds.  Convert to milliseconds.
	return ticks / 1000.0;
}

#endif	// not _WIN32


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
