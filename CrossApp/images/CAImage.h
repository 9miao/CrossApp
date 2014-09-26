

#ifndef __CAIMAGE_H__
#define __CAIMAGE_H__

#include <string>
#include "basics/CAObject.h"
#include "basics/CAGeometry.h"
#include "ccTypes.h"
#include "platform/CCPlatformMacros.h"
#include "CCGL.h"
#include "CCStdC.h"
#include "cocoa/CCArray.h"
#ifdef EMSCRIPTEN
#include "base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN

NS_CC_BEGIN

class CCImage;

typedef enum
{
    kCAImagePixelFormat_RGBA8888,
    kCAImagePixelFormat_RGB888,
    kCAImagePixelFormat_RGB565,
    kCAImagePixelFormat_A8,
    kCAImagePixelFormat_I8,
    kCAImagePixelFormat_AI88,
    kCAImagePixelFormat_RGBA4444,
    kCAImagePixelFormat_RGB5A1,
    kCAImagePixelFormat_Default = kCAImagePixelFormat_RGBA8888,
    kImagePixelFormat_RGBA8888 = kCAImagePixelFormat_RGBA8888,
    kImagePixelFormat_RGB888 = kCAImagePixelFormat_RGB888,
    kImagePixelFormat_RGB565 = kCAImagePixelFormat_RGB565,
    kImagePixelFormat_A8 = kCAImagePixelFormat_A8,
    kImagePixelFormat_RGBA4444 = kCAImagePixelFormat_RGBA4444,
    kImagePixelFormat_RGB5A1 = kCAImagePixelFormat_RGB5A1,
    kImagePixelFormat_Default = kCAImagePixelFormat_Default

} CAImagePixelFormat;

class CAGLProgram;

typedef struct _ccTexParams {
    GLuint    minFilter;
    GLuint    magFilter;
    GLuint    wrapS;
    GLuint    wrapT;
} ccTexParams;

class CC_DLL CAImage : public CAObject
#ifdef EMSCRIPTEN
, public CCGLBufferedNode
#endif // EMSCRIPTEN
{
public:

    CAImage();

    virtual ~CAImage();

	static CAImage* createWithString(const char *text, const char *fontName, float fontSize, const CCSize& dimensions, CATextAlignment hAlignment, CAVerticalTextAlignment vAlignment, bool isForTextField = false, int iLineSpacing = 0);
    
    static int getFontHeight(const char* pFontName, unsigned long nSize);
    
    static int getStringWidth(const char* pFontName, unsigned long nSize, const std::string& pText);
    
	static int getStringHeight(const char* pFontName, unsigned long nSize, const std::string& pText, int iLimitWidth, int iLineSpace = 0, bool bWordWrap = true);
    
	static CAImage* create(const std::string& file);
    
    static CAImage* createWithDataNoCache(void* data, int lenght);
    
    static CAImage* createWithData(void* data, int lenght, const std::string& key);
    
    const char* description(void);

    void releaseData(void *data);

    void* keepData(void *data, unsigned int length);

    bool initWithData(const void* data, CAImagePixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const CCSize& contentSize);

    bool initWithData(void* data, int lenght);
    
    void drawAtPoint(const CCPoint& point);

    void drawInRect(const CCRect& rect);

    bool initWithImage(CCImage * uiImage);

    bool initWithETCFile(const char* file);

    void setTexParameters(ccTexParams* texParams);

    void setAntiAliasTexParameters();

    void setAliasTexParameters();

    void generateMipmap();

    const char* stringForFormat();

    unsigned int bitsPerPixelForFormat();  

    unsigned int bitsPerPixelForFormat(CAImagePixelFormat format);

    static void setDefaultAlphaPixelFormat(CAImagePixelFormat format);

    static CAImagePixelFormat defaultAlphaPixelFormat();

    const CCSize& getContentSizeInPixels();
    
    bool hasPremultipliedAlpha();
    
    bool hasMipmaps();
    
    bool saveToFile(const std::string& fullPath);
    
    const char* getImageFileType();
    
    static CAImage* CC_WHITE_IMAGE();
    
    float getAspectRatio();
    
protected:
    
    CC_PROPERTY_READONLY_PASS_BY_REF(CAImagePixelFormat, m_ePixelFormat, PixelFormat)
    
    CC_PROPERTY_READONLY(unsigned int, m_uPixelsWide, PixelsWide)
    
    CC_PROPERTY_READONLY(unsigned int, m_uPixelsHigh, PixelsHigh)
    
    CC_PROPERTY_READONLY(GLuint, m_uName, Name)
    
    CC_PROPERTY(GLfloat, m_fMaxS, MaxS)
    
    CC_PROPERTY(GLfloat, m_fMaxT, MaxT)
    
    CC_PROPERTY_READONLY(CCSize, m_tContentSize, ContentSize)
    
    CC_PROPERTY(CAGLProgram*, m_pShaderProgram, ShaderProgram);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bMonochrome, Monochrome);
    
    CC_SYNTHESIZE_READONLY(unsigned char*, m_pData, Data);
    
    CC_SYNTHESIZE_READONLY(unsigned long, m_nDataLenght, DataLenght);
    
private:
    
    bool initPremultipliedATextureWithImage(CCImage * image, unsigned int pixelsWide, unsigned int pixelsHigh);

protected:
    
    bool m_bHasPremultipliedAlpha;
    
    bool m_bHasMipmaps;
};


class CC_DLL CAImageETC : public CAObject
{
public:
    CAImageETC();
    virtual ~CAImageETC();
    
    bool initWithFile(const char* file);
    
    unsigned int getName() const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    
private:
    bool loadTexture(const char* file);
    
private:
    GLuint _name;
    unsigned int _width;
    unsigned int _height;
};

NS_CC_END

#endif //__CAImage_H__

