// tu_mutex.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2009

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#include "gameswf_mutex.h"

#if TU_CONFIG_LINK_TO_THREAD == 1

namespace gameswf
{
	int sdl_thread_start_func(void* ptr)
	{
		((tu_thread*) ptr)->start();
		return 0;
	};

	tu_thread::tu_thread(void (*fn)(void *), void* data)
	{
		IF_VERBOSE_ACTION(log_msg("SDL thread is started\n"));
		m_func = fn;
		m_arg = data;
		m_thread = SDL_CreateThread(sdl_thread_start_func, this);
		assert(m_thread);
	}

	tu_thread::~tu_thread()
	{
		kill();
	}

	void tu_thread::wait()
	{
		SDL_WaitThread(m_thread, NULL);
		m_thread = NULL;
	}

	void tu_thread::kill()
	{
		SDL_KillThread(m_thread);
		m_thread = NULL;
	}

	void tu_thread::start()
	{
		(m_func)(m_arg);
	}

	tu_mutex::tu_mutex()
	{
		m_mutex = SDL_CreateMutex();
	}

	tu_mutex::~tu_mutex() 
	{
		SDL_DestroyMutex(m_mutex);
	}

	void tu_mutex::lock() 
	{
		SDL_LockMutex(m_mutex);
	}

	void tu_mutex::unlock() 
	{
		SDL_UnlockMutex(m_mutex);
	}

	tu_condition::tu_condition()
	{
		m_cond = SDL_CreateCond();
	}

	tu_condition::~tu_condition()
	{
		SDL_DestroyCond(m_cond);
	}

	void tu_condition::wait()
	{
		m_cond_mutex.lock();
		SDL_CondWait(m_cond, m_cond_mutex.m_mutex);
	}

	void tu_condition::signal()
	{
		SDL_CondSignal(m_cond);
	}
}

#elif TU_CONFIG_LINK_TO_THREAD == 2	// libpthread

#include <pthread.h>

namespace gameswf
{
	void* pthread_start_func(void* ptr)
	{
		((tu_thread*) ptr)->start();
		pthread_exit(NULL);
		return NULL;
	};

	tu_thread::tu_thread(void (*fn)(void *), void* data)
	{
		IF_VERBOSE_ACTION(log_msg("pthread is started\n"));
		m_func = fn;
		m_arg = data;
		if (pthread_create(&m_thread, NULL, pthread_start_func, this))
		{
			log_msg("Couldn't create the pthread\n");
		}
	}

	tu_thread::~tu_thread()
	{
		kill();
	}

	void tu_thread::wait()
	{
		// blocks the calling thread until the specified threadid thread terminates. 
		pthread_join(m_thread, NULL);
	}

	void tu_thread::kill()
	{
		//pthread_cancel(m_thread);
	}

	void tu_thread::start()
	{
		(m_func)(m_arg);
	}

	tu_mutex::tu_mutex()
	{
		pthread_mutex_init(&m_mutex, NULL);
	}

	tu_mutex::~tu_mutex() 
	{
		pthread_mutex_destroy(&m_mutex);
	}

	void tu_mutex::lock() 
	{
		pthread_mutex_lock(&m_mutex);
	}

	void tu_mutex::unlock() 
	{
		pthread_mutex_unlock(&m_mutex);
	}

	tu_condition::tu_condition()
	{
		pthread_cond_init(&m_cond, NULL);
	}

	tu_condition::~tu_condition()
	{
		pthread_cond_destroy(&m_cond);
	}

	void tu_condition::wait()
	{
		m_cond_mutex.lock();
		pthread_cond_wait(&m_cond, &m_cond_mutex.m_mutex);
		m_cond_mutex.unlock();
	}

	void tu_condition::signal()
	{
		pthread_cond_signal(&m_cond);
	}
}

#else

// no multithreads

#endif	// TU_CONFIG_LINK_TO_THREAD
