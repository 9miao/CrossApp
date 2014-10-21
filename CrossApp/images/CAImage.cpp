
#include "CAImage.h"
#include "ccConfig.h"
#include "ccMacros.h"
#include "platform/platform.h"
#include "platform/CCImage.h"
#include "CCGL.h"
#include "CCStdC.h"
#include "support/ccUtils.h"
#include "support/zip_support/ZipUtils.h"
#include "platform/CCPlatformMacros.h"
#include "platform/CCPlatformConfig.h"
#include "platform/CCFileUtils.h"
#include "CAImage.h"
#include "CAImageCache.h"
#include "basics/CAApplication.h"
#include "shaders/CAGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CAShaderCache.h"
#include <ctype.h>
#include <cctype>
#include "platform/CAFTFontCache.h"

#if CC_ENABLE_CACHE_TEXTURE_DATA
    #include "CAImageCache.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

NS_CC_BEGIN

//CLASS IMPLEMENTATIONS:

// If the image has alpha, you can create RGBA8 (32-bit) or RGBA4 (16-bit) or RGB5A1 (16-bit)
// Default is: RGBA8888 (32-bit textures)
static CAImagePixelFormat g_defaultAlphaPixelFormat = kCAImagePixelFormat_Default;

static CAImage* cc_white_image = NULL;

CAImage::CAImage()
: m_uPixelsWide(0)
, m_uPixelsHigh(0)
, m_uName(0)
, m_fMaxS(0.0)
, m_fMaxT(0.0)
, m_bHasPremultipliedAlpha(false)
, m_bHasMipmaps(false)
, m_pShaderProgram(NULL)
, m_bMonochrome(false)
, m_pData(NULL)
, m_nDataLenght(0)
{
    
}

CAImage::~CAImage()
{
#if CC_ENABLE_CACHE_TEXTURE_DATA
    VolatileTexture::removeImage(this);
#endif

    CCLOGINFO("CrossApp: deallocing CAImage %u.", m_uName);
    CC_SAFE_RELEASE(m_pShaderProgram);
    
    if(m_uName)
    {
        ccGLDeleteTexture(m_uName);
    }
}

CAImage* CAImage::create(const std::string& file)
{
	return CAImageCache::sharedImageCache()->addImage(file);
}

CAImage* CAImage::createWithDataNoCache(void* data, int lenght)
{
    CAImage* image = new CAImage();
    if (image && image->initWithData(data, lenght))
    {
        image->autorelease();
        return image;
    }
    CC_SAFE_DELETE(image);
    return NULL;
}

CAImage* CAImage::createWithData(void* data, int lenght, const std::string& key)
{
    CAImage* image = CAImage::createWithDataNoCache(data, lenght);
    if (image)
    {
        CAImageCache::sharedImageCache()->setImageForKey(image, key.c_str());
    }
    return image;
}

CAImage*  CAImage::createWithString(const char *text, const char *fontName, float fontSize, const CCSize& dimensions, CATextAlignment hAlignment, 
	CAVerticalTextAlignment vAlignment, bool bWordWrap, int iLineSpacing, bool bBold, bool bItalics)
{
	return g_AFTFontCache.initWithString(text, fontName, fontSize, dimensions.width, dimensions.height, hAlignment, vAlignment, bWordWrap, iLineSpacing, bBold, bItalics);
}

int CAImage::getFontHeight(const char* pFontName, unsigned long nSize)
{
	return g_AFTFontCache.getFontHeight(pFontName, nSize);
}

int CAImage::getStringWidth(const char* pFontName, unsigned long nSize, const std::string& pText)
{
    return g_AFTFontCache.getStringWidth(pFontName, nSize, pText);
}

int CAImage::cutStringByWidth(const char* pFontName, unsigned long nSize, const std::string& text, int iLimitWidth, int& cutWidth)
{
	return g_AFTFontCache.cutStringByWidth(pFontName, nSize, text, iLimitWidth, cutWidth);
}

int CAImage::getStringHeight(const char* pFontName, unsigned long nSize, const std::string& pText, int iLimitWidth, int iLineSpace, bool bWordWrap)
{
	return g_AFTFontCache.getStringHeight(pFontName, nSize, pText, iLimitWidth, iLineSpace, bWordWrap);
}

const CAImagePixelFormat& CAImage::getPixelFormat()
{
    return m_ePixelFormat;
}

unsigned int CAImage::getPixelsWide()
{
    return m_uPixelsWide;
}

