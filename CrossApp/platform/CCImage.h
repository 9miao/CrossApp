

#ifndef __CC_IMAGE_H__
#define __CC_IMAGE_H__

#include "basics/CAObject.h"

NS_CC_BEGIN

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
class CCFreeTypeFont;
#endif

/**
 * @addtogroup platform
 * @{
 */

class CC_DLL CCImage : public CAObject
{
public:
    /**
     @js ctor
     */
    CCImage();
    /**
     * @js NA
     * @lua NA
     */
    ~CCImage();

    typedef enum
    {
        kFmtJpg = 0,
        kFmtPng,
        kFmtTiff,
        kFmtWebp,
        kFmtRawData,
        kFmtUnKnown
    }EImageFormat;

    typedef enum
    {
        kAlignCenter        = 0x33, ///< Horizontal center and vertical center.
        kAlignTop           = 0x13, ///< Horizontal center and vertical top.
        kAlignTopRight      = 0x12, ///< Horizontal right and vertical top.
        kAlignRight         = 0x32, ///< Horizontal right and vertical center.
        kAlignBottomRight   = 0x22, ///< Horizontal right and vertical bottom.
        kAlignBottom        = 0x23, ///< Horizontal center and vertical bottom.
        kAlignBottomLeft    = 0x21, ///< Horizontal left and vertical bottom.
        kAlignLeft          = 0x31, ///< Horizontal left and vertical center.
        kAlignTopLeft       = 0x11, ///< Horizontal left and vertical top.
    }ETextAlign;
    
    /**
    @brief  Load the image from the specified path. 
    @param strPath   the absolute file path.
    @param imageType the type of image, currently only supporting two types.
    @return  true if loaded correctly.
    */
    bool initWithImageFile(const char * strPath, EImageFormat imageType = kFmtPng);

    /*
     @brief The same result as with initWithImageFile, but thread safe. It is caused by
            loadImage() in CAImageCache.cpp.
     @param fullpath  full path of the file.
     @param imageType the type of image, currently only supporting two types.
     @return  true if loaded correctly.
     */
    bool initWithImageFileThreadSafe(const char *fullpath, EImageFormat imageType = kFmtPng);

    /**
    @brief  Load image from stream buffer.

    @warning kFmtRawData only supports RGBA8888.
    @param pBuffer  stream buffer which holds the image data.
    @param nLength  data length expressed in (number of) bytes.
    @param nWidth, nHeight, nBitsPerComponent are used for kFmtRawData.
    @return true if loaded correctly.
    @js NA
    */
    bool initWithImageData(void * pData, 
                           int nDataLen, 
                           EImageFormat eFmt = kFmtUnKnown,
                           int nWidth = 0,
                           int nHeight = 0,
                           int nBitsPerComponent = 8);

    /**
    @brief    Create image with specified string.
    @param  pText       the text the image will show (cannot be nil).
    @param  nWidth      the image width, if 0, the width will match the text's width.
    @param  nHeight     the image height, if 0, the height will match the text's height.
    @param  eAlignMask  the test Alignment
    @param  pFontName   the name of the font used to draw the text. If nil, use the default system font.
    @param  nSize       the font size, if 0, use the system default size.
    @js NA
    */
    bool initWithString(
        const char *    pText, 
        int             nWidth = 0, 
        int             nHeight = 0,
        ETextAlign      eAlignMask = kAlignCenter,
        const char *    pFontName = 0,
        int             nSize = 0);
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)

    static int getFontHeight(const char* pFontName, float nSize);
    
    #endif
    

    unsigned char *   getData()               { return m_pData; }
    int               getDataLen()            { return m_nWidth * m_nHeight; }


    bool hasAlpha()                     { return m_bHasAlpha;   }
    bool isPremultipliedAlpha()         { return m_bPreMulti;   }


    /**
    @brief    Save CCImage data to the specified file, with specified format.
    @param    pszFilePath        the file's absolute path, including file suffix.
    @param    bIsToRGB        whether the image is saved as RGB format.
    */
    bool saveToFile(const char *pszFilePath, bool bIsToRGB = true);

    CC_SYNTHESIZE_READONLY(unsigned short,   m_nWidth,       Width);
    CC_SYNTHESIZE_READONLY(unsigned short,   m_nHeight,      Height);
    CC_SYNTHESIZE_READONLY(int,     m_nBitsPerComponent,   BitsPerComponent);

protected:
    bool _initWithJpgData(void *pData, int nDatalen);
    bool _initWithPngData(void *pData, int nDatalen);
    bool _initWithTiffData(void *pData, int nDataLen);
    bool _initWithWebpData(void *pData, int nDataLen);
    // @warning kFmtRawData only support RGBA8888
    bool _initWithRawData(void *pData, int nDatalen, int nWidth, int nHeight, int nBitsPerComponent, bool bPreMulti);

    bool _saveImageToPNG(const char *pszFilePath, bool bIsToRGB = true);
    bool _saveImageToJPG(const char *pszFilePath);

    unsigned char *m_pData;
    bool m_bHasAlpha;
    bool m_bPreMulti;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCFreeTypeFont* m_ft;
#endif

private:
    // noncopyable
    CCImage(const CCImage&    rImg);
    CCImage & operator=(const CCImage&);


};

// end of platform group
/// @}

NS_CC_END

#endif    // __CC_IMAGE_H__
