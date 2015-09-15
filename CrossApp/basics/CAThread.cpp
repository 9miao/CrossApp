
#include "CAThread.h"


NS_CC_BEGIN



CALock::CALock()
{
	pthread_mutex_init(&m_cMutex, NULL);
}

CALock::~CALock()
{
	pthread_mutex_destroy(&m_cMutex);
}

void CALock::Lock()
{
	pthread_mutex_lock(&m_cMutex);
}

void CALock::UnLock()
{
	pthread_mutex_unlock(&m_cMutex);
}


CAThread::CAThread()
: m_bIsRunning(false)
, m_pThreadFunc(NULL)
{
	pthread_mutex_init(&m_SleepMutex, NULL);
	pthread_cond_init(&m_SleepCondition, NULL);
}

CAThread::~CAThread()
{
	close();
	pthread_mutex_destroy(&m_SleepMutex);
	pthread_cond_destroy(&m_SleepCondition);
}

void CAThread::start()
{
	m_ThreadRunType = ThreadRunDirectly;
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
	pthread_create(&m_hThread, NULL, _ThreadProc, this);
#endif
}

void CAThread::startAndWait(ThreadProcFunc func)
{
	m_ThreadRunType = ThreadRunNotify;
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
	pthread_create(&m_hThread, NULL, _ThreadProc, this);
#endif
	m_pThreadFunc = func;
}

void CAThread::notifyRun(void* param)
{
	m_ThreadDataQueue.AddElement(param);
}

void CAThread::close()
{
	if (m_bIsRunning)
	{
		m_bIsRunning = false;
		pthread_cond_wait(&m_SleepCondition, &m_SleepMutex);
		pthread_detach(m_hThread);
	}
}

void CAThread::closeAtOnce()
{
	m_ThreadDataQueue.Clear();
	close();
}

bool CAThread::isRunning()
{
	return m_bIsRunning;
}

void* CAThread::_ThreadProc(void* lpParameter)
{
	CAThread *pAThread = (CAThread*)lpParameter;
	CCAssert(pAThread != NULL, "");
	
	pAThread->m_bIsRunning = true;
	pAThread->OnInitInstance();

	pthread_mutex_lock(&pAThread->m_SleepMutex);
	while (pAThread->m_bIsRunning)
	{
		if (pAThread->m_ThreadRunType == ThreadRunDirectly)
		{
			pAThread->OnRunning();
		}
		else if (pAThread->m_ThreadRunType == ThreadRunNotify)
		{
			void* param = NULL;
			if (pAThread->m_ThreadDataQueue.PopElement(param))
			{
				if (pAThread->m_pThreadFunc)
				{
					if (!pAThread->m_pThreadFunc(param))
						break;
				}
			}
			else
			{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
				Sleep(5);
#else
				usleep(5000);
#endif
			}
		}
		else break;
	}
	pthread_mutex_unlock(&pAThread->m_SleepMutex);
	pAThread->OnExitInstance();
	pAThread->m_bIsRunning = false;
	pthread_cond_signal(&pAThread->m_SleepCondition);
	return 0;
}

NS_CC_END