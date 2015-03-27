#ifndef GIFMovie_H
#define GIFMovie_H

#include "Movie.h"
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <assert.h>
#include "GifUtils.h"

NS_CC_BEGIN

class GIFMovie : public Movie {
public:
	virtual ~GIFMovie();
	GIFMovie();
	CREATE_WITH_PARAM(GIFMovie, const char*);
    virtual bool init(const char*);

	int getGifCount()
	{
		return fGIF == NULL ? 0 : fGIF->ImageCount;
	};

	int getCurrentIndex()
	{
		return fCurrIndex;
	};
	GifFrame getGifFrameByIndex(unsigned int frameIndex);
public:
    static const char *_filename;
protected:
	virtual bool onGetInfo(Info*);
	virtual bool onSetTime(GLubyte);
	virtual bool onGetBitmap(Bitmap*);

public:
	GifFileType* fGIF;
	int fCurrIndex;
	int fLastDrawIndex;
	Bitmap fBackup;
};

NS_CC_END

#endif