unsigned int CAImage::getPixelsHigh()
{
    return m_uPixelsHigh;
}

GLuint CAImage::getName()
{
    return m_uName;
}

CCSize CAImage::getContentSize()
{

    CCSize ret;
    ret.width = m_tContentSize.width / CC_CONTENT_SCALE_FACTOR();
    ret.height = m_tContentSize.height / CC_CONTENT_SCALE_FACTOR();
    
    return ret;
}

const CCSize& CAImage::getContentSizeInPixels()
{
    return m_tContentSize;
}

GLfloat CAImage::getMaxS()
{
    return m_fMaxS;
}

void CAImage::setMaxS(GLfloat maxS)
{
    m_fMaxS = maxS;
}

GLfloat CAImage::getMaxT()
{
    return m_fMaxT;
}

void CAImage::setMaxT(GLfloat maxT)
{
    m_fMaxT = maxT;
}

CAGLProgram* CAImage::getShaderProgram(void)
{
    return m_pShaderProgram;
}

void CAImage::setShaderProgram(CAGLProgram* pShaderProgram)
{
    CC_SAFE_RETAIN(pShaderProgram);
    CC_SAFE_RELEASE(m_pShaderProgram);
    m_pShaderProgram = pShaderProgram;
}

void CAImage::releaseData(void *data)
{
    free(data);
}

void* CAImage::keepData(void *data, unsigned int length)
{
    CC_UNUSED_PARAM(length);
    //The image data mustn't be saved because it isn't a mutable image.
    return data;
}

bool CAImage::hasPremultipliedAlpha()
{
    return m_bHasPremultipliedAlpha;
}

bool CAImage::initWithData(void* data, int lenght)
{
    bool bRet = false;
    CCImage* image = new CCImage();
    bRet = image->initWithImageData(data, lenght);
    if (bRet)
    {
        this->initWithImage(image);
    }
    image->release();
    return bRet;
}

bool CAImage::initWithData(const void *data, CAImagePixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const CCSize& contentSize)
{
    unsigned int bitsPerPixel;
    //Hack: bitsPerPixelForFormat returns wrong number for RGB_888 textures. See function.
    if(pixelFormat == kCAImagePixelFormat_RGB888)
    {
        bitsPerPixel = 24;
    }
    else
    {
        bitsPerPixel = bitsPerPixelForFormat(pixelFormat);
    }

    unsigned int bytesPerRow = pixelsWide * bitsPerPixel / 8;

    if(bytesPerRow % 8 == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
    }
    else if(bytesPerRow % 4 == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
    else if(bytesPerRow % 2 == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    }
    else
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }


    glGenTextures(1, &m_uName);
    ccGLBindTexture2D(m_uName);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    // Specify OpenGL texture image

    switch(pixelFormat)
    {
    case kCAImagePixelFormat_RGBA8888:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        break;
    case kCAImagePixelFormat_RGB888:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        break;
    case kCAImagePixelFormat_RGBA4444:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data);
        break;
    case kCAImagePixelFormat_RGB5A1:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, data);
        break;
    case kCAImagePixelFormat_RGB565:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
        break;
    case kCAImagePixelFormat_AI88:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
        break;
    case kCAImagePixelFormat_A8:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
        break;
    case kCAImagePixelFormat_I8:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
        break;
    default:
        CCAssert(0, "NSInternalInconsistencyException");

    }
    m_pData = ((unsigned char*)const_cast<char*>((const char*)data));
    m_nDataLenght = (unsigned long)pixelsWide * pixelsHigh;
    m_tContentSize = contentSize;
    m_uPixelsWide = pixelsWide;
    m_uPixelsHigh = pixelsHigh;
    m_ePixelFormat = pixelFormat;
    m_fMaxS = contentSize.width / (float)(pixelsWide);
    m_fMaxT = contentSize.height / (float)(pixelsHigh);

    m_bHasPremultipliedAlpha = false;
    m_bHasMipmaps = false;

    setShaderProgram(CAShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));

    return true;
}


const char* CAImage::description(void)
{
    return CCString::createWithFormat("<CAImage | Name = %u | Dimensions = %u x %u | Coordinates = (%.2f, %.2f)>", m_uName, m_uPixelsWide, m_uPixelsHigh, m_fMaxS, m_fMaxT)->getCString();
}

