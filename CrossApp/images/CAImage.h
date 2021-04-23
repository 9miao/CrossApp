//
//  CAImage.h
//  CrossApp
//
//  Created by 栗元峰 on 15-3-23.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CAIMAGE_H__
#define __CAIMAGE_H__

#include "basics/CAObject.h"
#include "basics/CAPoint.h"
#include "basics/CASize.h"
#include "basics/CARect.h"
#include "basics/CAData.h"
#include "ccTypes.h"
#include "platform/CCPlatformMacros.h"
#include "controller/CAFont.h"
#include "CCGL.h"
#include "CCStdC.h"
#include <list>
#include <map>
#include <set>

NS_CC_BEGIN

class GLProgram;
class CAColor4B;
class CC_DLL CAImage : public CAObject
{
public:
    
    enum class PixelFormat
    {
        RGBA8888 = 0,
        RGB888,
        RGB565,
        A8,
        I8,
        AI88,
        RGBA4444,
        RGB5A1,
        DEFAULT
    };
    
    /** Supported formats for Image */
    enum class Format
    {
        JPG,//! JPEG
        PNG,//! PNG
        TIFF,//! TIFF
        WEBP,//! WebP
        ETC,//! ETC
        TGA,//! TGA
        RAW_DATA,//! Raw Data
        UNKOWN//! Unknown format
    };
    
    struct PixelFormatInfo
    {
        
        PixelFormatInfo(GLenum anInternalFormat, GLenum aFormat, GLenum aType, int aBpp, bool aCompressed, bool anAlpha)
        : internalFormat(anInternalFormat)
        , format(aFormat)
        , type(aType)
        , bpp(aBpp)
        , compressed(aCompressed)
        , alpha(anAlpha)
        {}
        
        GLenum internalFormat;
        GLenum format;
        GLenum type;
        int bpp;
        bool compressed;
        bool alpha;
    };
    
    typedef std::map<CAImage::PixelFormat, const PixelFormatInfo> PixelFormatInfoMap;
    
    /**
     Extension to set the Min / Mag filter
     */
    typedef struct _TexParams {
        GLuint    minFilter;
        GLuint    magFilter;
        GLuint    wrapS;
        GLuint    wrapT;
    }TexParams;
    
    CAImage();
    
    virtual ~CAImage();
    
    static int getFontHeight(const char* pFontName, float nSize);
    
    static int getStringWidth(const char* pFontName,
                              float nSize,
                              const std::string& pText);

    static int getStringHeight(const char* pFontName,
                               float nSize,
                               const std::string& pText,
                               float iLimitWidth,
                               int iLineSpace = 0,
                               bool bWordWrap = true);
    
    static CAImage* createWithString(const std::string& text, const CAFont& font, DSize& dimensions, CATextAlignment textAlignment);
    
    static CAImage* scaleToNewImageWithImage(CAImage* image, const DSize& size);
    
    static CAImage* scaleToNewImageWithImage(CAImage* image, float scaleX, float scaleY);
    
    static CAImage* generateMipmapsWithImage(CAImage* image);
    
    static CAImage* create(const std::string& file);

    static CAImage* createWithImageDataNoCache(CAData* data);
    
    static CAImage* createWithImageData(CAData* data, const std::string& key);
    
    static CAImage* createWithRawDataNoCache(CAData* data,
                                             const CAImage::PixelFormat& pixelFormat,
                                             unsigned int pixelsWide,
                                             unsigned int pixelsHigh,
                                             bool hasPremultipliedAlpha = false);
    
    static CAImage* createWithRawData(CAData* data,
                                      const CAImage::PixelFormat& pixelFormat,
                                      unsigned int pixelsWide,
                                      unsigned int pixelsHigh,
                                      const std::string& key,
                                      bool hasPremultipliedAlpha = false);
    
    static CAImage* createWithColor4B(const CAColor4B& color);
    
    bool initWithImageFile(const std::string& file, bool isOpenGLThread = true);
    
    bool initWithImageData(CAData* data, bool isOpenGLThread = true);
    
    bool initWithRawData(CAData* data, const CAImage::PixelFormat& pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, bool hasPremultipliedAlpha = false);
    
    bool initWithColor4B(const CAColor4B& color);

    const char* description(void);
    
    virtual void drawAtPoint(const DPoint& point);
    
    virtual void drawInRect(const DRect& rect);
    
    virtual void setTexParameters(CAImage::TexParams* texParams);
    
    virtual void setAntiAliasTexParameters();
    
    virtual void setAliasTexParameters();
    
    virtual void generateMipmap();
    
    const char* stringForFormat();
    
    unsigned int bitsPerPixelForFormat();
    
    unsigned int bitsPerPixelForFormat(CAImage::PixelFormat format);

    bool hasPremultipliedAlpha();
    
    bool hasMipmaps();
    
    int  getBitPerPixel();
    
    bool hasAlpha();
    
    bool isCompressed();
    
    bool saveToFile(const std::string& fullPath, bool bIsToRGB = false);
    
    const char* getImageFileType();
    
    float getAspectRatio();
    
    static CAImage* WHITE_IMAGE();
    
    static CAImage* BLACK_IMAGE();

    static CAImage* CLEAR_IMAGE();
    
