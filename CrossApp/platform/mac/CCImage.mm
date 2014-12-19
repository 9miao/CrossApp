/****************************************************************************
Copyright (c) 2010 9miao.com

http://www.9miao.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include <Foundation/Foundation.h>
#include <Cocoa/Cocoa.h>
#include "basics/CAApplication.h"
#include "ccMacros.h"
#include "CCImage.h"
#include "CCFileUtils.h"
#include "CAImage.h"
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

typedef struct
{
    unsigned int height;
    unsigned int width;
    int         bitsPerComponent;
    bool        hasAlpha;
    bool        isPremultipliedAlpha;
    unsigned char*  data;
} tImageInfo;

static unsigned int nextPOT(unsigned int x)
{
    x = x - 1;
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return x + 1;
}

typedef enum {
    kCAImagePixelFormat_Automatic = 0,
        //! 32-bit texture: RGBA8888
    kCAImagePixelFormat_RGBA8888,
        //! 24-bit texture: RGBA888
    kCAImagePixelFormat_RGB888,
        //! 16-bit Image without Alpha channel
    kCAImagePixelFormat_RGB565,
        //! 8-bit textures used as masks
    kCAImagePixelFormat_A8,
        //! 16-bit textures: RGBA4444
    kCAImagePixelFormat_RGBA4444,
        //! 16-bit textures: RGB5A1
    kCAImagePixelFormat_RGB5A1,    
    
        //! Default Image format: RGBA8888
    kCAImagePixelFormat_Default = kCAImagePixelFormat_RGBA8888,
    
        // backward compatibility stuff
    KImagePixelFormat_Automatic = kCAImagePixelFormat_Automatic,
    KImagePixelFormat_RGBA8888 = kCAImagePixelFormat_RGBA8888,
    KImagePixelFormat_RGB888 = kCAImagePixelFormat_RGB888,
    KImagePixelFormat_RGB565 = kCAImagePixelFormat_RGB565,
    KImagePixelFormat_A8 = kCAImagePixelFormat_A8,
    KImagePixelFormat_RGBA4444 = kCAImagePixelFormat_RGBA4444,
    KImagePixelFormat_RGB5A1 = kCAImagePixelFormat_RGB5A1,
    KImagePixelFormat_Default = kCAImagePixelFormat_Default
    
} CAImagePixelFormat;

static bool _initPremultipliedATextureWithImage(CGImageRef image, NSUInteger POTWide, NSUInteger POTHigh, tImageInfo *pImageInfo)
{
    NSUInteger            i;
    CGContextRef        context = nil;
    unsigned char*        data = nil;;
    CGColorSpaceRef        colorSpace;
    unsigned char*        tempData;
    unsigned int*        inPixel32;
    unsigned short*        outPixel16;
    bool                hasAlpha;
    CGImageAlphaInfo    info;
    CGSize                imageSize;
    CAImagePixelFormat    pixelFormat;
    
    info = CGImageGetAlphaInfo(image);
    hasAlpha = ((info == kCGImageAlphaPremultipliedLast) || (info == kCGImageAlphaPremultipliedFirst) || (info == kCGImageAlphaLast) || (info == kCGImageAlphaFirst) ? YES : NO);
    
    size_t bpp = CGImageGetBitsPerComponent(image);
    colorSpace = CGImageGetColorSpace(image);
    
    if(colorSpace) 
    {
        if(hasAlpha || bpp >= 8)
        {
            pixelFormat = kCAImagePixelFormat_Default;
        }
        else 
        {
            pixelFormat = kCAImagePixelFormat_RGB565;
        }
    } 
    else  
    {
        // NOTE: No colorspace means a mask image
        pixelFormat = kCAImagePixelFormat_A8;
    }
    
    imageSize.width = CGImageGetWidth(image);
    imageSize.height = CGImageGetHeight(image);
    
    // Create the bitmap graphics context
    
    switch(pixelFormat) 
    {      
        case kCAImagePixelFormat_RGBA8888:
        case kCAImagePixelFormat_RGBA4444:
        case kCAImagePixelFormat_RGB5A1:
            colorSpace = CGColorSpaceCreateDeviceRGB();
            data = new unsigned char[POTHigh * POTWide * 4];
            info = hasAlpha ? kCGImageAlphaPremultipliedLast : kCGImageAlphaNoneSkipLast;
            context = CGBitmapContextCreate(data, POTWide, POTHigh, 8, 4 * POTWide, colorSpace, info | kCGBitmapByteOrder32Big);                
            CGColorSpaceRelease(colorSpace);
            break;
            
        case kCAImagePixelFormat_RGB565:
            colorSpace = CGColorSpaceCreateDeviceRGB();
            data = new unsigned char[POTHigh * POTWide * 4];
            info = kCGImageAlphaNoneSkipLast;
            context = CGBitmapContextCreate(data, POTWide, POTHigh, 8, 4 * POTWide, colorSpace, info | kCGBitmapByteOrder32Big);
            CGColorSpaceRelease(colorSpace);
            break;
        case kCAImagePixelFormat_A8:
            data = new unsigned char[POTHigh * POTWide];
            info = kCGImageAlphaOnly;
            context = CGBitmapContextCreate(data, POTWide, POTHigh, 8, POTWide, NULL, info);
            break;            
        default:
            return false;
    }
    
    CGRect rect;
    rect.size.width = POTWide;
    rect.size.height = POTHigh;
    rect.origin.x = 0;
    rect.origin.y = 0;
    
    CGContextClearRect(context, rect);
    CGContextTranslateCTM(context, 0, 0);
    CGContextDrawImage(context, rect, image);
    
    // Repack the pixel data into the right format
    
    if(pixelFormat == kCAImagePixelFormat_RGB565) 
    {
        //Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGGBBBBB"
        tempData = new unsigned char[POTHigh * POTWide * 2];
        inPixel32 = (unsigned int*)data;
        outPixel16 = (unsigned short*)tempData;
        for(i = 0; i < POTWide * POTHigh; ++i, ++inPixel32)
        {
            *outPixel16++ = ((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | ((((*inPixel32 >> 8) & 0xFF) >> 2) << 5) | ((((*inPixel32 >> 16) & 0xFF) >> 3) << 0);
        }

        delete []data;
        data = tempData;
        
    }
    else if (pixelFormat == kCAImagePixelFormat_RGBA4444) 
    {
        //Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRGGGGBBBBAAAA"
        tempData = new unsigned char[POTHigh * POTWide * 2];
        inPixel32 = (unsigned int*)data;
        outPixel16 = (unsigned short*)tempData;
        for(i = 0; i < POTWide * POTHigh; ++i, ++inPixel32)
        {
            *outPixel16++ = 
            ((((*inPixel32 >> 0) & 0xFF) >> 4) << 12) | // R
            ((((*inPixel32 >> 8) & 0xFF) >> 4) << 8) | // G
            ((((*inPixel32 >> 16) & 0xFF) >> 4) << 4) | // B
            ((((*inPixel32 >> 24) & 0xFF) >> 4) << 0); // A
        }       
        
        delete []data;
        data = tempData;
        
    }
    else if (pixelFormat == kCAImagePixelFormat_RGB5A1) 
    {
        //Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGBBBBBA"
        tempData = new unsigned char[POTHigh * POTWide * 2];
        inPixel32 = (unsigned int*)data;
        outPixel16 = (unsigned short*)tempData;
        for(i = 0; i < POTWide * POTHigh; ++i, ++inPixel32)
        {
            *outPixel16++ = 
            ((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | // R
            ((((*inPixel32 >> 8) & 0xFF) >> 3) << 6) | // G
            ((((*inPixel32 >> 16) & 0xFF) >> 3) << 1) | // B
            ((((*inPixel32 >> 24) & 0xFF) >> 7) << 0); // A
        }
                
        delete []data;
        data = tempData;
    }
    
    // should be after calling super init
    pImageInfo->isPremultipliedAlpha = true;
    pImageInfo->hasAlpha = true;
    pImageInfo->bitsPerComponent = bpp;
    pImageInfo->width = POTWide;
    pImageInfo->height = POTHigh;
    
    if (pImageInfo->data)
    {
        delete [] pImageInfo->data;
    }
    pImageInfo->data = data;
    
    CGContextRelease(context);
    return true;
}
// TODO: rename _initWithImage, it also makes a draw call.
static bool _initWithImage(CGImageRef CGImage, tImageInfo *pImageinfo, double scaleX, double scaleY)
{
    NSUInteger POTWide, POTHigh;
    
    if(CGImage == NULL) 
    {
        return false;
    }
    
	//if (CrossApp::CCImage::getIsScaleEnabled())
	if( CrossApp::CAApplication::getApplication()->getContentScaleFactor() > 1.0f )
	{
		POTWide = CGImageGetWidth(CGImage) * scaleX;
		POTHigh = CGImageGetHeight(CGImage) * scaleY;
	}
	else 
	{
		POTWide = CGImageGetWidth(CGImage);
		POTHigh = CGImageGetHeight(CGImage);
	}

    
    // load and draw image
    return _initPremultipliedATextureWithImage(CGImage, POTWide, POTHigh, pImageinfo);
}

static bool _initWithFile(const char* path, tImageInfo *pImageinfo)
{
    CGImageRef                CGImage;    
    NSImage                    *jpg;
    //NSImage                    *png;
    bool            ret;
    
    // convert jpg to png before loading the texture
    
    NSString *fullPath = [NSString stringWithUTF8String:path];
    jpg = [[NSImage alloc] initWithContentsOfFile: fullPath];
    //png = [[NSImage alloc] initWithData:UIImagePNGRepresentation(jpg)];
    CGImageSourceRef source = CGImageSourceCreateWithData((CFDataRef)[jpg TIFFRepresentation], NULL);
    CGImage = CGImageSourceCreateImageAtIndex(source, 0, NULL);
    
    ret = _initWithImage(CGImage, pImageinfo, 1.0, 1.0);
    
    //[png release];
    [jpg release];
    if (CGImage) CFRelease(CGImage);
    if (source) CFRelease(source);
  
    return ret;
}

// TODO: rename _initWithData, it also makes a draw call.
static bool _initWithData(void * pBuffer, int length, tImageInfo *pImageinfo, double scaleX, double scaleY)
{
    bool ret = false;
    
    if (pBuffer) 
    {
        CGImageRef CGImage;
        NSData *data;
        
        data = [NSData dataWithBytes:pBuffer length:length];
		CGImageSourceRef source = CGImageSourceCreateWithData((CFDataRef)data, NULL);
        CGImage = CGImageSourceCreateImageAtIndex(source, 0, NULL);
        
        ret = _initWithImage(CGImage, pImageinfo, scaleX, scaleY);
        if (CGImage) CFRelease(CGImage);
        if (source) CFRelease(source);
    }
    return ret;
}


NS_CC_BEGIN

bool isFileExists(const char* szFilePath);

bool isFileExists(const char* szFilePath)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	//TCHAR dirpath[MAX_PATH];
	//MultiByteToWideChar(936,0,szFilePath,-1,dirpath,sizeof(dirpath));
	DWORD dwFileAttr = GetFileAttributesA(szFilePath);
	if (INVALID_FILE_ATTRIBUTES == dwFileAttr
		|| (dwFileAttr&FILE_ATTRIBUTE_DIRECTORY))	{
		return false;
	}		
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	bool bFind = true;
	do 
	{
		struct stat buf;
		int n = stat(szFilePath, &buf);
		if ((0 != n)
			|| !(buf.st_mode&S_IFMT))	
		{
			bFind = false;
		}
	} while (0);
	if (!bFind)
	{
		//std::string strFilenName = s_strRelativePath + szFilePath;
		unsigned char * pBuffer = NULL;
		unzFile pFile = NULL;
		unsigned long pSize = 0;
		
		do 
		{
			pFile = unzOpen(s_strAndroidPackagePath.c_str());
			if(!pFile)break;
			
			int nRet = unzLocateFile(pFile, szFilePath, 1);
			if(UNZ_OK != nRet)
				bFind = false;
			else
				bFind = true;
		} while (0);
		
		if (pFile)
		{
			unzClose(pFile);
		}
	}
	
	return bFind;
#else
	struct stat buf;
	int n = stat(szFilePath, &buf);
	if ((0 != n)
		|| !(buf.st_mode&S_IFMT))	{
		return false;
	}		
	
#endif
	return true;
}

CCImage::CCImage()
: m_nWidth(0)
, m_nHeight(0)
, m_nBitsPerComponent(0)
, m_pData(0)
, m_bHasAlpha(false)
, m_bPreMulti(false)
{
    
}

CCImage::~CCImage()
{
    CC_SAFE_DELETE_ARRAY(m_pData);
}

bool CCImage::initWithImageFile(const char * strPath, EImageFormat eImgFmt/* = eFmtPng*/)
{
    std::string strTemp = CCFileUtils::sharedFileUtils()->fullPathForFilename(strPath);
    
	unsigned long fileSize = 0;
	unsigned char* pFileData = CCFileUtils::sharedFileUtils()->getFileData(strTemp.c_str(), "rb", &fileSize);
	bool ret = initWithImageData(pFileData, fileSize, eImgFmt);
	delete []pFileData;
	return ret;
}

