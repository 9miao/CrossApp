

#include "CAMotionManager.h"
#include "basics/CAScheduler.h"
#include "basics/CAApplication.h"
NS_CC_BEGIN

CAMotionManager* CAMotionManager::getInstance()
{
    return CAApplication::getApplication()->getMotionManager();
}

CAMotionManager::CAMotionManager()
{

}

CAMotionManager::~CAMotionManager()
{

}

void CAMotionManager::startGyroscope(const CAMotionManager::Callback& callback)
{

}

void CAMotionManager::setGyroInterval(float interval)
{
    
}

void CAMotionManager::stopGyroscope()
{

}

NS_CC_END


