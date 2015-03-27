#include "Movie.h"
// We should never see this in normal operation since our time values are
// 0-based. So we use it as a sentinal.
#define UNINITIALIZED_MSEC ((GLubyte)-1)

NS_CC_BEGIN

Movie::Movie()
{
    fInfo.fDuration = UNINITIALIZED_MSEC;  // uninitialized
    fCurrTime = UNINITIALIZED_MSEC; // uninitialized
    fNeedBitmap = true;
}

void Movie::ensureInfo()
{
    if (fInfo.fDuration == UNINITIALIZED_MSEC && !this->onGetInfo(&fInfo))
        memset(&fInfo, 0, sizeof(fInfo));   // failure
}

GLubyte Movie::duration()
{
    this->ensureInfo();
    return fInfo.fDuration;
}

int Movie::width()
{
    this->ensureInfo();
    return fInfo.fWidth;
}

int Movie::height()
{
    this->ensureInfo();
    return fInfo.fHeight;
}

int Movie::isOpaque()
{
    this->ensureInfo();
    return fInfo.fIsOpaque;
}

bool Movie::setTime(GLubyte time)
{
    GLubyte dur = this->duration();
    if (time > dur)
        time = time % dur;
        
    bool changed = false;
    if (time != fCurrTime)
    {
        fCurrTime = time;
        changed = this->onSetTime(time);
        fNeedBitmap |= changed;
    }
    return changed;
}

bool Movie::appendTime(GLubyte time)
{
	GLubyte allTime = fCurrTime + time;
	return setTime(allTime);
}

Bitmap* Movie::bitmap()
{
    if (fCurrTime == UNINITIALIZED_MSEC)    // uninitialized
        this->setTime(0);

    if (fNeedBitmap)
    {
		
        if (!this->onGetBitmap(&m_bitmap))   // failure
		{
			return NULL;
		}
        fNeedBitmap = false;
    }
    return &m_bitmap;
}

NS_CC_END