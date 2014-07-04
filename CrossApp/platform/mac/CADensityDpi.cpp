

#include "platform/CADensityDpi.h"
#include "basics/CAApplication.h"
#include "CCEGLView.h"
NS_CC_BEGIN

float CADensityDpi::getDensityDpi()
{
    float relust = 320.0f;
    
    CCSize size = CCEGLView::sharedOpenGLView()->getFrameSize();
    
    if (size.width > size.height)
    {
        relust *= size.height / 640.0f;
    }
    else
    {
        relust *= size.width / 640.0f;
    }
    
    return relust;
}


NS_CC_END