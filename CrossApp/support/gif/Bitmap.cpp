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
    m_data = static_cast<unsigned char*>(malloc(m_width * m_hight * sizeof(unsigned int)));
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

unsigned long Bitmap::getPixelLenth()
{
	return m_width*m_hight;
};

const unsigned char* Bitmap::getRGBA()
{
	if(m_data == NULL)
	{
		return NULL;
	}
	return m_data;
}

void Bitmap::eraseColor(const CAColor4B& color)
{
    unsigned short r, g, b, a = color.a;
    
	// make rgb premultiplied
	if (255 != a)
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

	for (unsigned long i = 0; i < m_width * m_hight * 4; i+=4)
    {
        *(m_data + i * 4) = r;
        *(m_data + i * 4 + 1) = g;
        *(m_data + i * 4 + 2) = b;
        *(m_data + i * 4 + 3) = a;
    }
}

Bitmap* Bitmap::getDebugBitmap()
{
	Bitmap* bitmap = new Bitmap;
	bitmap->m_width = 64;
	bitmap->m_hight = 64;

	bitmap->allocateBitmap();
	for(unsigned int hight =0; hight < bitmap->m_hight; hight++ )
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

		for(unsigned int width = 0; width < bitmap->m_width; width++)
		{
            int index = (hight * bitmap->m_width + width) * 4;
            bitmap->m_data[index] = color.r;
            bitmap->m_data[index + 1] = color.r;
            bitmap->m_data[index + 2] = color.r;
            bitmap->m_data[index + 3] = color.r;
		}

	}

	return bitmap;
}

unsigned char* Bitmap::getAddr(int left, int top, int offset)
{
	return &m_data[(top * m_width + left) * 4 + offset];
}

void Bitmap::swap(Bitmap* toSwap)
{
	TSwap(this->m_data, toSwap->m_data);
	TSwap(this->m_width, toSwap->m_width);
	TSwap(this->m_hight, toSwap->m_hight);
}

NS_CC_END