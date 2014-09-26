
#include "CCThread.h"

NS_CC_BEGIN

CCThread::~CCThread()
{
    [(id)m_pAutoreasePool release];
}

void CCThread::createAutoreleasePool()
{
    m_pAutoreasePool = [[NSAutoreleasePool alloc] init];
}

NS_CC_END
