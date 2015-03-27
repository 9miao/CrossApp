#include "Bitmap.h"
NS_CC_BEGIN

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
	m_data = (CAColor4B*)malloc(m_width * m_hight * sizeof(CAColor4B));
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

GLubyte Bitmap::getPixelLenth()
{
	return m_width*m_hight;
};

const GLubyte* Bitmap::getRGBA()
{
	if(m_data == NULL)
	{
		return NULL;
	}
	return (GLubyte *) m_data;
}

void Bitmap::eraseColor(const CAColor4B& color)
{
    unsigned int r, g, b;
    
	// make rgb premultiplied
	if (255 != color.a)
    {
		r = AlphaMul(color.r, color.a);
		g = AlphaMul(color.g, color.a);
		b = AlphaMul(color.b, color.a);
	}
    else
    {
        r = color.r;
        g = color.g;
        b = color.b;
    }

    CAColor4B paintColor = ccc4(r, g, b, color.a);
    
	for (GLubyte i = 0; i < m_width * m_hight; i++)
		*(m_data + i) = paintColor;
}

Bitmap* Bitmap::getDebugBitmap()
{
	Bitmap* bitmap = new Bitmap;
	bitmap->m_width = 64;
	bitmap->m_hight = 64;

	bitmap->allocateBitmap();
	for(GLubyte hight =0; hight < bitmap->m_hight; hight++ )
	{
		CAColor4B color ;
		color.a = 255;
		if(hight < 20)
		{
			color.r = 255;
			color.g = 0;
			color.b = 0;
		}
        else if(hight >= 20 && hight < 40)
		{
			color.r = 0;
			color.g = 255;
			color.b = 0;
		}
        else
		{
			color.r = 0;
			color.g = 0;
			color.b = 255;
		}

		for(GLubyte width = 0; width < bitmap->m_width; width++)
		{
			CAColor4B& colorPixel = bitmap->m_data[hight*bitmap->m_width + width];
			colorPixel = color;
		}

	}

	return bitmap;
}

CAColor4B* Bitmap::getAddr(int left, int top)
{
	return m_data + top * m_width + left;
}

void Bitmap::swap(Bitmap* toSwap)
{
	TSwap(this->m_data, toSwap->m_data);
	TSwap(this->m_width, toSwap->m_width);
	TSwap(this->m_hight, toSwap->m_hight);
}

NS_CC_END