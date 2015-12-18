//
//  CAThread.h
//  CrossApp
//
//  Created by Zhujian on 14-9-1.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __CrossApp_CAThread__
#define __CrossApp_CAThread__

#include "platform/CCPlatformMacros.h"
#include "ccMacros.h"
#include "CASyncQueue.h"
#include <pthread.h>

NS_CC_BEGIN


enum ThreadRunType
{
	ThreadRunDirectly,
	ThreadRunNotify
};

typedef bool (*ThreadProcFunc)(void* lpParameter);

class CC_DLL CAThread
{
public:
	CAThread();
	virtual ~CAThread();

	void start();
	void startAndWait(ThreadProcFunc func);
	void notifyRun(void* param);
	void clear(bool bFree=false);
	void close();
	void closeAtOnce();
	void setMaxMsgCount(int v);

	bool isRunning();

	//-- put the initialization code here.
	virtual void OnInitInstance() {}

	//-- put the main code of the thread here.
	virtual void OnRunning() {}

	//-- put the cleanup code here.
	virtual void OnExitInstance() {}

private:
	static void* _ThreadProc(void* lpParameter);
	
	pthread_t m_hThread;

	pthread_mutex_t m_SleepMutex;
	pthread_cond_t m_SleepCondition;

	int m_iMaxMsgCount;
	CASyncQueue<void*> m_ThreadDataQueue;

	ThreadProcFunc m_pThreadFunc;

	bool m_bIsRunning;

	ThreadRunType m_ThreadRunType;
};


NS_CC_END

#endif    // __CrossApp_CAThread__
