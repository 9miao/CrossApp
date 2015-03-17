#include "Bitmap.h"

Bitmap::Bitmap()
{
	m_width = 0;
	m_hight = 0;
	m_data = NULL;
};

Bitmap::~Bitmap()
{
	resetBitmap();
};

void Bitmap::allocateBitmap()
{
	assert(m_data == NULL && m_width * m_hight > 0);
	m_data = (Rgba*)malloc(m_width * m_hight * sizeof(Rgba));
};

void Bitmap::resetBitmap()
{
	if(m_data)
	{
		free(m_data);
		m_data = NULL;
	}
};

bool Bitmap::hasData()
{
	return m_data != NULL;
}

bool Bitmap::isValid()
{
	return m_width > 0 && m_hight >0 && hasData();
}

uint32_t Bitmap::getPixelLenth()
{
	return m_width*m_hight;
};

const uint32_t* Bitmap::getRGBA()
{
	if(m_data == NULL)
	{
		return NULL;
	}
	return (uint32_t *) m_data;
}

void Bitmap::eraseColor(Rgba color)
{
	Rgba paintColor = color;

	// make rgb premultiplied
	if (255 != color.alpha) {
		paintColor.red = AlphaMul(color.red, color.alpha);
		paintColor.green = AlphaMul(color.green, color.alpha);
		paintColor.blue = AlphaMul(color.blue, color.alpha);
	}

	for (uint32_t i = 0; i < m_width * m_hight; i++)
		*(m_data + i) = paintColor;
}

Bitmap* Bitmap::getDebugBitmap()
{
	Bitmap* bitmap = new Bitmap;
	bitmap->m_width = 64;
	bitmap->m_hight = 64;

	bitmap->allocateBitmap();
	for(uint32_t hight =0; hight < bitmap->m_hight; hight++ )
	{
		Rgba color ;
		color.alpha = 255;
		if(hight < 20)
		{
			color.red = 255;
			color.green = 0;
			color.blue = 0;
		}else if(hight >= 20 && hight < 40)
		{
			color.red = 0;
			color.green = 255;
			color.blue = 0;
		}else
		{
			color.red = 0;
			color.green = 0;
			color.blue = 255;
		}

		for(uint32_t width = 0; width < bitmap->m_width; width++)
		{
			Rgba& colorPixel = bitmap->m_data[hight*bitmap->m_width + width];
			colorPixel = color;
		}

	}

	return bitmap;
}

Rgba* Bitmap::getAddr(int left, int top)
{
	return m_data + top * m_width + left;
}

void Bitmap::swap(Bitmap* toSwap)
{
	TSwap(this->m_data, toSwap->m_data);
	TSwap(this->m_width, toSwap->m_width);
	TSwap(this->m_hight, toSwap->m_hight);
}