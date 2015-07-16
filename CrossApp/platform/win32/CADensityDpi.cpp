

#include "platform/CADensityDpi.h"
#include "basics/CAApplication.h"
#include "CCEGLView.h"
NS_CC_BEGIN

float CADensityDpi::getDensityDpi()
{
    float relust = 320.0f;
    
    return relust;
}

CADeviceIdiom CADensityDpi::getIdiom()
{
    return CADeviceIdiomPhone;
}

NS_CC_END