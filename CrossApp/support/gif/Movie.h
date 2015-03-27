#ifndef Movie_DEFINED
#define Movie_DEFINED
#include <stdint.h>
#include "gif_lib/gif_lib.h"
#include <stdio.h>
#include <string.h>
#include "Bitmap.h"

NS_CC_BEGIN

class Movie{
public:
    GLubyte  duration();
    int     width();
    int     height();
    int     isOpaque();
    
    /** Specify the time code (between 0...duration) to sample a bitmap
        from the movie. Returns true if this time code generated a different
        bitmap/frame from the previous state (i.e. true means you need to
        redraw).
    */
    bool setTime(GLubyte);

	bool appendTime(GLubyte time);

    // return the right bitmap for the current time code
    Bitmap* bitmap();
    
protected:
    struct Info {
        GLubyte  fDuration;
        int     fWidth;
        int     fHeight;
        bool    fIsOpaque;
    };

    virtual bool onGetInfo(Info*) = 0;
    virtual bool onSetTime(GLubyte) = 0;
    virtual bool onGetBitmap(Bitmap*) = 0;

    // visible for subclasses
    Movie();
	Bitmap m_bitmap;
private:
    Info        fInfo;
    GLubyte    fCurrTime;
    bool        fNeedBitmap;
    
    void ensureInfo();
	
};

NS_CC_END

#endif
