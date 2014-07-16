

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
    kCAImagePixelFormat_PVRTC4,
    kCAImagePixelFormat_PVRTC2,
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

	static CAImage* create(const char* file);
    
    static CAImage* createWithString(const char *text,  const char *fontName, float fontSize, const CCSize& dimensions, CATextAlignment hAlignment, CAVerticalTextAlignment vAlignment);
    
    static int getFontHeight(const char* pFontName, unsigned long nSize);
    
    const char* description(void);

    void releaseData(void *data);

    void* keepData(void *data, unsigned int length);

    bool initWithData(const void* data, CAImagePixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const CCSize& contentSize);

    void drawAtPoint(const CCPoint& point);

    void drawInRect(const CCRect& rect);

    bool initWithImage(CCImage * uiImage);

    bool initWithPVRFile(const char* file);

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

    static void PVRImagesHavePremultipliedAlpha(bool haveAlphaPremultiplied);

    const CCSize& getContentSizeInPixels();
    
    bool hasPremultipliedAlpha();
    
    bool hasMipmaps();
    
    static CAImage* CC_WHITE_IMAGE();
    
protected:
    
    CC_PROPERTY_READONLY(CAImagePixelFormat, m_ePixelFormat, PixelFormat)
    
    CC_PROPERTY_READONLY(unsigned int, m_uPixelsWide, PixelsWide)
    
    CC_PROPERTY_READONLY(unsigned int, m_uPixelsHigh, PixelsHigh)
    
    CC_PROPERTY_READONLY(GLuint, m_uName, Name)
    
    CC_PROPERTY(GLfloat, m_fMaxS, MaxS)
    
    CC_PROPERTY(GLfloat, m_fMaxT, MaxT)
    
    CC_PROPERTY_READONLY(CCSize, m_tContentSize, ContentSize)
    
    CC_PROPERTY(CAGLProgram*, m_pShaderProgram, ShaderProgram);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bMonochrome, Monochrome);
    
private:
    
    bool initPremultipliedATextureWithImage(CCImage * image, unsigned int pixelsWide, unsigned int pixelsHigh);

protected:
    
    bool m_bPVRHaveAlphaPremultiplied;
    
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

struct CAPVRMipmap {
    unsigned char *address;
    unsigned int len;
};

typedef struct _ccPVRTexturePixelFormatInfo {
	GLenum internalFormat;
	GLenum format;
	GLenum type;
	uint32_t bpp;
	bool compressed;
	bool alpha;
	CAImagePixelFormat ccPixelFormat;
} ccPVRTexturePixelFormatInfo;


enum {
    CC_PVRMIPMAP_MAX = 16,
};

class CAImagePVR : public CAObject
{
public:
    
    CAImagePVR();
    
    virtual ~CAImagePVR();

    bool initWithContentsOfFile(const char* path);

    static CAImagePVR* create(const char* path);

    inline bool hasAlpha() { return m_bHasAlpha; }

    inline bool hasPremultipliedAlpha() { return m_bHasPremultipliedAlpha; }

protected:
    
    CC_SYNTHESIZE_READONLY(GLuint, m_uName, Name);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_uWidth, Width);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_uHeight, Height);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bForcePremultipliedAlpha, ForcePremultipliedAlpha);

    CC_SYNTHESIZE_READONLY(unsigned int, m_uNumberOfMipmaps, NumberOfMipmaps);
    
    CC_SYNTHESIZE_READONLY(CAImagePixelFormat, m_eFormat, Format);
    
    CC_SYNTHESIZE_IS(bool, m_bRetainName, RetainName);
    
private:
    
    bool unpackPVRv2Data(unsigned char* data, unsigned int len);
    
    bool unpackPVRv3Data(unsigned char* dataPointer, unsigned int dataLength);
    
    bool createGLTexture();
    
protected:
    
    struct CAPVRMipmap m_asMipmaps[CC_PVRMIPMAP_MAX];

    bool m_bHasAlpha;
    
    bool m_bHasPremultipliedAlpha;
    
    const ccPVRTexturePixelFormatInfo *m_pPixelFormatInfo;
};

NS_CC_END

#endif //__CAImage_H__

