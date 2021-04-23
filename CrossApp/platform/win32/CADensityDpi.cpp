

#include "platform/CADensityDpi.h"
#include "CCEGLView.h"
NS_CC_BEGIN

static float s_fDPI = -1.f;

void CADensityDpi::setDensityDpi(float dpi)
{
    s_fDPI = dpi;
}

float CADensityDpi::getDensityDpi()
{
    if (s_fDPI > 0)
    {
        return s_fDPI;
    }
    return DPI_SIMULATOR;
}

CADeviceIdiom CADensityDpi::getIdiom()
{
    return CADeviceIdiom::iPhone;
}

NS_CC_END
