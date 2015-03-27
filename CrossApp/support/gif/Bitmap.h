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
	
	GLubyte m_width;
    
	GLubyte m_hight;

	Bitmap();
    
	virtual ~Bitmap();
    
	void allocateBitmap();
    
	void resetBitmap();
    
	bool hasData();
    
	bool isValid();
    
	GLubyte getPixelLenth();
    
	const GLubyte* getRGBA();
    
	CAColor4B* getAddr(int left, int top);
    
	void swap(Bitmap*);
    
	void eraseColor(const CAColor4B& color);
    
	static Bitmap* getDebugBitmap();
    
private:
    
	CAColor4B* m_data;
};

struct FrameData
{
	GLubyte m_duration;
	GLubyte m_index;
	FrameData(GLubyte duration, GLubyte index)
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