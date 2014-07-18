
#include "CAImage.h"
#include "ccConfig.h"
#include "ccMacros.h"
#include "basics/CAConfiguration.h"
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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "platform/CAFreeTypeFont.h"
#endif

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

// By default PVR images are treated as if they don't have the alpha channel premultiplied
static bool PVRHaveAlphaPremultiplied_ = false;

static CAImage* cc_white_image = NULL;

CAImage::CAImage()
: m_bPVRHaveAlphaPremultiplied(true)
, m_uPixelsWide(0)
, m_uPixelsHigh(0)
, m_uName(0)
, m_fMaxS(0.0)
, m_fMaxT(0.0)
, m_bHasPremultipliedAlpha(false)
, m_bHasMipmaps(false)
, m_pShaderProgram(NULL)
, m_bMonochrome(false)
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

CAImage* CAImage::create(const char* file)
{
	return CAImageCache::sharedImageCache()->addImage(file);
}

CAImage*  CAImage::createWithString(const char *text, const char *fontName, float fontSize, const CCSize& dimensions, CATextAlignment hAlignment, CAVerticalTextAlignment vAlignment)
{
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    
    CAFreeTypeFont cFreeTypeFont;
    
	CAImage* image = cFreeTypeFont.initWithString(text,
                                         fontName,
                                         fontSize,
                                         dimensions.width,
                                         dimensions.height,
                                         hAlignment,vAlignment);
    return image;
    
#else
    
    CAImage* image = new CAImage();
    image->autorelease();
    
    bool bRet = false;
    
    CCImage::ETextAlign eAlign;
    
    if (CAVerticalTextAlignmentTop == vAlignment)
    {
        eAlign = (CATextAlignmentCenter == hAlignment) ? CCImage::kAlignTop
        : (CATextAlignmentLeft == hAlignment) ? CCImage::kAlignTopLeft : CCImage::kAlignTopRight;
    }
    else if (CAVerticalTextAlignmentCenter == vAlignment)
    {
        eAlign = (CATextAlignmentCenter == hAlignment) ? CCImage::kAlignCenter
        : (CATextAlignmentLeft == hAlignment) ? CCImage::kAlignLeft : CCImage::kAlignRight;
    }
    else if (CAVerticalTextAlignmentBottom == vAlignment)
    {
        eAlign = (CATextAlignmentCenter == hAlignment) ? CCImage::kAlignBottom
        : (CATextAlignmentLeft == hAlignment) ? CCImage::kAlignBottomLeft : CCImage::kAlignBottomRight;
    }
    else
    {
        CCAssert(false, "Not supported alignment format!");
        return false;
    }
    
    do
    {
        CCImage* pImage = new CCImage();
        CC_BREAK_IF(NULL == pImage);
        bRet = pImage->initWithString(text,
                                      (int)dimensions.width,
                                      (int)dimensions.height,
                                      eAlign,
                                      fontName,
                                      (int)fontSize);
        CC_BREAK_IF(!bRet);
        image->initWithImage(pImage);
        CC_SAFE_RELEASE(pImage);
        
    } while (0);
    
    return image;
    
#endif
}

int CAImage::getFontHeight(const char* pFontName, unsigned long nSize)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    
    return CAFreeTypeFont::getFontHeight(pFontName, nSize);

#else
    
    return CCImage::getFontHeight(pFontName, (float)nSize);
    
#endif
}

CAImagePixelFormat CAImage::getPixelFormat()
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
    //The texture data mustn't be saved because it isn't a mutable texture.
    return data;
}

