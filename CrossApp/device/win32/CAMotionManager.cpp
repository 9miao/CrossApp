
#include "../CAMotionManager.h"
#include "basics/CAApplication.h"

NS_CC_BEGIN

CAMotionManager::CAMotionManager()
{
}

CAMotionManager::~CAMotionManager()
{
}

CAMotionManager* CAMotionManager::getInstance()
{
	return CAApplication::getApplication()->getMotionManager();
}

void CAMotionManager::startGyroscope(const std::function<void(const CAMotionManager::Data&)>& callback)
{

}

void CAMotionManager::setGyroInterval(float interval)
{
}

void CAMotionManager::stopGyroscope()
{
}

NS_CC_END