bool CCImage::initWithImageFileThreadSafe(const char *fullpath, EImageFormat imageType)
{
    /*
     * CCFileUtils::fullPathFromRelativePath() is not thread-safe, it use autorelease().
     */
    bool bRet = false;
    unsigned long nSize = 0;
    unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(fullpath, "rb", &nSize);
    if (pBuffer != NULL && nSize > 0)
    {
        bRet = initWithImageData(pBuffer, nSize, imageType);
    }
    CC_SAFE_DELETE_ARRAY(pBuffer);
    return bRet;
}



//bool CCImage::initWithImageData(void * pData, int nDataLen, EImageFormat eFmt/* = eSrcFmtPng*/)
bool CCImage::initWithImageData(void * pData, 
                           int nDataLen, 
                           EImageFormat eFmt,
                           int nWidth,
                           int nHeight,
                           int nBitsPerComponent)
{
    bool bRet = false;
    tImageInfo info = {0};
    do 
    {
        CC_BREAK_IF(! pData || nDataLen <= 0);
        
        if (eFmt == kFmtRawData)
        {
            bRet = _initWithRawData(pData, nDataLen, nWidth, nHeight, nBitsPerComponent, false);
        }
        else if (eFmt == kFmtWebp)
        {
            bRet = _initWithWebpData(pData, nDataLen);
        }
        else
        {
            bRet = _initWithData(pData, nDataLen, &info, 1.0f, 1.0f);//m_dScaleX, m_dScaleY);
            if (bRet)
            {
                m_nHeight = (short)info.height;
                m_nWidth = (short)info.width;
                m_nBitsPerComponent = info.bitsPerComponent;
                if (eFmt == kFmtJpg)
                {
                    m_bHasAlpha = true;
                    m_bPreMulti = false;
                }
                else
                {
                    m_bHasAlpha = info.hasAlpha;
                    m_bPreMulti = info.isPremultipliedAlpha;
                }
                m_pData = info.data;
            }
        }
    } while (0);
	
    return bRet;
}

bool CCImage::_initWithRawData(void *pData, int nDatalen, int nWidth, int nHeight, int nBitsPerComponent, bool bPreMulti)
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(0 == nWidth || 0 == nHeight);
        
        m_nBitsPerComponent = nBitsPerComponent;
        m_nHeight   = (short)nHeight;
        m_nWidth    = (short)nWidth;
        m_bHasAlpha = true;
        
        m_pData = (unsigned char*)pData;

        // only RGBA8888 supported
				int nBytesPerComponent = 4;
				int nSize = nHeight * nWidth * nBytesPerComponent;
				m_pData = new unsigned char[nSize];
				CC_BREAK_IF(!m_pData);
				memcpy(m_pData, pData, nSize);
        
        bRet = true;
    } while (0);
    return bRet;
}

bool CCImage::saveToFile(const char *pszFilePath, bool bIsToRGB)
{
	assert(false);
	return false;
}



NS_CC_END