bool CAImage::hasPremultipliedAlpha()
{
    return m_bHasPremultipliedAlpha;
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
    
    CAConfiguration *conf = CAConfiguration::sharedConfiguration();
    
    unsigned maxTextureSize = conf->getMaxTextureSize();
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

bool CAImage::initWithPVRFile(const char* file)
{
    bool bRet = false;
    // nothing to do with CAObject::init
    
    CAImagePVR *pvr = new CAImagePVR;
    bRet = pvr->initWithContentsOfFile(file);
        
    if (bRet)
    {
        pvr->setRetainName(true); // don't dealloc texture on release
        
        m_uName = pvr->getName();
        m_fMaxS = 1.0f;
        m_fMaxT = 1.0f;
        m_uPixelsWide = pvr->getWidth();
        m_uPixelsHigh = pvr->getHeight();
        m_tContentSize = CCSizeMake((float)m_uPixelsWide, (float)m_uPixelsHigh);
        m_bHasPremultipliedAlpha = PVRHaveAlphaPremultiplied_;
        m_ePixelFormat = pvr->getFormat();
        m_bHasMipmaps = pvr->getNumberOfMipmaps() > 1;       

        pvr->release();
    }
    else
    {
        CCLOG("CrossApp: Couldn't load PVR image %s", file);
    }

    return bRet;
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

void CAImage::PVRImagesHavePremultipliedAlpha(bool haveAlphaPremultiplied)
{
    PVRHaveAlphaPremultiplied_ = haveAlphaPremultiplied;
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

		case kCAImagePixelFormat_PVRTC4:
			return  "PVRTC4";

		case kCAImagePixelFormat_PVRTC2:
			return  "PVRTC2";

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
		case kCAImagePixelFormat_PVRTC4:
			ret = 4;
			break;
		case kCAImagePixelFormat_PVRTC2:
			ret = 2;
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

#define PVR_TEXTURE_FLAG_TYPE_MASK    0xff

static const ccPVRTexturePixelFormatInfo PVRTableFormats[] = {
	
	// 0: BGRA_8888
	{GL_RGBA, GL_BGRA, GL_UNSIGNED_BYTE, 32, false, true, kCAImagePixelFormat_RGBA8888},
	// 1: RGBA_8888
	{GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 32, false, true, kCAImagePixelFormat_RGBA8888},
	// 2: RGBA_4444
	{GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, 16, false, true, kCAImagePixelFormat_RGBA4444},
	// 3: RGBA_5551
	{GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, 16, false, true, kCAImagePixelFormat_RGB5A1},
	// 4: RGB_565
	{GL_RGB, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, 16, false, false, kCAImagePixelFormat_RGB565},
	// 5: RGB_888
	{GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, 24, false, false, kCAImagePixelFormat_RGB888},
	// 6: A_8
	{GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE, 8, false, false, kCAImagePixelFormat_A8},
	// 7: L_8
	{GL_LUMINANCE, GL_LUMINANCE, GL_UNSIGNED_BYTE, 8, false, false, kCAImagePixelFormat_I8},
	// 8: LA_88
	{GL_LUMINANCE_ALPHA, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, 16, false, true, kCAImagePixelFormat_AI88},
    
    // Not all platforms include GLES/gl2ext.h so these PVRTC enums are not always
    // available.
#ifdef GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG
	// 9: PVRTC 2BPP RGB
	{GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, 0xFFFFFFFF, 0xFFFFFFFF, 2, true, false, kCAImagePixelFormat_PVRTC2},
	// 10: PVRTC 2BPP RGBA
	{GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG, 0xFFFFFFFF, 0xFFFFFFFF, 2, true, true, kCAImagePixelFormat_PVRTC2},
	// 11: PVRTC 4BPP RGB
	{GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG, 0xFFFFFFFF, 0xFFFFFFFF, 4, true, false, kCAImagePixelFormat_PVRTC4},
	// 12: PVRTC 4BPP RGBA
	{GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, 0xFFFFFFFF, 0xFFFFFFFF, 4, true, true, kCAImagePixelFormat_PVRTC4},
#endif

};

struct _pixel_formathash {
	uint64_t pixelFormat;
	const ccPVRTexturePixelFormatInfo * pixelFormatInfo;
};

// Values taken from PVRTexture.h from http://www.imgtec.com
enum {
    kPVR2TextureFlagMipmap         = (1<<8),        // has mip map levels
    kPVR2TextureFlagTwiddle        = (1<<9),        // is twiddled
    kPVR2TextureFlagBumpmap        = (1<<10),       // has normals encoded for a bump map
    kPVR2TextureFlagTiling         = (1<<11),       // is bordered for tiled pvr
    kPVR2TextureFlagCubemap        = (1<<12),       // is a cubemap/skybox
    kPVR2TextureFlagFalseMipCol    = (1<<13),       // are there false colored MIP levels
    kPVR2TextureFlagVolume         = (1<<14),       // is this a volume texture
    kPVR2TextureFlagAlpha          = (1<<15),       // v2.1 is there transparency info in the texture
    kPVR2TextureFlagVerticalFlip   = (1<<16),       // v2.1 is the texture vertically flipped
};

enum {
	kPVR3TextureFlagPremultipliedAlpha	= (1<<1)	// has premultiplied alpha
};

static char gPVRTexIdentifier[5] = "PVR!";

// v2
typedef enum
{
	kPVR2TexturePixelFormat_RGBA_4444= 0x10,
	kPVR2TexturePixelFormat_RGBA_5551,
	kPVR2TexturePixelFormat_RGBA_8888,
	kPVR2TexturePixelFormat_RGB_565,
	kPVR2TexturePixelFormat_RGB_555,				// unsupported
	kPVR2TexturePixelFormat_RGB_888,
	kPVR2TexturePixelFormat_I_8,
	kPVR2TexturePixelFormat_AI_88,
	kPVR2TexturePixelFormat_PVRTC_2BPP_RGBA,
	kPVR2TexturePixelFormat_PVRTC_4BPP_RGBA,
	kPVR2TexturePixelFormat_BGRA_8888,
	kPVR2TexturePixelFormat_A_8,
} ccPVR2TexturePixelFormat;

// v3
/* supported predefined formats */
#define kPVR3TexturePixelFormat_PVRTC_2BPP_RGB   0
#define kPVR3TexturePixelFormat_PVRTC_2BPP_RGBA  1
#define kPVR3TexturePixelFormat_PVRTC_4BPP_RGB   2
#define kPVR3TexturePixelFormat_PVRTC_4BPP_RGBA  3

/* supported channel type formats */
#define kPVR3TexturePixelFormat_BGRA_8888  0x0808080861726762ULL
#define kPVR3TexturePixelFormat_RGBA_8888  0x0808080861626772ULL
#define kPVR3TexturePixelFormat_RGBA_4444  0x0404040461626772ULL
#define kPVR3TexturePixelFormat_RGBA_5551  0x0105050561626772ULL
#define kPVR3TexturePixelFormat_RGB_565    0x0005060500626772ULL
#define kPVR3TexturePixelFormat_RGB_888    0x0008080800626772ULL
#define kPVR3TexturePixelFormat_A_8        0x0000000800000061ULL
#define kPVR3TexturePixelFormat_L_8        0x000000080000006cULL
#define kPVR3TexturePixelFormat_LA_88      0x000008080000616cULL


// v2
static struct _pixel_formathash v2_pixel_formathash[] = {
    
	{ kPVR2TexturePixelFormat_BGRA_8888,	&PVRTableFormats[0] },
	{ kPVR2TexturePixelFormat_RGBA_8888,	&PVRTableFormats[1] },
	{ kPVR2TexturePixelFormat_RGBA_4444,	&PVRTableFormats[2] },
	{ kPVR2TexturePixelFormat_RGBA_5551,	&PVRTableFormats[3] },
	{ kPVR2TexturePixelFormat_RGB_565,		&PVRTableFormats[4] },
	{ kPVR2TexturePixelFormat_RGB_888,		&PVRTableFormats[5] },
	{ kPVR2TexturePixelFormat_A_8,			&PVRTableFormats[6] },
	{ kPVR2TexturePixelFormat_I_8,			&PVRTableFormats[7] },
	{ kPVR2TexturePixelFormat_AI_88,		&PVRTableFormats[8] },
    
#ifdef GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG
	{ kPVR2TexturePixelFormat_PVRTC_2BPP_RGBA,	&PVRTableFormats[10] },
	{ kPVR2TexturePixelFormat_PVRTC_4BPP_RGBA,	&PVRTableFormats[12] },
#endif
};

#define PVR2_MAX_TABLE_ELEMENTS (sizeof(v2_pixel_formathash) / sizeof(v2_pixel_formathash[0]))

// v3
struct _pixel_formathash v3_pixel_formathash[] = {
	
	{kPVR3TexturePixelFormat_BGRA_8888,	&PVRTableFormats[0] },
	{kPVR3TexturePixelFormat_RGBA_8888,	&PVRTableFormats[1] },
	{kPVR3TexturePixelFormat_RGBA_4444, &PVRTableFormats[2] },
	{kPVR3TexturePixelFormat_RGBA_5551, &PVRTableFormats[3] },
	{kPVR3TexturePixelFormat_RGB_565,	&PVRTableFormats[4] },
	{kPVR3TexturePixelFormat_RGB_888,	&PVRTableFormats[5] },
	{kPVR3TexturePixelFormat_A_8,		&PVRTableFormats[6] },
	{kPVR3TexturePixelFormat_L_8,		&PVRTableFormats[7] },
	{kPVR3TexturePixelFormat_LA_88,		&PVRTableFormats[8] },
	
#ifdef GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG
	{kPVR3TexturePixelFormat_PVRTC_2BPP_RGB,	&PVRTableFormats[9] },
	{kPVR3TexturePixelFormat_PVRTC_2BPP_RGBA,	&PVRTableFormats[10] },
	{kPVR3TexturePixelFormat_PVRTC_4BPP_RGB,	&PVRTableFormats[11] },
	{kPVR3TexturePixelFormat_PVRTC_4BPP_RGBA,	&PVRTableFormats[12] },
#endif
};


//Tells How large is tableFormats
#define PVR3_MAX_TABLE_ELEMENTS (sizeof(v3_pixel_formathash) / sizeof(v3_pixel_formathash[0]))


typedef struct _PVRTexHeader
{
    unsigned int headerLength;
    unsigned int height;
    unsigned int width;
    unsigned int numMipmaps;
    unsigned int flags;
    unsigned int dataLength;
    unsigned int bpp;
    unsigned int bitmaskRed;
    unsigned int bitmaskGreen;
    unsigned int bitmaskBlue;
    unsigned int bitmaskAlpha;
    unsigned int pvrTag;
    unsigned int numSurfs;
} ccPVRv2TexHeader;

#ifdef _MSC_VER
#pragma pack(push,1)
#endif
typedef struct {
	uint32_t version;
	uint32_t flags;
	uint64_t pixelFormat;
	uint32_t colorSpace;
	uint32_t channelType;
	uint32_t height;
	uint32_t width;
	uint32_t depth;
	uint32_t numberOfSurfaces;
	uint32_t numberOfFaces;
	uint32_t numberOfMipmaps;
	uint32_t metadataLength;
#ifdef _MSC_VER
} ccPVRv3TexHeader;
#pragma pack(pop)
#else
} __attribute__((packed)) ccPVRv3TexHeader;
#endif


CAImagePVR::CAImagePVR()
: m_uNumberOfMipmaps(0)
, m_uWidth(0)
, m_uHeight(0)
, m_uName(0)
, m_bHasAlpha(false)
, m_bHasPremultipliedAlpha(false)
, m_bForcePremultipliedAlpha(false)
, m_bRetainName(false)
, m_eFormat(kCAImagePixelFormat_Default)
, m_pPixelFormatInfo(NULL)
{
}

CAImagePVR::~CAImagePVR()
{
    CCLOGINFO( "CrossApp: deallocing CAImagePVR" );
    
    if (m_uName != 0 && ! m_bRetainName)
    {
        ccGLDeleteTexture(m_uName);
    }
}

bool CAImagePVR::unpackPVRv2Data(unsigned char* data, unsigned int len)
{
    bool success = false;
    ccPVRv2TexHeader *header = NULL;
    unsigned int flags, pvrTag;
    unsigned int dataLength = 0, dataOffset = 0, dataSize = 0;
    unsigned int blockSize = 0, widthBlocks = 0, heightBlocks = 0;
    unsigned int width = 0, height = 0, bpp = 4;
    unsigned char *bytes = NULL;
    unsigned int formatFlags;
    
    //Cast first sizeof(PVRTexHeader) bytes of data stream as PVRTexHeader
    header = (ccPVRv2TexHeader *)data;
    
    //Make sure that tag is in correct formatting
    pvrTag = CC_SWAP_INT32_LITTLE_TO_HOST(header->pvrTag);
    
    if (gPVRTexIdentifier[0] != (char)(((pvrTag >>  0) & 0xff)) ||
        gPVRTexIdentifier[1] != (char)(((pvrTag >>  8) & 0xff)) ||
        gPVRTexIdentifier[2] != (char)(((pvrTag >> 16) & 0xff)) ||
        gPVRTexIdentifier[3] != (char)(((pvrTag >> 24) & 0xff)))
    {
        return false;
    }
    
    CAConfiguration *configuration = CAConfiguration::sharedConfiguration();
    
    flags = CC_SWAP_INT32_LITTLE_TO_HOST(header->flags);
    formatFlags = flags & PVR_TEXTURE_FLAG_TYPE_MASK;
    bool flipped = (flags & kPVR2TextureFlagVerticalFlip) ? true : false;
    if (flipped)
    {
        CCLOG("CrossApp: WARNING: Image is flipped. Regenerate it using PVRTexTool");
    }
    
    if (! configuration->supportsNPOT() &&
        (header->width != ccNextPOT(header->width) || header->height != ccNextPOT(header->height)))
    {
        CCLOG("CrossApp: ERROR: Loading an NPOT texture (%dx%d) but is not supported on this device", header->width, header->height);
        return false;
    }
    
    unsigned int pvr2TableElements = PVR2_MAX_TABLE_ELEMENTS;
    if (! CAConfiguration::sharedConfiguration()->supportsPVRTC())
    {
        pvr2TableElements = 9;
    }
    
    for (unsigned int i = 0; i < pvr2TableElements; i++)
    {
        //Does image format in table fits to the one parsed from header?
        if (v2_pixel_formathash[i].pixelFormat == formatFlags)
        {
            m_pPixelFormatInfo = v2_pixel_formathash[i].pixelFormatInfo;
            
            //Reset num of mipmaps
            m_uNumberOfMipmaps = 0;
            
            //Get size of mipmap
            m_uWidth = width = CC_SWAP_INT32_LITTLE_TO_HOST(header->width);
            m_uHeight = height = CC_SWAP_INT32_LITTLE_TO_HOST(header->height);
            
            //Do we use alpha ?
            if (CC_SWAP_INT32_LITTLE_TO_HOST(header->bitmaskAlpha))
            {
                m_bHasAlpha = true;
            }
            else
            {
                m_bHasAlpha = false;
            }
            
            //Get ptr to where data starts..
            dataLength = CC_SWAP_INT32_LITTLE_TO_HOST(header->dataLength);
            
            //Move by size of header
            bytes = ((unsigned char *)data) + sizeof(ccPVRv2TexHeader);
            m_eFormat = m_pPixelFormatInfo->ccPixelFormat;
            bpp = m_pPixelFormatInfo->bpp;
            
            // Calculate the data size for each texture level and respect the minimum number of blocks
            while (dataOffset < dataLength)
            {
                switch (formatFlags) {
                    case kPVR2TexturePixelFormat_PVRTC_2BPP_RGBA:
                        blockSize = 8 * 4; // Pixel by pixel block size for 2bpp
                        widthBlocks = width / 8;
                        heightBlocks = height / 4;
                        break;
                    case kPVR2TexturePixelFormat_PVRTC_4BPP_RGBA:
                        blockSize = 4 * 4; // Pixel by pixel block size for 4bpp
                        widthBlocks = width / 4;
                        heightBlocks = height / 4;
                        break;
                    case kPVR2TexturePixelFormat_BGRA_8888:
                        if (CAConfiguration::sharedConfiguration()->supportsBGRA8888() == false)
                        {
                            CCLOG("CrossApp: TexturePVR. BGRA8888 not supported on this device");
                            return false;
                        }
                    default:
                        blockSize = 1;
                        widthBlocks = width;
                        heightBlocks = height;
                        break;
                }
                
                // Clamp to minimum number of blocks
                if (widthBlocks < 2)
                {
                    widthBlocks = 2;
                }
                if (heightBlocks < 2)
                {
                    heightBlocks = 2;
                }
                
                dataSize = widthBlocks * heightBlocks * ((blockSize  * bpp) / 8);
                unsigned int packetLength = (dataLength - dataOffset);
                packetLength = packetLength > dataSize ? dataSize : packetLength;
                
                //Make record to the mipmaps array and increment counter
                m_asMipmaps[m_uNumberOfMipmaps].address = bytes + dataOffset;
                m_asMipmaps[m_uNumberOfMipmaps].len = packetLength;
                m_uNumberOfMipmaps++;
                
                //Check that we didn't overflow
                CCAssert(m_uNumberOfMipmaps < CC_PVRMIPMAP_MAX,
                         "TexturePVR: Maximum number of mipmaps reached. Increase the CC_PVRMIPMAP_MAX value");
                
                dataOffset += packetLength;
                
                //Update width and height to the next lower power of two
                width = MAX(width >> 1, 1);
                height = MAX(height >> 1, 1);
            }
            
            //Mark pass as success
            success = true;
            break;
        }
    }
    
    if (! success)
    {
        CCLOG("CrossApp: WARNING: Unsupported PVR Pixel Format: 0x%2x. Re-encode it with a OpenGL pixel format variant", formatFlags);
    }
    
    return success;
}

bool CAImagePVR::unpackPVRv3Data(unsigned char* dataPointer, unsigned int dataLength)
{
    if (dataLength < sizeof(ccPVRv3TexHeader))
    {
		return false;
	}
	
	ccPVRv3TexHeader *header = (ccPVRv3TexHeader *)dataPointer;
	
	// validate version
	if (CC_SWAP_INT32_BIG_TO_HOST(header->version) != 0x50565203)
    {
		CCLOG("CrossApp: WARNING: pvr file version mismatch");
		return false;
	}
	
	// parse pixel format
	uint64_t pixelFormat = header->pixelFormat;
    
	
	bool infoValid = false;
    
    unsigned int pvr3TableElements = PVR3_MAX_TABLE_ELEMENTS;
    if (! CAConfiguration::sharedConfiguration()->supportsPVRTC())
    {
        pvr3TableElements = 9;
    }
	
	for(unsigned int i = 0; i < pvr3TableElements; i++)
    {
		if( v3_pixel_formathash[i].pixelFormat == pixelFormat )
        {
			m_pPixelFormatInfo = v3_pixel_formathash[i].pixelFormatInfo;
			m_bHasAlpha = m_pPixelFormatInfo->alpha;
			infoValid = true;
			break;
		}
	}
	
	// unsupported / bad pixel format
	if (! infoValid)
    {
		CCLOG("CrossApp: WARNING: unsupported pvr pixelformat: %lx", (unsigned long)pixelFormat );
		return false;
	}
    
    // flags
	uint32_t flags = CC_SWAP_INT32_LITTLE_TO_HOST(header->flags);
	
	// PVRv3 specifies premultiply alpha in a flag -- should always respect this in PVRv3 files
	m_bForcePremultipliedAlpha = true;
	if (flags & kPVR3TextureFlagPremultipliedAlpha)
    {
		m_bHasPremultipliedAlpha = true;
	}
    
	// sizing
	uint32_t width = CC_SWAP_INT32_LITTLE_TO_HOST(header->width);
	uint32_t height = CC_SWAP_INT32_LITTLE_TO_HOST(header->height);
	m_uWidth = width;
	m_uHeight = height;
	uint32_t dataOffset = 0, dataSize = 0;
	uint32_t blockSize = 0, widthBlocks = 0, heightBlocks = 0;
	uint8_t *bytes = NULL;
	
	dataOffset = (sizeof(ccPVRv3TexHeader) + header->metadataLength);
	bytes = dataPointer;
	
	m_uNumberOfMipmaps = header->numberOfMipmaps;
	CCAssert(m_uNumberOfMipmaps < CC_PVRMIPMAP_MAX, "TexturePVR: Maximum number of mimpaps reached. Increate the CC_PVRMIPMAP_MAX value");
    
	for (unsigned int i = 0; i < m_uNumberOfMipmaps; i++)
    {
		switch (pixelFormat)
        {
			case kPVR3TexturePixelFormat_PVRTC_2BPP_RGB :
			case kPVR3TexturePixelFormat_PVRTC_2BPP_RGBA :
				blockSize = 8 * 4; // Pixel by pixel block size for 2bpp
				widthBlocks = width / 8;
				heightBlocks = height / 4;
				break;
			case kPVR3TexturePixelFormat_PVRTC_4BPP_RGB :
			case kPVR3TexturePixelFormat_PVRTC_4BPP_RGBA :
				blockSize = 4 * 4; // Pixel by pixel block size for 4bpp
				widthBlocks = width / 4;
				heightBlocks = height / 4;
				break;
			case kPVR3TexturePixelFormat_BGRA_8888:
				if( ! CAConfiguration::sharedConfiguration()->supportsBGRA8888())
                {
					CCLOG("CrossApp: TexturePVR. BGRA8888 not supported on this device");
					return false;
				}
			default:
				blockSize = 1;
				widthBlocks = width;
				heightBlocks = height;
				break;
		}
        
		// Clamp to minimum number of blocks
		if (widthBlocks < 2)
        {
			widthBlocks = 2;
        }
		if (heightBlocks < 2)
        {
			heightBlocks = 2;
        }
		
		dataSize = widthBlocks * heightBlocks * ((blockSize  * m_pPixelFormatInfo->bpp) / 8);
		unsigned int packetLength = ((unsigned int)dataLength-dataOffset);
		packetLength = packetLength > dataSize ? dataSize : packetLength;
		
		m_asMipmaps[i].address = bytes+dataOffset;
		m_asMipmaps[i].len = packetLength;
		
		dataOffset += packetLength;
		CCAssert(dataOffset <= dataLength, "CCTexurePVR: Invalid lenght");
		
		
		width = MAX(width >> 1, 1);
		height = MAX(height >> 1, 1);
	}
	
	return true;
}

bool CAImagePVR::createGLTexture()
{
    unsigned int width = m_uWidth;
    unsigned int height = m_uHeight;
    GLenum err;
    
    if (m_uNumberOfMipmaps > 0)
    {
        if (m_uName != 0)
        {
            ccGLDeleteTexture(m_uName);
        }
        
        // From PVR sources: "PVR files are never row aligned."
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        
        glGenTextures(1, &m_uName);
        glBindTexture(GL_TEXTURE_2D, m_uName);
        
        // Default: Anti alias.
		if (m_uNumberOfMipmaps == 1)
        {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
		else
        {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        }
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    
    CHECK_GL_ERROR_DEBUG(); // clean possible GL error
    
	GLenum internalFormat = m_pPixelFormatInfo->internalFormat;
	GLenum format = m_pPixelFormatInfo->format;
	GLenum type = m_pPixelFormatInfo->type;
    bool compressed = m_pPixelFormatInfo->compressed;
    
    // Generate textures with mipmaps
    for (unsigned int i = 0; i < m_uNumberOfMipmaps; ++i)
    {
        if (compressed && ! CAConfiguration::sharedConfiguration()->supportsPVRTC())
        {
			CCLOG("CrossApp: WARNING: PVRTC images are not supported");
			return false;
		}
        
		unsigned char *data = m_asMipmaps[i].address;
		GLsizei datalen = m_asMipmaps[i].len;
        
		if (compressed)
        {
			glCompressedTexImage2D(GL_TEXTURE_2D, i, internalFormat, width, height, 0, datalen, data);
        }
		else
        {
			glTexImage2D(GL_TEXTURE_2D, i, internalFormat, width, height, 0, format, type, data);
        }
        
		if (i > 0 && (width != height || ccNextPOT(width) != width ))
        {
			CCLOG("CrossApp: TexturePVR. WARNING. Mipmap level %u is not squared. Texture won't render correctly. width=%u != height=%u", i, width, height);
        }
        
		err = glGetError();
		if (err != GL_NO_ERROR)
		{
			CCLOG("CrossApp: TexturePVR: Error uploading compressed texture level: %u . glError: 0x%04X", i, err);
			return false;
		}
        
		width = MAX(width >> 1, 1);
		height = MAX(height >> 1, 1);
    }
    
    return true;
}


bool CAImagePVR::initWithContentsOfFile(const char* path)
{
    unsigned char* pvrdata = NULL;
    int pvrlen = 0;
    
    std::string lowerCase(path);
    for (unsigned int i = 0; i < lowerCase.length(); ++i)
    {
        lowerCase[i] = tolower(lowerCase[i]);
    }
    
    if (lowerCase.find(".ccz") != std::string::npos)
    {
        pvrlen = ZipUtils::ccInflateCCZFile(path, &pvrdata);
    }
    else if (lowerCase.find(".gz") != std::string::npos)
    {
        pvrlen = ZipUtils::ccInflateGZipFile(path, &pvrdata);
    }
    else
    {
        pvrdata = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", (unsigned long *)(&pvrlen));
    }
    
    if (pvrlen < 0)
    {
        this->release();
        return false;
    }
    
    m_uNumberOfMipmaps = 0;
    
    m_uName = 0;
    m_uWidth = m_uHeight = 0;
    m_pPixelFormatInfo = NULL;
    m_bHasAlpha = false;
    m_bForcePremultipliedAlpha = false;
    m_bHasPremultipliedAlpha = false;
    
    m_bRetainName = false; // CrossApp integration
    
    if (! ((unpackPVRv2Data(pvrdata, pvrlen)  || unpackPVRv3Data(pvrdata, pvrlen)) && createGLTexture()) )
    {
        CC_SAFE_DELETE_ARRAY(pvrdata);
        this->release();
        return false;
    }
    
    CC_SAFE_DELETE_ARRAY(pvrdata);
    
    return true;
}

CAImagePVR * CAImagePVR::create(const char* path)
{
    CAImagePVR * pTexture = new CAImagePVR();
    if (pTexture)
    {
        if (pTexture->initWithContentsOfFile(path))
        {
            pTexture->autorelease();
        }
        else
        {
            delete pTexture;
            pTexture = NULL;
        }
    }
    
    return pTexture;
}


NS_CC_END
