//
//  CASyncQueue.h
//  CrossApp
//
//  Created by Zhujian on 14-9-1.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//
#ifndef __CrossApp_CASyncQueue__
#define __CrossApp_CASyncQueue__

#include "ccMacros.h"
#include <queue>
#include <vector>
#include <pthread.h>

NS_CC_BEGIN

class CC_DLL CALock
{
public:
	CALock();
	~CALock();
    
	void Lock();
	void UnLock();
    
private:
	pthread_mutex_t m_cMutex;
};

class CC_DLL CAAutoLock
{
public:
	explicit CAAutoLock(CALock& lock)
    : m_cLock(lock)
	{
		m_cLock.Lock();
	}
	~CAAutoLock()
	{
		m_cLock.UnLock();
	}
private:
	CALock& m_cLock;
};

template <typename T>
class CASyncQueue
{
public:
	virtual ~CASyncQueue(void) {}

	void AddElement(T v)
	{
		CAAutoLock lock(m_Cs);
		m_SyncQueue.push(v);
	}
	bool PopElement(T& v)
	{
		CAAutoLock lock(m_Cs);
		if (!m_SyncQueue.empty())
		{
			v = m_SyncQueue.front();
			m_SyncQueue.pop();
			return true;
		}
		return false;
	}
	std::vector<T> GetQueueElements()
	{
		CAAutoLock lock(m_Cs);

		std::vector<T> aList;
		while (!m_SyncQueue.empty())
		{
			aList.push_back(m_SyncQueue.front());
			m_SyncQueue.pop();
		}
		return aList;
	}
	int GetCount()
	{
		CAAutoLock lock(m_Cs);
		return (int)(m_SyncQueue.size());
	}
	void Clear()
	{
		CAAutoLock lock(m_Cs);
		while (!m_SyncQueue.empty())
			m_SyncQueue.pop();
	}

private:
	CALock m_Cs;
	std::queue<T> m_SyncQueue;
};

NS_CC_END

#endif    // __CrossApp_CAThread__