    static CAImage* CC_SHADOW_IMAGE();
    
    virtual CAImage* copy();
    
    CAImage::Format detectFormat(const unsigned char * data, unsigned long dataLen);
    bool isPng(const unsigned char * data, unsigned long dataLen);
    bool isJpg(const unsigned char * data, unsigned long dataLen);
    bool isTiff(const unsigned char * data, unsigned long dataLen);
    bool isWebp(const unsigned char * data, unsigned long dataLen);
    bool isPvr(const unsigned char * data, unsigned long dataLen);
    bool isEtc(const unsigned char * data, unsigned long dataLen);
    
    CC_PROPERTY_READONLY_PASS_BY_REF(CAImage::PixelFormat, m_ePixelFormat, PixelFormat)
    
    CC_PROPERTY_READONLY(unsigned int, m_uPixelsWide, PixelsWide)
    
    CC_PROPERTY_READONLY(unsigned int, m_uPixelsHigh, PixelsHigh)
    
    CC_PROPERTY_READONLY(GLuint, m_uName, Name)
    
    CC_PROPERTY(GLfloat, m_fMaxS, MaxS)
    
    CC_PROPERTY(GLfloat, m_fMaxT, MaxT)
    
    CC_PROPERTY_READONLY_PASS_BY_REF(DSize, m_tContentSize, ContentSize)
    
    CC_PROPERTY(GLProgram*, m_pShaderProgram, ShaderProgram);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bMonochrome, Monochrome);
    
    CC_SYNTHESIZE_READONLY(CAData*, m_pData, Data);

    void premultipliedImageData();
    
    void repremultipliedImageData();
    
    void freeName();
    
    static void reloadAllImages();
    
    static const PixelFormatInfoMap& getPixelFormatInfoMap();
    
    CAImage::PixelFormat convertDataToFormat(const unsigned char* data,
                                             unsigned long dataLen,
                                             CAImage::PixelFormat originFormat,
                                             CAImage::PixelFormat format,
                                             unsigned char** outData,
                                             unsigned long* outDataLen);
    
    CAImage::PixelFormat convertI8ToFormat(const unsigned char* data,
                                           unsigned long dataLen,
                                           CAImage::PixelFormat format,
                                           unsigned char** outData,
                                           unsigned long* outDataLen);
    CAImage::PixelFormat convertAI88ToFormat(const unsigned char* data,
                                             unsigned long dataLen,
                                             CAImage::PixelFormat format,
                                             unsigned char** outData,
                                             unsigned long* outDataLen);
    CAImage::PixelFormat convertRGB888ToFormat(const unsigned char* data,
                                               unsigned long dataLen,
                                               CAImage::PixelFormat format,
                                               unsigned char** outData,
                                               unsigned long* outDataLen);
    CAImage::PixelFormat convertRGBA8888ToFormat(const unsigned char* data,
                                                 unsigned long dataLen,
                                                 CAImage::PixelFormat format,
                                                 unsigned char** outData,
                                                 unsigned long* outDataLen);
    
protected:
    
    bool initWithJpgData(const unsigned char *  data, unsigned long dataLen);
    bool initWithPngData(const unsigned char * data, unsigned long dataLen);
    bool initWithTiffData(const unsigned char * data, unsigned long dataLen);
    bool initWithWebpData(const unsigned char * data, unsigned long dataLen);
    typedef struct sImageTGA tImageTGA;
    bool initWithTGAData(tImageTGA* tgaData);
    
    bool saveImageToPNG(const std::string& filePath, bool isToRGB);
    bool saveImageToJPG(const std::string& filePath);

    void convertToRawData();

    void convertRGBA8888ToRGB5A1(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertRGB888ToRGB5A1(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertRGBA8888ToRGBA4444(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertRGB888ToRGBA4444(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertRGBA8888ToAI88(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertRGB888ToAI88(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertRGBA8888ToA8(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertRGBA8888ToI8(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertRGB888ToI8(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertRGBA8888ToRGB565(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertRGB888ToRGB565(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertRGBA8888ToRGB888(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertRGB888ToRGBA8888(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertAI88ToI8(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertAI88ToA8(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertI8ToAI88(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertAI88ToRGB5A1(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertI8ToRGB5A1(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertAI88ToRGBA4444(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertI8ToRGBA4444(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertAI88ToRGB565(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertI8ToRGB565(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertAI88ToRGBA8888(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertI8ToRGBA8888(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertAI88ToRGB888(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    void convertI8ToRGB888(const unsigned char* data, unsigned long dataLen, unsigned char* outData);
    
public:
    
    std::wstring m_txt;
    
    std::string m_FileName;
    
    static const std::set<CAImage*>& getImagesSet();
    
    static void purgeCAImage();
    
protected:
    
    bool m_bPremultiplied;
    
    bool m_bHasPremultipliedAlpha;
    
    bool m_bHasMipmaps;
    
    bool m_bTextImage;
    
    int  m_nBitsPerComponent;
                
    static const PixelFormatInfoMap s_pixelFormatInfoTables;
    
	friend class CAFTRichFont;
    
    friend class CAFreeTypeFont;
};

NS_CC_END

#endif //__CAImage_H__