bool CAImage::initWithImage(CCImage *uiImage)
{
    if (uiImage == NULL)
    {
        CCLOG("CrossApp: CAImage. Can't create Texture. UIImage is nil");
        return false;
    }
    
    unsigned int imageWidth = uiImage->getWidth();
    unsigned int imageHeight = uiImage->getHeight();
    

    unsigned maxTextureSize = 16384;
    if (imageWidth > maxTextureSize || imageHeight > maxTextureSize) 
    {
        CCLOG("CrossApp: WARNING: Image (%u x %u) is bigger than the supported %u x %u", imageWidth, imageHeight, maxTextureSize, maxTextureSize);
        return false;
    }
    
    // always load premultiplied images
    return initPremultipliedATextureWithImage(uiImage, imageWidth, imageHeight);
}

bool CAImage::initPremultipliedATextureWithImage(CCImage *image, unsigned int width, unsigned int height)
{
    unsigned char*            tempData = image->getData();
    unsigned int*             inPixel32  = NULL;
    unsigned char*            inPixel8 = NULL;
    unsigned short*           outPixel16 = NULL;
    bool                      hasAlpha = image->hasAlpha();
    CCSize                    imageSize = CCSizeMake((float)(image->getWidth()), (float)(image->getHeight()));
    CAImagePixelFormat    pixelFormat;
    size_t                    bpp = image->getBitsPerComponent();

    // compute pixel format
    if (hasAlpha)
    {
    	pixelFormat = g_defaultAlphaPixelFormat;
    }
    else
    {
        if (bpp >= 8)
        {
            pixelFormat = kCAImagePixelFormat_RGB888;
        }
        else 
        {
            pixelFormat = kCAImagePixelFormat_RGB565;
        }
        
    }
    
    // Repack the pixel data into the right format
    unsigned int length = width * height;

    if (pixelFormat == kCAImagePixelFormat_RGB565)
    {
        if (hasAlpha)
        {
            // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGGBBBBB"
            
            tempData = new unsigned char[width * height * 2];
            outPixel16 = (unsigned short*)tempData;
            inPixel32 = (unsigned int*)image->getData();
            
            for(unsigned int i = 0; i < length; ++i, ++inPixel32)
            {
                *outPixel16++ = 
                ((((*inPixel32 >>  0) & 0xFF) >> 3) << 11) |  // R
                ((((*inPixel32 >>  8) & 0xFF) >> 2) << 5)  |  // G
                ((((*inPixel32 >> 16) & 0xFF) >> 3) << 0);    // B
            }
        }
        else 
        {
            // Convert "RRRRRRRRRGGGGGGGGBBBBBBBB" to "RRRRRGGGGGGBBBBB"
            
            tempData = new unsigned char[width * height * 2];
            outPixel16 = (unsigned short*)tempData;
            inPixel8 = (unsigned char*)image->getData();
            
            for(unsigned int i = 0; i < length; ++i)
            {
                *outPixel16++ = 
                (((*inPixel8++ & 0xFF) >> 3) << 11) |  // R
                (((*inPixel8++ & 0xFF) >> 2) << 5)  |  // G
                (((*inPixel8++ & 0xFF) >> 3) << 0);    // B
            }
        }    
    }
    else if (pixelFormat == kCAImagePixelFormat_RGBA4444)
    {
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRGGGGBBBBAAAA"
        
        inPixel32 = (unsigned int*)image->getData();  
        tempData = new unsigned char[width * height * 2];
        outPixel16 = (unsigned short*)tempData;
        
        for(unsigned int i = 0; i < length; ++i, ++inPixel32)
        {
            *outPixel16++ = 
            ((((*inPixel32 >> 0) & 0xFF) >> 4) << 12) | // R
            ((((*inPixel32 >> 8) & 0xFF) >> 4) <<  8) | // G
            ((((*inPixel32 >> 16) & 0xFF) >> 4) << 4) | // B
            ((((*inPixel32 >> 24) & 0xFF) >> 4) << 0);  // A
        }
    }
    else if (pixelFormat == kCAImagePixelFormat_RGB5A1)
    {
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGBBBBBA"
        inPixel32 = (unsigned int*)image->getData();   
        tempData = new unsigned char[width * height * 2];
        outPixel16 = (unsigned short*)tempData;
        
        for(unsigned int i = 0; i < length; ++i, ++inPixel32)
        {
            *outPixel16++ = 
            ((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | // R
            ((((*inPixel32 >> 8) & 0xFF) >> 3) <<  6) | // G
            ((((*inPixel32 >> 16) & 0xFF) >> 3) << 1) | // B
            ((((*inPixel32 >> 24) & 0xFF) >> 7) << 0);  // A
        }
    }
    else if (pixelFormat == kCAImagePixelFormat_A8)
    {
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "AAAAAAAA"
        inPixel32 = (unsigned int*)image->getData();
        tempData = new unsigned char[width * height];
        unsigned char *outPixel8 = tempData;
        
        for(unsigned int i = 0; i < length; ++i, ++inPixel32)
        {
            *outPixel8++ = (*inPixel32 >> 24) & 0xFF;  // A
        }
    }
    
    if (hasAlpha && pixelFormat == kCAImagePixelFormat_RGB888)
    {
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRRRRGGGGGGGGBBBBBBBB"
        inPixel32 = (unsigned int*)image->getData();
        tempData = new unsigned char[width * height * 3];
        unsigned char *outPixel8 = tempData;
        
        for(unsigned int i = 0; i < length; ++i, ++inPixel32)
        {
            *outPixel8++ = (*inPixel32 >> 0) & 0xFF; // R
            *outPixel8++ = (*inPixel32 >> 8) & 0xFF; // G
            *outPixel8++ = (*inPixel32 >> 16) & 0xFF; // B
        }
    }
    
    initWithData(tempData, pixelFormat, width, height, imageSize);
    
    if (tempData != image->getData())
    {
        delete [] tempData;
    }

    m_bHasPremultipliedAlpha = image->isPremultipliedAlpha();
    return true;
}


// implementation CAImage (Drawing)

void CAImage::drawAtPoint(const CCPoint& point)
{
    GLfloat    coordinates[] = {    
        0.0f,    m_fMaxT,
        m_fMaxS,m_fMaxT,
        0.0f,    0.0f,
        m_fMaxS,0.0f };

    GLfloat    width = (GLfloat)m_uPixelsWide * m_fMaxS,
        height = (GLfloat)m_uPixelsHigh * m_fMaxT;

    GLfloat        vertices[] = {    
        point.x,            point.y,
        width + point.x,    point.y,
        point.x,            height  + point.y,
        width + point.x,    height  + point.y };

    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );
    m_pShaderProgram->use();
    m_pShaderProgram->setUniformsForBuiltins();

    ccGLBindTexture2D( m_uName );


#ifdef EMSCRIPTEN
    setGLBufferData(vertices, 8 * sizeof(GLfloat), 0);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0);

    setGLBufferData(coordinates, 8 * sizeof(GLfloat), 1);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, 0);
