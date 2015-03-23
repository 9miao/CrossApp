#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>
#include <assert.h>
#include <cstdlib>
#include "GifMacros.h"

#define ColorType uint8_t
#define UNINITIALIZED_UINT ((uint32_t)-1)

class Rgba
{
public:
	Rgba()
	{
		blue = 0;
		green = 0;
		red = 0;
		alpha = 0;
	}
	Rgba(ColorType ialpha, ColorType ired, ColorType igreen, ColorType iblue)
	{
		alpha = ialpha;
		red = ired;
		green = igreen;
		blue = iblue;
	}

	Rgba(Rgba& color)
	{
		alpha = color.alpha;
		blue = color.blue;
		green = color.green;
		red = color.red;
	}

	void setColor(ColorType ialpha, ColorType ired, ColorType igreen, ColorType iblue)
	{
		alpha = ialpha;
		red = ired;
		green = igreen;
		blue = iblue;
	}
	ColorType red;
	ColorType green;
	ColorType blue;
	ColorType alpha;
};

class Bitmap{

public:
	
	uint32_t m_width;
	uint32_t m_hight;

	Bitmap();
	virtual ~Bitmap();
	void allocateBitmap();
	void resetBitmap();
	bool hasData();
	bool isValid();
	uint32_t getPixelLenth();
	const uint32_t* getRGBA();
	Rgba* getAddr(int left, int top);
	void swap(Bitmap*);
	void eraseColor(Rgba color);
	static Bitmap* getDebugBitmap();
private:
	Rgba* m_data;
};

struct FrameData
{
	uint32_t m_duration;
	uint32_t m_index;
	FrameData(uint32_t duration, uint32_t index)
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

#endif