#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>
#include <assert.h>
#include <cstdlib>
#include "GifMacros.h"
#include "ccTypes.h"

NS_CC_BEGIN

#define UNINITIALIZED_UINT ((GLubyte)-1)

class CC_DLL Bitmap
{

public:
	
	unsigned int m_width;
    
	unsigned int m_hight;

	Bitmap();
    
	virtual ~Bitmap();
    
	void allocateBitmap();
    
	void resetBitmap();
    
	bool hasData();
    
	bool isValid();
    
	unsigned long getPixelLenth();
    
	const unsigned char* getRGBA();
    
	unsigned char* getAddr(int left, int top, int offset);
    
	void swap(Bitmap*);
    
	void eraseColor(const CAColor4B& color);
    
	static Bitmap* getDebugBitmap();
    
private:
    
	unsigned char* m_data;
};

struct FrameData
{
	unsigned int m_duration;
	unsigned int m_index;
	FrameData(unsigned int duration, unsigned int index)
	{
		m_duration = duration;
		m_index = index;
	}
};

class GifFrame
{
public:
	GifFrame()
	:m_frameData(UNINITIALIZED_UINT, UNINITIALIZED_UINT)
	{
	}
	Bitmap* m_bm;
	FrameData m_frameData;
};

NS_CC_END
#endif