

#include "CCThread.h"

// iOS and Mac already has a CCThread.mm
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)

NS_CC_BEGIN

CCThread::~CCThread()
{

}

void CCThread::createAutoreleasePool()
{

}

NS_CC_END

#endif