#else
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, coordinates);
#endif // EMSCRIPTEN

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void CAImage::drawInRect(const CCRect& rect)
{
    GLfloat    coordinates[] = {    
        0.0f,    m_fMaxT,
        m_fMaxS,m_fMaxT,
        0.0f,    0.0f,
        m_fMaxS,0.0f };

    GLfloat    vertices[] = {    rect.origin.x,        rect.origin.y,                            /*0.0f,*/
        rect.origin.x + rect.size.width,        rect.origin.y,                            /*0.0f,*/
        rect.origin.x,                            rect.origin.y + rect.size.height,        /*0.0f,*/
        rect.origin.x + rect.size.width,        rect.origin.y + rect.size.height,        /*0.0f*/ };

    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );
    m_pShaderProgram->use();
    m_pShaderProgram->setUniformsForBuiltins();

    ccGLBindTexture2D( m_uName );

#ifdef EMSCRIPTEN
    setGLBufferData(vertices, 8 * sizeof(GLfloat), 0);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0);

    setGLBufferData(coordinates, 8 * sizeof(GLfloat), 1);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, 0);
#else
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, coordinates);
#endif // EMSCRIPTEN
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

bool CAImage::initWithETCFile(const char* file)
{
    bool bRet = false;
    // nothing to do with CAObject::init
    
    CAImageETC *etc = new CAImageETC;
    bRet = etc->initWithFile(file);
    
    if (bRet)
    {
        m_uName = etc->getName();
        m_fMaxS = 1.0f;
        m_fMaxT = 1.0f;
        m_uPixelsWide = etc->getWidth();
        m_uPixelsHigh = etc->getHeight();
        m_tContentSize = CCSizeMake((float)m_uPixelsWide, (float)m_uPixelsHigh);
        m_bHasPremultipliedAlpha = true;
        
        etc->release();
    }
    else
    {
        CCLOG("CrossApp: Couldn't load ETC image %s", file);
    }
    
    return bRet;
}

