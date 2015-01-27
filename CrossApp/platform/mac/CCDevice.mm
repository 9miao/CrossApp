#include "platform/CCDevice.h"

NS_CC_BEGIN

int CCDevice::getDPI()
{
//TODO: 
    return 160;
}

CCDeviceIdiom CCDevice::getIdiom()
{
	return CCDeviceIdiomPhone;
}

NS_CC_END
