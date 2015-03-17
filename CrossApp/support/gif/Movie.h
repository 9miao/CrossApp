#ifndef Movie_DEFINED
#define Movie_DEFINED
#include <stdint.h>
#include "gif_lib/gif_lib.h"
#include <stdio.h>
#include <string.h>
#include "Bitmap.h"

class Movie{
public:
    uint32_t  duration();
    int     width();
    int     height();
    int     isOpaque();
    
    /** Specify the time code (between 0...duration) to sample a bitmap
        from the movie. Returns true if this time code generated a different
        bitmap/frame from the previous state (i.e. true means you need to
        redraw).
    */
    bool setTime(uint32_t);

	bool appendTime(uint32_t time);

    // return the right bitmap for the current time code
    Bitmap* bitmap();
    
protected:
    struct Info {
        uint32_t  fDuration;
        int     fWidth;
        int     fHeight;
        bool    fIsOpaque;
    };

    virtual bool onGetInfo(Info*) = 0;
    virtual bool onSetTime(uint32_t) = 0;
    virtual bool onGetBitmap(Bitmap*) = 0;

    // visible for subclasses
    Movie();
	Bitmap m_bitmap;
private:
    Info        fInfo;
    uint32_t    fCurrTime;
    bool        fNeedBitmap;
    
    void ensureInfo();
	
};

#endif