void CAImage::generateMipmap()
{
    CCAssert( m_uPixelsWide == ccNextPOT(m_uPixelsWide) && m_uPixelsHigh == ccNextPOT(m_uPixelsHigh), "Mipmap texture only works in POT textures");
    ccGLBindTexture2D( m_uName );
    glGenerateMipmap(GL_TEXTURE_2D);
    m_bHasMipmaps = true;
}

bool CAImage::hasMipmaps()
{
    return m_bHasMipmaps;
}

void CAImage::setTexParameters(ccTexParams *texParams)
{
    CCAssert( (m_uPixelsWide == ccNextPOT(m_uPixelsWide) || texParams->wrapS == GL_CLAMP_TO_EDGE) &&
        (m_uPixelsHigh == ccNextPOT(m_uPixelsHigh) || texParams->wrapT == GL_CLAMP_TO_EDGE),
        "GL_CLAMP_TO_EDGE should be used in NPOT dimensions");

    ccGLBindTexture2D( m_uName );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texParams->minFilter );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texParams->magFilter );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texParams->wrapS );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texParams->wrapT );

#if CC_ENABLE_CACHE_TEXTURE_DATA
    VolatileTexture::setTexParameters(this, texParams);
#endif
}

void CAImage::setAliasTexParameters()
{
    ccGLBindTexture2D( m_uName );

    if( ! m_bHasMipmaps )
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    }
    else
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
    }

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
#if CC_ENABLE_CACHE_TEXTURE_DATA
    ccTexParams texParams = {m_bHasMipmaps?GL_NEAREST_MIPMAP_NEAREST:GL_NEAREST,GL_NEAREST,GL_NONE,GL_NONE};
    VolatileTexture::setTexParameters(this, &texParams);
#endif
}

void CAImage::setAntiAliasTexParameters()
{
    ccGLBindTexture2D( m_uName );

    if( ! m_bHasMipmaps )
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    }
    else
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    }

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
#if CC_ENABLE_CACHE_TEXTURE_DATA
    ccTexParams texParams = {m_bHasMipmaps?GL_LINEAR_MIPMAP_NEAREST:GL_LINEAR,GL_LINEAR,GL_NONE,GL_NONE};
    VolatileTexture::setTexParameters(this, &texParams);
#endif
}

const char* CAImage::stringForFormat()
{
	switch (m_ePixelFormat) 
	{
		case kCAImagePixelFormat_RGBA8888:
			return  "RGBA8888";

		case kCAImagePixelFormat_RGB888:
			return  "RGB888";

		case kCAImagePixelFormat_RGB565:
			return  "RGB565";

		case kCAImagePixelFormat_RGBA4444:
			return  "RGBA4444";

		case kCAImagePixelFormat_RGB5A1:
			return  "RGB5A1";

		case kCAImagePixelFormat_AI88:
			return  "AI88";

		case kCAImagePixelFormat_A8:
			return  "A8";

		case kCAImagePixelFormat_I8:
			return  "I8";
            
		default:
			CCAssert(false , "unrecognized pixel format");
			CCLOG("stringForFormat: %ld, cannot give useful result", (long)m_ePixelFormat);
			break;
	}

	return  NULL;
}

//
// Texture options for images that contains alpha
//
// implementation CAImage (PixelFormat)

void CAImage::setDefaultAlphaPixelFormat(CAImagePixelFormat format)
{
    g_defaultAlphaPixelFormat = format;
}

CAImagePixelFormat CAImage::defaultAlphaPixelFormat()
{
    return g_defaultAlphaPixelFormat;
}

unsigned int CAImage::bitsPerPixelForFormat(CAImagePixelFormat format)
{
	unsigned int ret=0;

	switch (format) {
		case kCAImagePixelFormat_RGBA8888:
			ret = 32;
			break;
		case kCAImagePixelFormat_RGB888:
			// It is 32 and not 24, since its internal representation uses 32 bits.
			ret = 32;
			break;
		case kCAImagePixelFormat_RGB565:
			ret = 16;
			break;
		case kCAImagePixelFormat_RGBA4444:
			ret = 16;
			break;
		case kCAImagePixelFormat_RGB5A1:
			ret = 16;
			break;
		case kCAImagePixelFormat_AI88:
			ret = 16;
			break;
		case kCAImagePixelFormat_A8:
			ret = 8;
			break;
		case kCAImagePixelFormat_I8:
			ret = 8;
			break;
		default:
			ret = -1;
			CCAssert(false , "unrecognized pixel format");
			CCLOG("bitsPerPixelForFormat: %ld, cannot give useful result", (long)format);
			break;
	}
	return ret;
}

