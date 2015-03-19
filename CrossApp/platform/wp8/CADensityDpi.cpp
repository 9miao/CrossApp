

#include "platform/CADensityDpi.h"
NS_CC_BEGIN

float CADensityDpi::getDensityDpi()
{
    return 320.0f;
}

CADeviceIdiom CADensityDpi::getIdiom()
{
    return CADeviceIdiomPhone;
}

NS_CC_END