unsigned int CAImage::bitsPerPixelForFormat()
{
	return this->bitsPerPixelForFormat(m_ePixelFormat);
}

bool CAImage::saveToFile(const std::string& fullPath)
{
    FILE *fp = fopen(fullPath.c_str(), "wb+");
    
	if (!fp) return false;
    
    fwrite(m_pData, sizeof(char), m_nDataLenght, fp);
	fclose(fp);
    
    return true;
}

float CAImage::getAspectRatio()
{
    return m_tContentSize.width / m_tContentSize.height;
}

const char* CAImage::getImageFileType()
{
    const char* text = NULL;
    
    unsigned char p = m_pData[0] ;
    
    switch (p)
    {
        case 0xFF:
            text = "jpeg";
            break;
        case 0x89:
            text = "png";
            break;
        case 0x47:
            text = "gif";
            break;
        case 0x49:
        case 0x4D:
            text = "tiff";
        default:
            break;
    }
    return text;
}

CAImage* CAImage::CC_WHITE_IMAGE()
{
    if (cc_white_image == NULL)
    {
        int pixels[16][16];
        for (int i=0; i<16; i++)
        {
            for (int j=0; j<16; j++)
            {
                pixels[i][j] = 0xffffffff;
            }
        }
        
        CCImage* image = new CCImage();
        image->autorelease();
        image->initWithImageData(pixels, sizeof(pixels), CCImage::kFmtRawData, 16, 16, 8);
        cc_white_image = CAImageCache::sharedImageCache()->addUIImage(image, "CC_WHITE_IMAGE");
        cc_white_image->retain();
        cc_white_image->m_bMonochrome = true;
    }
    return cc_white_image;
}



CAImageETC::CAImageETC()
: _name(0)
, _width(0)
, _height(0)
{}

CAImageETC::~CAImageETC()
{
}

bool CAImageETC::initWithFile(const char *file)
{
    // Only Android supports ETC file format
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    bool ret = loadTexture(CCFileUtils::sharedFileUtils()->fullPathForFilename(file).c_str());
    return ret;
#else
    return false;
#endif
}

unsigned int CAImageETC::getName() const
{
    return _name;
}

unsigned int CAImageETC::getWidth() const
{
    return _width;
}

unsigned int CAImageETC::getHeight() const
{
    return _height;
}

// Call back function for java
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define  LOG_TAG    "CAImageETC.cpp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

static unsigned int sWidth = 0;
static unsigned int sHeight = 0;
static unsigned char *sData = NULL;
static unsigned int sLength = 0;

extern "C"
{
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_Cocos2dxETCLoader_nativesetImageInfo(JNIEnv* env, jobject thiz, jint width, jint height, jbyteArray data, jint dataLength)
    {
        sWidth = (unsigned int)width;
        sHeight = (unsigned int)height;
        sLength = dataLength;
        sData = new unsigned char[sLength];
        env->GetByteArrayRegion(data, 0, sLength, (jbyte*)sData);
    }
}
#endif

bool CAImageETC::loadTexture(const char* file)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/CrossApp/lib/Cocos2dxETCLoader", "loadTexture", "(Ljava/lang/String;)Z"))
    {
        jstring stringArg1 = t.env->NewStringUTF(file);
        jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg1);
        
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
        
        if (ret)
        {
            _width = sWidth;
            _height = sHeight;
            
            
            glGenTextures(1, &_name);
            glBindTexture(GL_TEXTURE_2D, _name);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            
            glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, _width, _height, 0, sLength, sData);
            
            glBindTexture(GL_TEXTURE_2D, 0);
            
            delete [] sData;
            sData = NULL;
            
            GLenum err = glGetError();
            if (err != GL_NO_ERROR)
            {
                LOGD("width %d, height %d, lenght %d", _width, _height, sLength);
                LOGD("CrossApp: TextureETC: Error uploading compressed texture %s glError: 0x%04X", file, err);
                return false;
            }
            
            return true;
        }
        else
        {
            return false;
        }
    }
#else
    return false;
#endif
}


NS_CC_END
