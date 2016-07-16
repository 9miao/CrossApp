//
//  CAImage.cpp
//  CrossApp
//
//  Created by 栗元峰 on 15-3-23.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#include "CAImage.h"
#include "CAImageCache.h"
#include "ccConfig.h"
#include "ccMacros.h"
#include "CCGL.h"
#include "CCStdC.h"
#include "support/ccUtils.h"
#include "support/zip_support/ZipUtils.h"
#include "platform/platform.h"
#include "platform/CCPlatformConfig.h"
#include "platform/CCFileUtils.h"
#include "platform/CAFTFontCache.h"
#include "basics/CAApplication.h"
#include "shaders/CAGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CAShaderCache.h"
#include "support/image_support/TGAlib.h"
#include "images/gif_lib/gif_lib.h"
#include "images/gif_lib/gif_hash.h"
#include "images/gif_lib/gif_lib_private.h"
#include "images/gif_lib/GifUtils.h"

#include <ctype.h>
#include <cctype>
#include <png.h>
#include <jpeglib.h>
#include <tiffio.h>
#include <decode.h>
#include <etc1.h>


NS_CC_BEGIN


CAImage* CAImage::scaleToNewImageWithImage(CAImage* image, const DSize& size)
{
    CARenderImage* renderImage = CARenderImage::create(size.width, size.height);
    renderImage->begin();
    
    GLfloat    coordinates[] =
    {
        0.0f,                   0.0f,
        image->getMaxS(),       0.0f,
        0.0f,                   image->getMaxT(),
        image->getMaxS(),       image->getMaxT()
    };
    
    GLfloat    vertices[] =
    {
        0,            0,               /*0.0f,*/
        size.width,   0,                    /*0.0f,*/
        0,            size.height,     /*0.0f,*/
        size.width,   size.height           /*0.0f*/
    };
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );
    
    image->getShaderProgram()->use();
    image->getShaderProgram()->setUniformsForBuiltins();
    
    ccGLBindTexture2D( image->getName() );
    
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
    
    renderImage->end();
    return renderImage->getImageView()->getImage();
}

CAImage* CAImage::scaleToNewImageWithImage(CAImage* image, float scaleX, float scaleY)
{
    DSize size;
    size.width = image->getContentSize().width * scaleX;
    size.height = image->getContentSize().height * scaleY;
    return scaleToNewImageWithImage(image, size);
}

CAImage* generateMipmapsWithImageRGB(CAImage* image)
{
    int	new_w = image->getPixelsWide() >> 1;
    int	new_h = image->getPixelsHigh() >> 1;
    if (new_w < 1) new_w = 1;
    if (new_h < 1) new_h = 1;
    
    int	new_pitch = new_w * 3;
    
    new_pitch = (new_pitch + 3) & ~3;
    
    //    if (new_w * 2 != m_uPixelsWide || new_h * 2 != m_uPixelsHigh)
    //    {
    //    }
    //    else
    {
        CAImage* newImage = new CAImage();
        unsigned char* data = new unsigned char[new_pitch * new_h];
        // Resample.  Simple average 2x2 --> 1, in-place.
        int	pitch = image->getPixelsWide() * 3;
        
        for (int j = 0; j < new_h; j++)
        {
            unsigned char*	out = ((unsigned char*)data) + j * new_pitch;
            unsigned char*	in = ((unsigned char*)image->getData()) + (j << 1) * pitch;
            for (int i = 0; i < new_w; i++)
            {
                int	r, g, b;
                r = (*(in + 0) + *(in + 3) + *(in + 0 + pitch) + *(in + 3 + pitch));
                g = (*(in + 1) + *(in + 4) + *(in + 1 + pitch) + *(in + 4 + pitch));
                b = (*(in + 2) + *(in + 5) + *(in + 2 + pitch) + *(in + 5 + pitch));
                *(out + 0) = r >> 2;
                *(out + 1) = g >> 2;
                *(out + 2) = b >> 2;
                out += 3;
                in += 6;
            }
        }
        
        newImage->initWithRawData(data, CAImage::PixelFormat_RGB888, new_w, new_h);
        newImage->autorelease();
        delete[] data;
        
        return newImage;
    }
    return 0;
}

CAImage* generateMipmapsWithImageRGBA(CAImage* image)
{
    int	new_w = image->getPixelsWide() >> 1;
    int	new_h = image->getPixelsHigh() >> 1;
    if (new_w < 1) new_w = 1;
    if (new_h < 1) new_h = 1;
    
    int	new_pitch = new_w * 4;
    
    //    if (new_w * 2 != m_uPixelsWide || new_h * 2 != m_uPixelsHigh)
    //    {
    //    }
    //    else
    {
        CAImage* newImage = new CAImage();
        unsigned char* data = new unsigned char[new_pitch * new_h];
        
        // Resample.  Simple average 2x2 --> 1, in-place.
        int	pitch = image->getPixelsWide() * 4;
        for (int j = 0; j < new_h; j++)
        {
            unsigned char*	out = ((unsigned char*)data) + j * new_pitch;
            unsigned char*	in = ((unsigned char*)image->getData()) + (j << 1) * pitch;
            for (int i = 0; i < new_w; i++)
            {
                int	r, g, b, a;
                r = (*(in + 0) + *(in + 4) + *(in + 0 + pitch) + *(in + 4 + pitch));
                g = (*(in + 1) + *(in + 5) + *(in + 1 + pitch) + *(in + 5 + pitch));
                b = (*(in + 2) + *(in + 6) + *(in + 2 + pitch) + *(in + 6 + pitch));
                a = (*(in + 3) + *(in + 7) + *(in + 3 + pitch) + *(in + 7 + pitch));
                *(out + 0) = r >> 2;
                *(out + 1) = g >> 2;
                *(out + 2) = b >> 2;
                *(out + 3) = a >> 2;
                out += 4;
                in += 8;
            }
        }
        newImage->initWithRawData(data, CAImage::PixelFormat_RGBA8888, new_w, new_h);
        newImage->autorelease();
        delete[] data;
        
        return newImage;
    }

    return 0;
}


CAImage* CAImage::generateMipmapsWithImage(CAImage* image)
{
    if (DPoint(image->getContentSize()).getLength() < 2895)
    {
        CAImage* newImage = new CAImage();
        newImage->initWithRawData(image->getData(), image->getPixelFormat(), image->getPixelsWide(), image->getPixelsHigh());
        newImage->autorelease();
        return newImage;
    }
    
    if (image->getPixelFormat() == CAImage::PixelFormat_RGB888)
    {
        return generateMipmapsWithImageRGB(image);
    }
    else if (image->getPixelFormat() == CAImage::PixelFormat_RGBA8888)
    {
        return generateMipmapsWithImageRGBA(image);
    }
    
    return NULL;
}

extern "C"
{
    
    size_t fwrite$UNIX2003( const void *a, size_t b, size_t c, FILE *d )
    {
        return fwrite(a, b, c, d);
    }
    
    char* strerror$UNIX2003( int errnum )
    {
        return strerror(errnum);
    }
}

namespace
{
    /*
     * ERROR HANDLING:
     *
     * The JPEG library's standard error handler (jerror.c) is divided into
     * several "methods" which you can override individually.  This lets you
     * adjust the behavior without duplicating a lot of code, which you might
     * have to update with each future release.
     *
     * We override the "error_exit" method so that control is returned to the
     * library's caller when a fatal error occurs, rather than calling exit()
     * as the standard error_exit method does.
     *
     * We use C's setjmp/longjmp facility to return control.  This means that the
     * routine which calls the JPEG library must first execute a setjmp() call to
     * establish the return point.  We want the replacement error_exit to do a
     * longjmp().  But we need to make the setjmp buffer accessible to the
     * error_exit routine.  To do this, we make a private extension of the
     * standard JPEG error handler object.  (If we were using C++, we'd say we
     * were making a subclass of the regular error handler.)
     *
     * Here's the extended error handler struct:
     */
    struct MyErrorMgr
    {
        struct jpeg_error_mgr pub;	/* "public" fields */
        jmp_buf setjmp_buffer;	/* for return to caller */
    };
    
    typedef struct MyErrorMgr * MyErrorPtr;
    
    /*
     * Here's the routine that will replace the standard error_exit method:
     */
    
    METHODDEF(void)
    myErrorExit(j_common_ptr cinfo)
    {
        /* cinfo->err really points to a MyErrorMgr struct, so coerce pointer */
        MyErrorPtr myerr = (MyErrorPtr) cinfo->err;
        
        /* Always display the message. */
        /* We could postpone this until after returning, if we chose. */
        /* internal message function cann't show error message in some platforms, so we rewrite it here.
         * edit it if has version confilict.
         */
        //(*cinfo->err->output_message) (cinfo);
        char buffer[JMSG_LENGTH_MAX];
        (*cinfo->err->format_message) (cinfo, buffer);
        CCLOG("jpeg error: %s", buffer);
        
        /* Return control to the setjmp point */
        longjmp(myerr->setjmp_buffer, 1);
    }
}

namespace
{
    typedef struct
    {
        const unsigned char * data;
        unsigned long size;
        int offset;
    }tImageSource;
    
    void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
    {
        tImageSource* isource = (tImageSource*)png_get_io_ptr(png_ptr);
        
        if((int)(isource->offset + length) <= isource->size)
        {
            memcpy(data, isource->data+isource->offset, length);
            isource->offset += length;
        }
        else
        {
            png_error(png_ptr, "pngReaderCallback failed");
        }
    }
}

namespace
{
    static tmsize_t tiffReadProc(thandle_t fd, void* buf, tmsize_t size)
    {
        tImageSource* isource = (tImageSource*)fd;
        uint8* ma;
        uint64 mb;
        unsigned long n;
        unsigned long o;
        tmsize_t p;
        ma=(uint8*)buf;
        mb=size;
        p=0;
        while (mb>0)
        {
            n=0x80000000UL;
            if ((uint64)n>mb)
                n=(unsigned long)mb;
            
            
            if((int)(isource->offset + n) <= isource->size)
            {
                memcpy(ma, isource->data+isource->offset, n);
                isource->offset += n;
                o = n;
            }
            else
            {
                return 0;
            }
            
            ma+=o;
            mb-=o;
            p+=o;
            if (o!=n)
            {
                break;
            }
        }
        return p;
    }
    
    static tmsize_t tiffWriteProc(thandle_t fd, void* buf, tmsize_t size)
    {
        CC_UNUSED_PARAM(fd);
        CC_UNUSED_PARAM(buf);
        CC_UNUSED_PARAM(size);
        return 0;
    }
    
    
    static uint64 tiffSeekProc(thandle_t fd, uint64 off, int whence)
    {
        tImageSource* isource = (tImageSource*)fd;
        uint64 ret = -1;
        do
        {
            if (whence == SEEK_SET)
            {
                CC_BREAK_IF(off >= (uint64)isource->size);
                ret = isource->offset = (uint32)off;
            }
            else if (whence == SEEK_CUR)
            {
                CC_BREAK_IF(isource->offset + off >= (uint64)isource->size);
                ret = isource->offset += (uint32)off;
            }
            else if (whence == SEEK_END)
            {
                CC_BREAK_IF(off >= (uint64)isource->size);
                ret = isource->offset = (uint32)(isource->size-1 - off);
            }
            else
            {
                CC_BREAK_IF(off >= (uint64)isource->size);
                ret = isource->offset = (uint32)off;
            }
        } while (0);
        
        return ret;
    }
    
    static uint64 tiffSizeProc(thandle_t fd)
    {
        tImageSource* pImageSrc = (tImageSource*)fd;
        return pImageSrc->size;
    }
    
    static int tiffCloseProc(thandle_t fd)
    {
        CC_UNUSED_PARAM(fd);
        return 0;
    }
    
    static int tiffMapProc(thandle_t fd, void** pbase, toff_t* psize)
    {
        CC_UNUSED_PARAM(fd);
        CC_UNUSED_PARAM(pbase);
        CC_UNUSED_PARAM(psize);
        return 0;
    }
    
    void tiffUnmapProc(thandle_t fd, void* base, toff_t size)
    {
        CC_UNUSED_PARAM(fd);
        CC_UNUSED_PARAM(base);
        CC_UNUSED_PARAM(size);
    }
}


// IIIIIIII -> RRRRRRRRGGGGGGGGGBBBBBBBB
void CAImage::convertI8ToRGB888(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    for (unsigned long i=0; i < dataLen; ++i)
    {
        *outData++ = data[i];     //R
        *outData++ = data[i];     //G
        *outData++ = data[i];     //B
    }
}

// IIIIIIIIAAAAAAAA -> RRRRRRRRGGGGGGGGBBBBBBBB
void CAImage::convertAI88ToRGB888(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    for (unsigned long i = 0, l = dataLen - 1; i < l; i += 2)
    {
        *outData++ = data[i];     //R
        *outData++ = data[i];     //G
        *outData++ = data[i];     //B
    }
}

// IIIIIIII -> RRRRRRRRGGGGGGGGGBBBBBBBBAAAAAAAA
void CAImage::convertI8ToRGBA8888(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    for (unsigned long i = 0; i < dataLen; ++i)
    {
        *outData++ = data[i];     //R
        *outData++ = data[i];     //G
        *outData++ = data[i];     //B
        *outData++ = 0xFF;        //A
    }
}

// IIIIIIIIAAAAAAAA -> RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA
void CAImage::convertAI88ToRGBA8888(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    for (unsigned long i = 0, l = dataLen - 1; i < l; i += 2)
    {
        *outData++ = data[i];     //R
        *outData++ = data[i];     //G
        *outData++ = data[i];     //B
        *outData++ = data[i + 1]; //A
    }
}

// IIIIIIII -> RRRRRGGGGGGBBBBB
void CAImage::convertI8ToRGB565(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (int i = 0; i < dataLen; ++i)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
        | (data[i] & 0x00FC) << 3         //G
        | (data[i] & 0x00F8) >> 3;        //B
    }
}

// IIIIIIIIAAAAAAAA -> RRRRRGGGGGGBBBBB
void CAImage::convertAI88ToRGB565(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (unsigned long i = 0, l = dataLen - 1; i < l; i += 2)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
        | (data[i] & 0x00FC) << 3         //G
        | (data[i] & 0x00F8) >> 3;        //B
    }
}

// IIIIIIII -> RRRRGGGGBBBBAAAA
void CAImage::convertI8ToRGBA4444(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (unsigned long i = 0; i < dataLen; ++i)
    {
        *out16++ = (data[i] & 0x00F0) << 8    //R
        | (data[i] & 0x00F0) << 4             //G
        | (data[i] & 0x00F0)                  //B
        | 0x000F;                             //A
    }
}

// IIIIIIIIAAAAAAAA -> RRRRGGGGBBBBAAAA
void CAImage::convertAI88ToRGBA4444(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (unsigned long i = 0, l = dataLen - 1; i < l; i += 2)
    {
        *out16++ = (data[i] & 0x00F0) << 8    //R
        | (data[i] & 0x00F0) << 4             //G
        | (data[i] & 0x00F0)                  //B
        | (data[i+1] & 0x00F0) >> 4;          //A
    }
}

// IIIIIIII -> RRRRRGGGGGBBBBBA
void CAImage::convertI8ToRGB5A1(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (int i = 0; i < dataLen; ++i)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
        | (data[i] & 0x00F8) << 3         //G
        | (data[i] & 0x00F8) >> 2         //B
        | 0x0001;                         //A
    }
}

// IIIIIIIIAAAAAAAA -> RRRRRGGGGGBBBBBA
void CAImage::convertAI88ToRGB5A1(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (unsigned long i = 0, l = dataLen - 1; i < l; i += 2)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
        | (data[i] & 0x00F8) << 3         //G
        | (data[i] & 0x00F8) >> 2         //B
        | (data[i + 1] & 0x0080) >> 7;    //A
    }
}

// IIIIIIII -> IIIIIIIIAAAAAAAA
void CAImage::convertI8ToAI88(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (unsigned long i = 0; i < dataLen; ++i)
    {
        *out16++ = 0xFF00     //A
        | data[i];            //I
    }
}

// IIIIIIIIAAAAAAAA -> AAAAAAAA
void CAImage::convertAI88ToA8(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    for (unsigned long i = 1; i < dataLen; i += 2)
    {
        *outData++ = data[i]; //A
    }
}

// IIIIIIIIAAAAAAAA -> IIIIIIII
void CAImage::convertAI88ToI8(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    for (unsigned long i = 0, l = dataLen - 1; i < l; i += 2)
    {
        *outData++ = data[i]; //R
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBB -> RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA
void CAImage::convertRGB888ToRGBA8888(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    for (unsigned long i = 0, l = dataLen - 2; i < l; i += 3)
    {
        *outData++ = data[i];         //R
        *outData++ = data[i + 1];     //G
        *outData++ = data[i + 2];     //B
        *outData++ = 0xFF;            //A
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA -> RRRRRRRRGGGGGGGGBBBBBBBB
void CAImage::convertRGBA8888ToRGB888(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    for (unsigned long i = 0, l = dataLen - 3; i < l; i += 4)
    {
        *outData++ = data[i];         //R
        *outData++ = data[i + 1];     //G
        *outData++ = data[i + 2];     //B
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBB -> RRRRRGGGGGGBBBBB
void CAImage::convertRGB888ToRGB565(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (unsigned long i = 0, l = dataLen - 2; i < l; i += 3)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
        | (data[i + 1] & 0x00FC) << 3     //G
        | (data[i + 2] & 0x00F8) >> 3;    //B
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA -> RRRRRGGGGGGBBBBB
void CAImage::convertRGBA8888ToRGB565(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (unsigned long i = 0, l = dataLen - 3; i < l; i += 4)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
        | (data[i + 1] & 0x00FC) << 3     //G
        | (data[i + 2] & 0x00F8) >> 3;    //B
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBB -> IIIIIIII
void CAImage::convertRGB888ToI8(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    for (unsigned long i = 0, l = dataLen - 2; i < l; i += 3)
    {
        *outData++ = (data[i] * 299 + data[i + 1] * 587 + data[i + 2] * 114 + 500) / 1000;  //I =  (R*299 + G*587 + B*114 + 500) / 1000
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA -> IIIIIIII
void CAImage::convertRGBA8888ToI8(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    for (unsigned long i = 0, l = dataLen - 3; i < l; i += 4)
    {
        *outData++ = (data[i] * 299 + data[i + 1] * 587 + data[i + 2] * 114 + 500) / 1000;  //I =  (R*299 + G*587 + B*114 + 500) / 1000
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA -> AAAAAAAA
void CAImage::convertRGBA8888ToA8(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    for (unsigned long i = 0, l = dataLen -3; i < l; i += 4)
    {
        *outData++ = data[i + 3]; //A
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBB -> IIIIIIIIAAAAAAAA
void CAImage::convertRGB888ToAI88(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    for (unsigned long i = 0, l = dataLen - 2; i < l; i += 3)
    {
        *outData++ = (data[i] * 299 + data[i + 1] * 587 + data[i + 2] * 114 + 500) / 1000;  //I =  (R*299 + G*587 + B*114 + 500) / 1000
        *outData++ = 0xFF;
    }
}


// RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA -> IIIIIIIIAAAAAAAA
void CAImage::convertRGBA8888ToAI88(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    for (unsigned long i = 0, l = dataLen - 3; i < l; i += 4)
    {
        *outData++ = (data[i] * 299 + data[i + 1] * 587 + data[i + 2] * 114 + 500) / 1000;  //I =  (R*299 + G*587 + B*114 + 500) / 1000
        *outData++ = data[i + 3];
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBB -> RRRRGGGGBBBBAAAA
void CAImage::convertRGB888ToRGBA4444(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (unsigned long i = 0, l = dataLen - 2; i < l; i += 3)
    {
        *out16++ = ((data[i] & 0x00F0) << 8           //R
                    | (data[i + 1] & 0x00F0) << 4     //G
                    | (data[i + 2] & 0xF0)            //B
                    |  0x0F);                         //A
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA -> RRRRGGGGBBBBAAAA
void CAImage::convertRGBA8888ToRGBA4444(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (unsigned long i = 0, l = dataLen - 3; i < l; i += 4)
    {
        *out16++ = (data[i] & 0x00F0) << 8    //R
        | (data[i + 1] & 0x00F0) << 4         //G
        | (data[i + 2] & 0xF0)                //B
        |  (data[i + 3] & 0xF0) >> 4;         //A
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBB -> RRRRRGGGGGBBBBBA
void CAImage::convertRGB888ToRGB5A1(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (unsigned long i = 0, l = dataLen - 2; i < l; i += 3)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
        | (data[i + 1] & 0x00F8) << 3     //G
        | (data[i + 2] & 0x00F8) >> 2     //B
        |  0x01;                          //A
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBB -> RRRRRGGGGGBBBBBA
void CAImage::convertRGBA8888ToRGB5A1(const unsigned char* data, unsigned long dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (unsigned long i = 0, l = dataLen - 2; i < l; i += 4)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
        | (data[i + 1] & 0x00F8) << 3     //G
        | (data[i + 2] & 0x00F8) >> 2     //B
        |  (data[i + 3] & 0x0080) >> 7;   //A
    }
}

CAImage::PixelFormat CAImage::convertI8ToFormat(const unsigned char* data, unsigned long dataLen,  CAImage::PixelFormat format, unsigned char** outData, unsigned long* outDataLen)
{
    switch (format)
    {
        case CAImage::PixelFormat_RGBA8888:
            *outDataLen = dataLen*4;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertI8ToRGBA8888(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_RGB888:
            *outDataLen = dataLen*3;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertI8ToRGB888(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_RGB565:
            *outDataLen = dataLen*2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertI8ToRGB565(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_AI88:
            *outDataLen = dataLen*2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertI8ToAI88(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_RGBA4444:
            *outDataLen = dataLen*2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertI8ToRGBA4444(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_RGB5A1:
            *outDataLen = dataLen*2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertI8ToRGB5A1(data, dataLen, *outData);
            break;
        default:
            // unsupport convertion or don't need to convert
            if (format != CAImage::PixelFormat_Default && format != CAImage::PixelFormat_I8)
            {
                CCLOG("Can not convert image format CAImage::PixelFormat_I8 to format ID:%d, we will use it's origin format CAImage::PixelFormat_I8", format);
            }
            
            *outData = (unsigned char*)data;
            *outDataLen = dataLen;
            return CAImage::PixelFormat_I8;
    }
    
    return format;
}

CAImage::PixelFormat CAImage::convertAI88ToFormat(const unsigned char* data, unsigned long dataLen,  CAImage::PixelFormat format, unsigned char** outData, unsigned long* outDataLen)
{
    switch (format)
    {
        case CAImage::PixelFormat_RGBA8888:
            *outDataLen = dataLen*2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertAI88ToRGBA8888(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_RGB888:
            *outDataLen = dataLen/2*3;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertAI88ToRGB888(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_RGB565:
            *outDataLen = dataLen;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertAI88ToRGB565(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_A8:
            *outDataLen = dataLen/2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertAI88ToA8(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_I8:
            *outDataLen = dataLen/2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertAI88ToI8(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_RGBA4444:
            *outDataLen = dataLen;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertAI88ToRGBA4444(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_RGB5A1:
            *outDataLen = dataLen;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertAI88ToRGB5A1(data, dataLen, *outData);
            break;
        default:
            // unsupport convertion or don't need to convert
            if (format != CAImage::PixelFormat_Default && format != CAImage::PixelFormat_AI88)
            {
                CCLOG("Can not convert image format CAImage::PixelFormat_AI88 to format ID:%d, we will use it's origin format CAImage::PixelFormat_AI88", format);
            }
            
            *outData = (unsigned char*)data;
            *outDataLen = dataLen;
            return CAImage::PixelFormat_AI88;
            break;
    }
    
    return format;
}

CAImage::PixelFormat CAImage::convertRGB888ToFormat(const unsigned char* data, unsigned long dataLen, CAImage::PixelFormat format, unsigned char** outData, unsigned long* outDataLen)
{
    switch (format)
    {
        case CAImage::PixelFormat_RGBA8888:
            *outDataLen = dataLen/3*4;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGB888ToRGBA8888(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_RGB565:
            *outDataLen = dataLen/3*2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGB888ToRGB565(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_I8:
            *outDataLen = dataLen/3;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGB888ToI8(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_AI88:
            *outDataLen = dataLen/3*2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGB888ToAI88(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_RGBA4444:
            *outDataLen = dataLen/3*2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGB888ToRGBA4444(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_RGB5A1:
            *outDataLen = dataLen;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGB888ToRGB5A1(data, dataLen, *outData);
            break;
        default:
            // unsupport convertion or don't need to convert
            if (format != CAImage::PixelFormat_Default && format != CAImage::PixelFormat_RGB888)
            {
                CCLOG("Can not convert image format CAImage::PixelFormat_RGB888 to format ID:%d, we will use it's origin format CAImage::PixelFormat_RGB888", format);
            }
            
            *outData = (unsigned char*)data;
            *outDataLen = dataLen;
            return CAImage::PixelFormat_RGB888;
    }
    return format;
}

CAImage::PixelFormat CAImage::convertRGBA8888ToFormat(const unsigned char* data, unsigned long dataLen, CAImage::PixelFormat format, unsigned char** outData, unsigned long* outDataLen)
{
    
    switch (format)
    {
        case CAImage::PixelFormat_RGB888:
            *outDataLen = dataLen/4*3;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGBA8888ToRGB888(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_RGB565:
            *outDataLen = dataLen/2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGBA8888ToRGB565(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_A8:
            *outDataLen = dataLen/4;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGBA8888ToA8(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_I8:
            *outDataLen = dataLen/4;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGBA8888ToI8(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_AI88:
            *outDataLen = dataLen/2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGBA8888ToAI88(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_RGBA4444:
            *outDataLen = dataLen/2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGBA8888ToRGBA4444(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat_RGB5A1:
            *outDataLen = dataLen/2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGBA8888ToRGB5A1(data, dataLen, *outData);
            break;
        default:
            // unsupport convertion or don't need to convert
            if (format != CAImage::PixelFormat_Default && format != CAImage::PixelFormat_RGBA8888)
            {
                CCLOG("Can not convert image format CAImage::PixelFormat_RGBA8888 to format ID:%d, we will use it's origin format CAImage::PixelFormat_RGBA8888", format);
            }
            
            *outData = (unsigned char*)data;
            *outDataLen = dataLen;
            return CAImage::PixelFormat_RGBA8888;
    }
    
    return format;
}

/*
 convert map:
 1.CAImage::PixelFormat_RGBA8888
 2.CAImage::PixelFormat_RGB888
 3.CAImage::PixelFormat_RGB565
 4.CAImage::PixelFormat_A8
 5.CAImage::PixelFormat_I8
 6.CAImage::PixelFormat_AI88
 7.CAImage::PixelFormat_RGBA4444
 8.CAImage::PixelFormat_RGB5A1
 
 gray(5) -> 1235678
 gray alpha(6) -> 12345678
 rgb(2) -> 1235678
 rgba(1) -> 12345678
 
 */
CAImage::PixelFormat CAImage::convertDataToFormat(const unsigned char* data, unsigned long dataLen, CAImage::PixelFormat originFormat, CAImage::PixelFormat format, unsigned char** outData, unsigned long* outDataLen)
{
    // don't need to convert
    if (format == originFormat || format == CAImage::PixelFormat_Default)
    {
        *outData = (unsigned char*)data;
        *outDataLen = dataLen;
        return originFormat;
    }
    
    switch (originFormat)
    {
        case CAImage::PixelFormat_I8:
            return convertI8ToFormat(data, dataLen, format, outData, outDataLen);
        case CAImage::PixelFormat_AI88:
            return convertAI88ToFormat(data, dataLen, format, outData, outDataLen);
        case CAImage::PixelFormat_RGB888:
            return convertRGB888ToFormat(data, dataLen, format, outData, outDataLen);
        case CAImage::PixelFormat_RGBA8888:
            return convertRGBA8888ToFormat(data, dataLen, format, outData, outDataLen);
        default:
            *outData = (unsigned char*)data;
            *outDataLen = dataLen;
            return originFormat;
    }
}

static std::set<CAImage*> s_pImages;

static CAImage* cc_white_image = NULL;

static const unsigned char* s_pData = NULL;
static int  s_pDataMark = 0;

static int DecodeCallBackProc(GifFileType* gif, GifByteType* bytes, int size)
{
    for(int i=0; i<size; i++, s_pDataMark++)
    {
        bytes[i] = s_pData[s_pDataMark];
    }
    return size;
}

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
, m_uDataLenght(0)
, m_pImageData(NULL)
, m_uImageDataLenght(0)
, m_nBitsPerComponent(0)
, m_bPremultiplied(false)
, m_pGIF(NULL)
, m_iGIFIndex(0)
, m_bTextImage(false)
{
    s_pImages.insert(this);
}

CAImage::~CAImage()
{
    CCLOGINFO("CrossApp: deallocing CAImage %u.", m_uName);
    CC_SAFE_RELEASE(m_pShaderProgram);
    
    if(m_uName)
    {
        ccGLDeleteTexture(m_uName);
    }

    releaseData();

    s_pImages.erase(this);
}

CAImage*  CAImage::createWithString(const char *text, const CAColor4B& fontColor, const char *fontName, float fontSize, const DSize& dimensions, CATextAlignment hAlignment,
	CAVerticalTextAlignment vAlignment, bool bWordWrap, int iLineSpacing, bool bBold, bool bItalics, bool bUnderLine, bool bDeleteLine)
{
	return g_AFTFontCache.initWithString(text, fontColor, fontName, fontSize, dimensions.width, dimensions.height, hAlignment, vAlignment, bWordWrap, iLineSpacing, bBold, bItalics, bUnderLine, bDeleteLine);
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

int CAImage::cutStringByDSize(std::string& text, const DSize& lableSize, const char* pFontName, unsigned long nSize, bool bWordWrap, int iLineSpacing, bool bBold, bool bItalics)
{
	return g_AFTFontCache.cutStringByDSize(text, lableSize, pFontName, nSize, iLineSpacing, bBold, bItalics);
}

int CAImage::getStringHeight(const char* pFontName, unsigned long nSize, const std::string& pText, int iLimitWidth, int iLineSpace, bool bWordWrap)
{
    return g_AFTFontCache.getStringHeight(pFontName, nSize, pText, iLimitWidth, iLineSpace, bWordWrap);
}

CAImage* CAImage::create(const std::string& file)
{
    return CAImageCache::sharedImageCache()->addImage(file);
}

unsigned int CAImage::getGifImageIndex()
{
    return m_iGIFIndex;
}

unsigned int CAImage::getGifImageCounts()
{
    return m_pGIF ? m_pGIF->ImageCount : 0;
}

void CAImage::updateGifImageWithIndex(unsigned int index)
{
    this->setGifImageWithIndex(index);
    this->convertToRawData();
    this->repremultipliedImageData();
}

void CAImage::copyLine(unsigned char* dst, const unsigned char* src, const ColorMapObject* cmap, int transparent, int width)
{
    for (; width > 0; width--, src++, dst+=4)
    {
        if (*src != transparent)
        {
            const GifColorType& col = cmap->Colors[*src];
            *dst     = col.Red;
            *(dst+1) = col.Green;
            *(dst+2) = col.Blue;
            *(dst+3) = 0xFF;
        }
    }
}

void CAImage::setGifImageWithIndex(unsigned int index)
{
    CC_RETURN_IF(m_pGIF == NULL);
    index = MIN(index, m_pGIF->ImageCount-1);
    m_iGIFIndex = index;
    
    CAColor4B bgColor;
    if (m_pGIF->SColorMap != NULL)
    {
        const GifColorType& col = m_pGIF->SColorMap->Colors[m_pGIF->SBackGroundColor];
        bgColor = ccc4(col.Red, col.Green, col.Blue, 0xFF);
    }

    static CAColor4B paintingColor;
    const SavedImage* cur = &m_pGIF->SavedImages[index];
    
    if (index == 0)
    {
        bool trans;
        int disposal;
        this->getTransparencyAndDisposalMethod(cur, &trans, &disposal);
        if (!trans && m_pGIF->SColorMap != NULL)
        {
            paintingColor = ccc4(0, 0, 0, 0);
        }
    }
    else
    {
        const SavedImage* prev = &m_pGIF->SavedImages[index-1];
        
        bool curTrans;
        int curDisposal;
        this->getTransparencyAndDisposalMethod(prev, &curTrans, &curDisposal);
        bool nextTrans;
        int nextDisposal;
        this->getTransparencyAndDisposalMethod(cur, &nextTrans, &nextDisposal);
        
        if (nextTrans || !checkIfCover(cur, prev))
        {
            if (curDisposal == 2)
            {
                unsigned char* dst = &m_pImageData[(prev->ImageDesc.Top * m_uPixelsWide + prev->ImageDesc.Left) * 4];
                GifWord copyWidth = prev->ImageDesc.Width;
                
                if (prev->ImageDesc.Left + copyWidth > m_uPixelsWide)
                {
                    copyWidth = m_uPixelsWide - prev->ImageDesc.Left;
                }
                
                GifWord copyHeight = prev->ImageDesc.Height;
                if (prev->ImageDesc.Top + copyHeight > m_uPixelsHigh)
                {
                    copyHeight = m_uPixelsHigh - prev->ImageDesc.Top;
                }
                
                for (; copyHeight > 0; copyHeight--)
                {
                    for(int wIndex = 0; wIndex < m_uPixelsWide; wIndex++, dst+=4)
                    {
                        *dst     = paintingColor.r;
                        *(dst+1) = paintingColor.g;
                        *(dst+2) = paintingColor.b;
                        *(dst+3) = paintingColor.a;
                    }
                }
            }
            else if (curDisposal == 3)
            {
                //bm->swap(backup);
            }
        }
    }
    
    if (index == m_pGIF->ImageCount-1 || !this->checkIfWillBeCleared(cur))
    {
        int transparent = -1;
        for (int i = 0; i < cur->ExtensionBlockCount; ++i)
        {
            ExtensionBlock* eb = cur->ExtensionBlocks + i;
            if (eb->Function == GRAPHICS_EXT_FUNC_CODE &&
                eb->ByteCount == 4)
            {
                bool has_transparency = ((eb->Bytes[0] & 1) == 1);
                if (has_transparency)
                {
                    transparent = (unsigned char)eb->Bytes[3];
                }
            }
        }
        
        if (cur->ImageDesc.ColorMap != NULL)
        {
            m_pGIF->SColorMap = cur->ImageDesc.ColorMap;
        }
 
        if (m_pGIF->SColorMap && m_pGIF->SColorMap->ColorCount == (1 << m_pGIF->SColorMap->BitsPerPixel))
        {
            unsigned char* src = (unsigned char*)cur->RasterBits;
            unsigned char* dst = &m_pImageData[(cur->ImageDesc.Top * m_uPixelsWide + cur->ImageDesc.Left) * 4];
            
            GifWord copyWidth = cur->ImageDesc.Width;
            if (cur->ImageDesc.Left + copyWidth > m_uPixelsWide)
            {
                copyWidth = m_uPixelsWide - cur->ImageDesc.Left;
            }
            
            GifWord copyHeight = cur->ImageDesc.Height;
            if (cur->ImageDesc.Top + copyHeight > m_uPixelsHigh)
            {
                copyHeight = m_uPixelsHigh - cur->ImageDesc.Top;
            }
            
            for (; copyHeight > 0; copyHeight--)
            {
                copyLine(dst, src, m_pGIF->SColorMap, transparent, copyWidth);
                src += cur->ImageDesc.Width;
                dst += m_uPixelsWide*4;
            }
        }
        
    }
}

void CAImage::getTransparencyAndDisposalMethod(const SavedImage* frame, bool* trans, int* disposal)
{
    *trans = false;
    *disposal = 0;
    for (int i = 0; i < frame->ExtensionBlockCount; ++i)
    {
        ExtensionBlock* eb = frame->ExtensionBlocks + i;
        if (eb->Function == GRAPHICS_EXT_FUNC_CODE &&
            eb->ByteCount == 4)
        {
            *trans = ((eb->Bytes[0] & 1) == 1);
            *disposal = ((eb->Bytes[0] >> 2) & 7);
        }
    }
}

bool CAImage::checkIfCover(const SavedImage* target, const SavedImage* covered)
{
    if (target->ImageDesc.Left <= covered->ImageDesc.Left
        && covered->ImageDesc.Left + covered->ImageDesc.Width <=
        target->ImageDesc.Left + target->ImageDesc.Width
        && target->ImageDesc.Top <= covered->ImageDesc.Top
        && covered->ImageDesc.Top + covered->ImageDesc.Height <=
        target->ImageDesc.Top + target->ImageDesc.Height) {
        return true;
    }
    return false;
}


bool CAImage::checkIfWillBeCleared(const SavedImage* frame)
{
    for (int i = 0; i < frame->ExtensionBlockCount; ++i)
    {
        ExtensionBlock* eb = frame->ExtensionBlocks + i;
        if (eb->Function == GRAPHICS_EXT_FUNC_CODE &&
            eb->ByteCount == 4)
        {
            // check disposal method
            int disposal = ((eb->Bytes[0] >> 2) & 7);
            if (disposal == 2 || disposal == 3)
            {
                return true;
            }
        }
    }
    return false;
}

CAImage* CAImage::createWithImageDataNoCache(const unsigned char * data, unsigned long lenght)
{
    CAImage* image = new CAImage();
    if (image && image->initWithImageData(data, lenght))
    {
        image->autorelease();
        return image;
    }
    CC_SAFE_DELETE(image);
    return NULL;
}

CAImage* CAImage::createWithImageData(const unsigned char * data, unsigned long lenght, const std::string& key)
{
    CAImage* image = CAImage::createWithImageDataNoCache(data, lenght);
    if (image)
    {
        CAImageCache::sharedImageCache()->setImageForKey(image, key.c_str());
    }
    return image;
}

CAImage* CAImage::createWithRawDataNoCache(const unsigned char * data,
                                           const CAImage::PixelFormat& pixelFormat,
                                           unsigned int pixelsWide,
                                           unsigned int pixelsHigh)
{
    CAImage* image = new CAImage();
    if (image && image->initWithRawData(data, pixelFormat, pixelsWide, pixelsHigh))
    {
        image->autorelease();
        return image;
    }
    CC_SAFE_DELETE(image);
    return NULL;
}

CAImage* CAImage::createWithRawData(const unsigned char * data,
                                    const CAImage::PixelFormat& pixelFormat,
                                    unsigned int pixelsWide,
                                    unsigned int pixelsHigh,
                                    const std::string& key)
{
    CAImage* image = CAImage::createWithRawDataNoCache(data, pixelFormat, pixelsWide, pixelsHigh);
    if (image)
    {
        CAImageCache::sharedImageCache()->setImageForKey(image, key.c_str());
    }
    return image;
}

bool CAImage::initWithImageFile(const std::string& file)
{
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(file.c_str());
    unsigned long pSize = 0;
    unsigned char* data = CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str(), "rb", &pSize);
    bool bRet = initWithImageData(data, pSize);
	delete[]data;
	return bRet;
}

bool CAImage::initWithImageFileThreadSafe(const std::string& fullPath)
{
    unsigned long pSize = 0;
    unsigned char* data = CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str(), "rb", &pSize);
    bool ret = false;
    
    do
    {
        CC_BREAK_IF(! data || pSize <= 0);
        
        unsigned char* unpackedData = NULL;
        unsigned long unpackedLen = 0;
        
        unpackedData = const_cast<unsigned char*>(data);
        unpackedLen = pSize;
        
        Format type = this->detectFormat(unpackedData, unpackedLen);
        
        switch (type)
        {
            case JPG:
                ret = this->initWithJpgData(unpackedData, unpackedLen);
                break;
            case PNG:
                ret = this->initWithPngData(unpackedData, unpackedLen);
                break;
            case GIF:
                ret = this->initWithGifData(unpackedData, unpackedLen);
                break;
            case TIFF:
                ret = this->initWithTiffData(unpackedData, unpackedLen);
                break;
            case WEBP:
                ret = this->initWithWebpData(unpackedData, unpackedLen);
                break;
            case ETC:
                ret = this->initWithETCData(unpackedData, unpackedLen);
                break;
            default:
            {
                ret = false;
                break;
            }
        }
        
        if (ret)
        {
            this->convertToRawData();
        }
        
        if(unpackedData != data)
        {
            free(unpackedData);
        }
        
    } while (0);
    
    return ret;
}

bool CAImage::initWithImageData(const unsigned char * data, unsigned long dataLen)
{
    bool ret = false;
    
    do
    {
        CC_BREAK_IF(! data || dataLen <= 0);
        
		unsigned char* unpackedData = const_cast<unsigned char*>(data);
		unsigned long unpackedLen = dataLen;
        
        Format type = this->detectFormat(unpackedData, unpackedLen);
        
        switch (type)
        {
            case JPG:
                ret = this->initWithJpgData(unpackedData, unpackedLen);
                break;
            case PNG:
                ret = this->initWithPngData(unpackedData, unpackedLen);
                break;
            case GIF:
                ret = this->initWithGifData(unpackedData, unpackedLen);
                break;
            case TIFF:
                ret = this->initWithTiffData(unpackedData, unpackedLen);
                break;
            case WEBP:
                ret = this->initWithWebpData(unpackedData, unpackedLen);
                break;
            case ETC:
                ret = this->initWithETCData(unpackedData, unpackedLen);
                break;
            default:
            {
                ret = false;
                break;
            }
        }
        if (ret)
        {
            this->convertToRawData();
            this->premultipliedImageData();
        }
    }
    while (0);
    return ret;
}

bool CAImage::initWithJpgData(const unsigned char *  data, unsigned long dataLen)
{
    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    /* We use our private extension JPEG error handler.
     * Note that this struct must live as long as the main JPEG parameter
     * struct, to avoid dangling-pointer problems.
     */
    struct MyErrorMgr jerr;
    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1] = {0};
    unsigned long location = 0;
    unsigned int i = 0;
    
    bool bRet = false;
    do
    {
        /* We set up the normal JPEG error routines, then override error_exit. */
        cinfo.err = jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = myErrorExit;
        /* Establish the setjmp return context for MyErrorExit to use. */
        if (setjmp(jerr.setjmp_buffer)) {
            /* If we get here, the JPEG code has signaled an error.
             * We need to clean up the JPEG object, close the input file, and return.
             */
            jpeg_destroy_decompress(&cinfo);
            break;
        }
        
        /* setup decompression process and source, then read JPEG header */
        jpeg_create_decompress( &cinfo );
        
#ifndef CC_TARGET_QT5
        jpeg_mem_src( &cinfo, const_cast<unsigned char*>(data), dataLen );
#endif /* CC_TARGET_QT5 */
        
        /* reading the image header which contains image information */
#if (JPEG_LIB_VERSION >= 90)
        // libjpeg 0.9 adds stricter types.
        jpeg_read_header( &cinfo, TRUE );
#else
        jpeg_read_header( &cinfo, true );
#endif
        
        // we only support RGB or grayscale
        if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
        {
            m_ePixelFormat = CAImage::PixelFormat_I8;
        }else
        {
            cinfo.out_color_space = JCS_RGB;
            m_ePixelFormat = CAImage::PixelFormat_RGB888;
        }
        
        /* Start decompression jpeg here */
        jpeg_start_decompress( &cinfo );
        
        /* init image info */
        m_uPixelsWide  = cinfo.output_width;
        m_uPixelsHigh = cinfo.output_height;
        m_bHasAlpha = false;
        m_bHasPremultipliedAlpha = false;
        m_nBitsPerComponent = 8;
        
        row_pointer[0] = static_cast<unsigned char*>(malloc(cinfo.output_width*cinfo.output_components * sizeof(unsigned char)));
        CC_BREAK_IF(! row_pointer[0]);
        
        m_uImageDataLenght = cinfo.output_width*cinfo.output_height*cinfo.output_components;
        m_pImageData = static_cast<unsigned char*>(malloc(m_uImageDataLenght * sizeof(unsigned char)));
        CC_BREAK_IF(! m_pImageData);
        
        /* now actually read the jpeg into the raw buffer */
        /* read one scan line at a time */
        while( cinfo.output_scanline < cinfo.output_height )
        {
            jpeg_read_scanlines( &cinfo, row_pointer, 1 );
            for( i=0; i<cinfo.output_width*cinfo.output_components;i++)
            {
                m_pImageData[location++] = row_pointer[0][i];
            }
        }
        
        /* When read image file with broken data, jpeg_finish_decompress() may cause error.
         * Besides, jpeg_destroy_decompress() shall deallocate and release all memory associated
         * with the decompression object.
         * So it doesn't need to call jpeg_finish_decompress().
         */
        //jpeg_finish_decompress( &cinfo );
        jpeg_destroy_decompress( &cinfo );
        /* wrap up decompression, destroy objects, free pointers and close open files */
        bRet = true;
    } while (0);
    
    if (row_pointer[0] != NULL)
    {
        free(row_pointer[0]);
    };
    return bRet;
}

bool CAImage::initWithPngData(const unsigned char * data, unsigned long dataLen)
{
    // length of bytes to check if it is a valid png file
#define PNGSIGSIZE  8
    bool bRet = false;
    png_byte        header[PNGSIGSIZE]   = {0};
    png_structp     png_ptr     =   0;
    png_infop       info_ptr    = 0;
    
    do
    {
        // png header len is 8 bytes
        CC_BREAK_IF(dataLen < PNGSIGSIZE);
        
        // check the data is png or not
        memcpy(header, data, PNGSIGSIZE);
        CC_BREAK_IF(png_sig_cmp(header, 0, PNGSIGSIZE));
        
        // init png_struct
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
        CC_BREAK_IF(! png_ptr);
        
        // init png_info
        info_ptr = png_create_info_struct(png_ptr);
        CC_BREAK_IF(!info_ptr);
        
#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA && CC_TARGET_PLATFORM != CC_PLATFORM_NACL)
        CC_BREAK_IF(setjmp(png_jmpbuf(png_ptr)));
#endif
        
        // set the read call back function
        tImageSource imageSource;
        imageSource.data    = (unsigned char*)data;
        imageSource.size    = dataLen;
        imageSource.offset  = 0;
        png_set_read_fn(png_ptr, &imageSource, pngReadCallback);
        
        // read png header info
        
        // read png file info
        png_read_info(png_ptr, info_ptr);
        
        m_uPixelsWide = png_get_image_width(png_ptr, info_ptr);
        m_uPixelsHigh = png_get_image_height(png_ptr, info_ptr);
        png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);
        
        //CCLOG("color type %u", color_type);
        
        // force palette images to be expanded to 24-bit RGB
        // it may include alpha channel
        if (color_type == PNG_COLOR_TYPE_PALETTE)
        {
            png_set_palette_to_rgb(png_ptr);
        }
        // low-bit-depth grayscale images are to be expanded to 8 bits
        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        {
            bit_depth = 8;
            png_set_expand_gray_1_2_4_to_8(png_ptr);
        }
        // expand any tRNS chunk data into a full alpha channel
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        {
            png_set_tRNS_to_alpha(png_ptr);
        }
        // reduce images with 16-bit samples to 8 bits
        if (bit_depth == 16)
        {
            png_set_strip_16(png_ptr);
        }
        if (bit_depth < 8)
        {
            png_set_packing(png_ptr);
        }
        
        // update info
        png_read_update_info(png_ptr, info_ptr);
        bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        color_type = png_get_color_type(png_ptr, info_ptr);
        
        switch (color_type)
        {
            case PNG_COLOR_TYPE_GRAY:
                m_ePixelFormat = CAImage::PixelFormat_I8;
                break;
            case PNG_COLOR_TYPE_GRAY_ALPHA:
                m_ePixelFormat = CAImage::PixelFormat_AI88;
                break;
            case PNG_COLOR_TYPE_RGB:
                m_ePixelFormat = CAImage::PixelFormat_RGB888;
                break;
            case PNG_COLOR_TYPE_RGB_ALPHA:
                m_ePixelFormat = CAImage::PixelFormat_RGBA8888;
                break;
            default:
                break;
        }
        
        // read png data
        m_nBitsPerComponent = bit_depth;
        png_size_t rowbytes;
        png_bytep* row_pointers = (png_bytep*)malloc( sizeof(png_bytep) * m_uPixelsHigh);
        
        rowbytes = png_get_rowbytes(png_ptr, info_ptr);
        
        m_uImageDataLenght = rowbytes * m_uPixelsHigh;
        m_pImageData = static_cast<unsigned char*>(malloc(m_uImageDataLenght * sizeof(unsigned char)));
        if(!m_pImageData)
        {
            if (row_pointers != NULL)
            {
                free(row_pointers);
            }
            break;
        }
        
        for (unsigned short i = 0; i < m_uPixelsHigh; ++i)
        {
            row_pointers[i] = m_pImageData + i*rowbytes;
        }
        png_read_image(png_ptr, row_pointers);
        
        png_read_end(png_ptr, NULL);
        
        png_uint_32 channel = (png_uint_32)rowbytes/m_uPixelsWide;
        
        // premultiplied alpha for RGBA8888
        if (channel == 4)
        {
            m_bHasAlpha = true;
            unsigned int* fourBytes = (unsigned int*)m_pImageData;
            for(unsigned int i = 0; i < m_uPixelsWide * m_uPixelsHigh; i++)
            {
                unsigned char* p = m_pImageData + i * 4;
                fourBytes[i] = (unsigned)(((unsigned)((unsigned char)(p[0]) * ((unsigned char)(p[3]) + 1)) >> 8) |
                                          ((unsigned)((unsigned char)(p[1]) * ((unsigned char)(p[3]) + 1) >> 8) << 8) |
                                          ((unsigned)((unsigned char)(p[2]) * ((unsigned char)(p[3]) + 1) >> 8) << 16) |
                                          ((unsigned)(unsigned char)(p[3]) << 24));
            }
            
            m_bHasPremultipliedAlpha = true;
        }
        else
        {
            m_bHasAlpha = false;
            m_bHasPremultipliedAlpha = false;
        }
        
        if (row_pointers != NULL)
        {
            free(row_pointers);
        }
        
        bRet = true;
    } while (0);
    
    if (png_ptr)
    {
        png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
    }
    return bRet;
}

bool CAImage::initWithGifData(const unsigned char * data, unsigned long dataLen)
{
    s_pData = data;
    s_pDataMark = 0;
    int error = 0;
    m_pGIF = DGifOpen(NULL,&DecodeCallBackProc,&error);
    if (NULL == m_pGIF || DGifSlurp(m_pGIF) != GIF_OK)
    {
        DGifCloseFile(m_pGIF);
        m_pGIF = NULL;
        return false;
    }
    
    m_ePixelFormat = CAImage::PixelFormat_RGBA8888;
    m_uPixelsWide = m_pGIF->SWidth;
    m_uPixelsHigh = m_pGIF->SHeight;
    m_uImageDataLenght = m_uPixelsWide * m_uPixelsHigh * 4;
    m_pImageData = (unsigned char*)malloc(sizeof(unsigned char) * m_uImageDataLenght);
    for (unsigned int i = 0; i < m_uPixelsWide * m_uPixelsHigh; i++)
    {
        *(m_pImageData + i * 4)     = '\0';
        *(m_pImageData + i * 4 + 1) = '\0';
        *(m_pImageData + i * 4 + 2) = '\0';
        *(m_pImageData + i * 4 + 3) = '\0';
    }
    
    m_bHasPremultipliedAlpha = false;
    
    this->setGifImageWithIndex(0);
    
    return true;
}




bool CAImage::initWithTiffData(const unsigned char * data, unsigned long dataLen)
{
    bool bRet = false;
    do
    {
        // set the read call back function
        tImageSource imageSource;
        imageSource.data    = data;
        imageSource.size    = dataLen;
        imageSource.offset  = 0;

        tiff* tif = TIFFClientOpen("file.tif", "rb", (thandle_t)&imageSource, tiffReadProc,
                                   tiffWriteProc, tiffSeekProc,
                                   tiffCloseProc, tiffSizeProc,
                                   tiffMapProc, tiffUnmapProc);
        
        CC_BREAK_IF(NULL == tif);
        
        uint32 w = 0, h = 0;
        uint16 bitsPerSample = 0, samplePerPixel = 0, planarConfig = 0;
        size_t npixels = 0;
        
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitsPerSample);
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplePerPixel);
        TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarConfig);
        
        npixels = w * h;
        
        m_ePixelFormat = CAImage::PixelFormat_RGBA8888;
        m_uPixelsWide = w;
        m_uPixelsHigh = h;
        m_bHasAlpha = true;
        m_nBitsPerComponent = 8;
        
        m_uImageDataLenght = npixels * sizeof (uint32);
        m_pImageData = static_cast<unsigned char*>(malloc(m_uImageDataLenght * sizeof(unsigned char)));
        
        uint32* raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
        if (raster != NULL)
        {
            if (TIFFReadRGBAImageOriented(tif, w, h, raster, ORIENTATION_TOPLEFT, 0))
            {
                /* the raster data is pre-multiplied by the alpha component
                 after invoking TIFFReadRGBAImageOriented*/
                m_bHasPremultipliedAlpha = true;
                
                memcpy(m_pImageData, raster, npixels*sizeof (uint32));
            }
            
            _TIFFfree(raster);
        }
        
        
        TIFFClose(tif);
        
        bRet = true;
    } while (0);
    return bRet;
}

bool CAImage::initWithWebpData(const unsigned char * data, unsigned long dataLen)
{
    bool bRet = false;
    do
    {
        WebPDecoderConfig config;
        if (WebPInitDecoderConfig(&config) == 0) break;
        if (WebPGetFeatures(static_cast<const uint8_t*>(data), dataLen, &config.input) != VP8_STATUS_OK) break;
        if (config.input.width == 0 || config.input.height == 0) break;
        
        config.output.colorspace = MODE_RGBA;
        m_ePixelFormat = CAImage::PixelFormat_RGBA8888;
        m_uPixelsWide    = config.input.width;
        m_uPixelsHigh   = config.input.height;
        m_bHasAlpha = true;
        m_nBitsPerComponent = 8;
        
        m_uImageDataLenght = m_uPixelsWide * m_uPixelsHigh * 4;
        m_pImageData = static_cast<unsigned char*>(malloc(m_uImageDataLenght * sizeof(unsigned char)));
        
        config.output.u.RGBA.rgba = static_cast<uint8_t*>(m_pImageData);
        config.output.u.RGBA.stride = m_uPixelsWide * 4;
        config.output.u.RGBA.size = m_uImageDataLenght;
        config.output.is_external_memory = 1;
        
        if (WebPDecode(static_cast<const uint8_t*>(data), dataLen, &config) != VP8_STATUS_OK)
        {
            releaseData(&m_pImageData);
            break;
        }
        
        bRet = true;
    } while (0);
    return bRet;
}

bool CAImage::initWithETCData(const unsigned char * data, unsigned long dataLen)
{

    return false;
}


bool CAImage::initWithRawData(const unsigned char * data,
                              const CAImage::PixelFormat& pixelFormat,
                              unsigned int pixelsWide,
                              unsigned int pixelsHigh)
{
    
    unsigned int bitsPerPixel;
    //Hack: bitsPerPixelForFormat returns wrong number for RGB_888 textures. See function.
    if(pixelFormat == PixelFormat_RGB888)
    {
        bitsPerPixel = 24;
    }
    else
    {
        bitsPerPixel = bitsPerPixelForFormat(pixelFormat);
    }
    
    int bytesPerComponent = bitsPerPixel / 8;
    unsigned long nDataLenght = (unsigned long)pixelsWide * pixelsHigh * bytesPerComponent;
    
    this->setData(data, nDataLenght);
    
    m_tContentSize = DSize(pixelsWide, pixelsHigh);
    m_uPixelsWide = pixelsWide;
    m_uPixelsHigh = pixelsHigh;
    m_ePixelFormat = pixelFormat;
    m_fMaxS = 1;
    m_fMaxT = 1;
    
    
    m_bHasMipmaps = false;
    
    switch (pixelFormat)
    {
        case PixelFormat_RGBA8888:
        case PixelFormat_RGBA4444:
        case PixelFormat_RGB5A1:
        case PixelFormat_AI88:
        {
            m_bHasAlpha = true;
            m_bHasPremultipliedAlpha = true;
        }
            break;
        case PixelFormat_RGB888:
        case PixelFormat_RGB565:
        case PixelFormat_I8:
        case PixelFormat_A8:
        default:
        {
            m_bHasAlpha = false;
            m_bHasPremultipliedAlpha = false;
        }
            break;
    }

    this->repremultipliedImageData();
    
    return true;
}

void CAImage::setData(const unsigned char* data, unsigned long dataLenght)
{
    m_uDataLenght = dataLenght;
    unsigned char * tmpData = static_cast<unsigned char*>(malloc(m_uDataLenght * sizeof(unsigned char)));
    memcpy(tmpData, data, m_uDataLenght);
    
    releaseData(&m_pData);
    m_pData = tmpData;
}

void CAImage::convertToRawData()
{
    unsigned char* data = NULL;
    unsigned long lenght = 0;
    convertDataToFormat(m_pImageData, m_uImageDataLenght, m_ePixelFormat, m_ePixelFormat, &data, &lenght);
    this->setData(data, lenght);
    releaseData(&m_pImageData);
    m_uImageDataLenght = 0;

    m_tContentSize = DSize(m_uPixelsWide, m_uPixelsHigh);
    m_fMaxS = 1;
    m_fMaxT = 1;
    
    m_bHasMipmaps = false;
    
    setShaderProgram(CAShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));
}

void CAImage::premultipliedImageData()
{
    CC_RETURN_IF(m_bPremultiplied);
    m_bPremultiplied = true;
    
    unsigned int bitsPerPixel;
    if(m_ePixelFormat == PixelFormat_RGB888)
    {
        bitsPerPixel = 24;
    }
    else
    {
        bitsPerPixel = bitsPerPixelForFormat(m_ePixelFormat);
    }
    
    unsigned int bytesPerRow = m_uPixelsWide * bitsPerPixel / 8;
    
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
    
    this->freeName();
    
    glGenTextures(1, &m_uName);
    ccGLBindTexture2D(m_uName);
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    
    // Specify OpenGL texture image
    
    switch(m_ePixelFormat)
    {
        case PixelFormat_RGBA8888:
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGBA,
                         (GLsizei)m_uPixelsWide,
                         (GLsizei)m_uPixelsHigh,
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         (const GLvoid *)m_pData);
            break;
        case PixelFormat_RGB888:
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGB,
                         (GLsizei)m_uPixelsWide,
                         (GLsizei)m_uPixelsHigh,
                         0,
                         GL_RGB,
                         GL_UNSIGNED_BYTE,
                         (const GLvoid *)m_pData);
            break;
        case PixelFormat_RGBA4444:
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGBA,
                         (GLsizei)m_uPixelsWide,
                         (GLsizei)m_uPixelsHigh,
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_SHORT_4_4_4_4,
                         (const GLvoid *)m_pData);
            break;
        case CAImage::PixelFormat_RGB5A1:
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGBA,
                         (GLsizei)m_uPixelsWide,
                         (GLsizei)m_uPixelsHigh,
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_SHORT_5_5_5_1,
                         (const GLvoid *)m_pData);
            break;
        case PixelFormat_RGB565:
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGB,
                         (GLsizei)m_uPixelsWide,
                         (GLsizei)m_uPixelsHigh,
                         0,
                         GL_RGB,
                         GL_UNSIGNED_SHORT_5_6_5,
                         (const GLvoid *)m_pData);
            break;
        case PixelFormat_AI88:
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_LUMINANCE_ALPHA,
                         (GLsizei)m_uPixelsWide,
                         (GLsizei)m_uPixelsHigh,
                         0,
                         GL_LUMINANCE_ALPHA,
                         GL_UNSIGNED_BYTE,
                         (const GLvoid *)m_pData);
            break;
        case PixelFormat_A8:
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_ALPHA,
                         (GLsizei)m_uPixelsWide,
                         (GLsizei)m_uPixelsHigh,
                         0,
                         GL_ALPHA,
                         GL_UNSIGNED_BYTE,
                         (const GLvoid *)m_pData);
            break;
        case PixelFormat_I8:
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_LUMINANCE,
                         (GLsizei)m_uPixelsWide,
                         (GLsizei)m_uPixelsHigh,
                         0,
                         GL_LUMINANCE,
                         GL_UNSIGNED_BYTE,
                         (const GLvoid *)m_pData);
            break;
        default:
            CCAssert(0, "NSInternalInconsistencyException");
            
    }
    
    setShaderProgram(CAShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));
}

void CAImage::freeName()
{
    if(m_uName)
    {
        ccGLDeleteTexture(m_uName);
        m_uName = 0;
    }
}

void CAImage::repremultipliedImageData()
{
    m_bPremultiplied = false;
    this->premultipliedImageData();
}

const CAImage::PixelFormat& CAImage::getPixelFormat()
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

const DSize& CAImage::getContentSize()
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

void CAImage::releaseData()
{
    if (m_pData)
    {
        releaseData(&m_pData);
        m_uDataLenght = 0;
    }
}

void CAImage::releaseData(unsigned char ** data)
{
    if (*data)
    {
        free(*data);
        *data = NULL;
    }
}

bool CAImage::hasPremultipliedAlpha()
{
    return m_bHasPremultipliedAlpha;
}

const char* CAImage::description(void)
{
    return crossapp_format_string("<CAImage | Name = %d | Dimensions = %u x %u | Coordinates = (%f, %f)>",m_uName, m_uPixelsWide, m_uPixelsHigh, m_fMaxS, m_fMaxT).c_str();
}

void CAImage::drawAtPoint(const DPoint& point)
{
    GLfloat    coordinates[] =
    {
        0.0f,       m_fMaxT,
        m_fMaxS,    m_fMaxT,
        0.0f,       0.0f,
        m_fMaxS,    0.0f
    };
    
    GLfloat    vertices[] =
    {
        point.x,                          point.y,                           /*0.0f,*/
        point.x + m_tContentSize.width,   point.y,                           /*0.0f,*/
        point.x,                          point.y + m_tContentSize.height,   /*0.0f,*/
        point.x + m_tContentSize.width,   point.y + m_tContentSize.height,   /*0.0f*/
    };
    
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

void CAImage::drawInRect(const DRect& rect)
{
    GLfloat    coordinates[] =
    {
        0.0f,       m_fMaxT,
        m_fMaxS,    m_fMaxT,
        0.0f,       0.0f,
        m_fMaxS,    0.0f
    };
    
    GLfloat    vertices[] =
    {
        rect.origin.x,                     rect.origin.y,                      /*0.0f,*/
        rect.origin.x + rect.size.width,   rect.origin.y,                      /*0.0f,*/
        rect.origin.x,                     rect.origin.y + rect.size.height,   /*0.0f,*/
        rect.origin.x + rect.size.width,   rect.origin.y + rect.size.height,   /*0.0f*/
    };
    
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
}

const char* CAImage::stringForFormat()
{
    switch (m_ePixelFormat)
    {
        case PixelFormat_RGBA8888:
            return  "RGBA8888";
            
        case PixelFormat_RGB888:
            return  "RGB888";
            
        case PixelFormat_RGB565:
            return  "RGB565";
            
        case PixelFormat_RGBA4444:
            return  "RGBA4444";
            
        case PixelFormat_RGB5A1:
            return  "RGB5A1";
            
        case PixelFormat_AI88:
            return  "AI88";
            
        case PixelFormat_A8:
            return  "A8";
            
        case PixelFormat_I8:
            return  "I8";
            
        default:
            CCAssert(false , "unrecognized pixel format");
            CCLOG("stringForFormat: %ld, cannot give useful result", (long)m_ePixelFormat);
            break;
    }
    
    return  NULL;
}

unsigned int CAImage::bitsPerPixelForFormat(CAImage::PixelFormat format)
{
    unsigned int ret = 0;
    
    switch (format) {
        case PixelFormat_RGBA8888:
            ret = 32;
            break;
        case PixelFormat_RGB888:
            // It is 32 and not 24, since its internal representation uses 32 bits.
            ret = 32;
            break;
        case PixelFormat_RGB565:
            ret = 16;
            break;
        case PixelFormat_RGBA4444:
            ret = 16;
            break;
        case PixelFormat_RGB5A1:
            ret = 16;
            break;
        case PixelFormat_AI88:
            ret = 16;
            break;
        case PixelFormat_A8:
            ret = 8;
            break;
        case PixelFormat_I8:
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


bool CAImage::saveImageToPNG(const std::string& filePath, bool isToRGB)
{
    bool bRet = false;
    
    do
    {
        FILE *fp;
        png_structp png_ptr;
        png_infop info_ptr;
        png_colorp palette;
        png_bytep *row_pointers;
        
        fp = fopen(filePath.c_str(), "wb");
        CC_BREAK_IF(NULL == fp);
        
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        
        if (NULL == png_ptr)
        {
            fclose(fp);
            break;
        }
        
        info_ptr = png_create_info_struct(png_ptr);
        if (NULL == info_ptr)
        {
            fclose(fp);
            png_destroy_write_struct(&png_ptr, NULL);
            break;
        }
#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA && CC_TARGET_PLATFORM != CC_PLATFORM_NACL)
        if (setjmp(png_jmpbuf(png_ptr)))
        {
            fclose(fp);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            break;
        }
#endif
        png_init_io(png_ptr, fp);
        
        if (!isToRGB && hasAlpha())
        {
            png_set_IHDR(png_ptr, info_ptr, m_uPixelsWide, m_uPixelsHigh, 8, PNG_COLOR_TYPE_RGB_ALPHA,
                         PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        }
        else
        {
            png_set_IHDR(png_ptr, info_ptr, m_uPixelsWide, m_uPixelsHigh, 8, PNG_COLOR_TYPE_RGB,
                         PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        }
        
        palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof (png_color));
        png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);
        
        png_write_info(png_ptr, info_ptr);
        
        png_set_packing(png_ptr);
        
        row_pointers = (png_bytep *)malloc(m_uPixelsHigh * sizeof(png_bytep));
        if(row_pointers == NULL)
        {
            fclose(fp);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            break;
        }
        
        if (!hasAlpha())
        {
            for (int i = 0; i < (int)m_uPixelsHigh; i++)
            {
                row_pointers[i] = (png_bytep)m_pData + i * m_uPixelsWide * 3;
            }
            
            png_write_image(png_ptr, row_pointers);
            
            free(row_pointers);
            row_pointers = NULL;
        }
        else
        {
            if (isToRGB)
            {
                unsigned char *pTempData = static_cast<unsigned char*>(malloc(m_uPixelsWide * m_uPixelsHigh * 3 * sizeof(unsigned char)));
                if (NULL == pTempData)
                {
                    fclose(fp);
                    png_destroy_write_struct(&png_ptr, &info_ptr);
                    
                    free(row_pointers);
                    row_pointers = NULL;
                    break;
                }
                
                for (int i = 0; i < m_uPixelsHigh; ++i)
                {
                    for (int j = 0; j < m_uPixelsWide; ++j)
                    {
                        pTempData[(i * m_uPixelsWide + j) * 3] = m_pData[(i * m_uPixelsWide + j) * 4];
                        pTempData[(i * m_uPixelsWide + j) * 3 + 1] = m_pData[(i * m_uPixelsWide + j) * 4 + 1];
                        pTempData[(i * m_uPixelsWide + j) * 3 + 2] = m_pData[(i * m_uPixelsWide + j) * 4 + 2];
                    }
                }
                
                for (int i = 0; i < (int)m_uPixelsHigh; i++)
                {
                    row_pointers[i] = (png_bytep)pTempData + i * m_uPixelsWide * 3;
                }
                
                png_write_image(png_ptr, row_pointers);
                
                free(row_pointers);
                row_pointers = NULL;
                
                if (pTempData != NULL)
                {
                    free(pTempData);
                }
            }
            else
            {
                for (int i = 0; i < (int)m_uPixelsHigh; i++)
                {
                    row_pointers[i] = (png_bytep)m_pData + i * m_uPixelsWide * 4;
                }
                
                png_write_image(png_ptr, row_pointers);
                
                free(row_pointers);
                row_pointers = NULL;
            }
        }
        
        png_write_end(png_ptr, info_ptr);
        
        png_free(png_ptr, palette);
        palette = NULL;
        
        png_destroy_write_struct(&png_ptr, &info_ptr);
        
        fclose(fp);
        
        bRet = true;
    } while (0);
    
    return bRet;
}

bool CAImage::saveImageToJPG(const std::string& filePath)
{
    bool bRet = false;
    
    do
    {
        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr jerr;
        FILE * outfile;                 /* target file */
        JSAMPROW row_pointer[1];        /* pointer to JSAMPLE row[s] */
        int     row_stride;          /* physical row width in image buffer */
        
        cinfo.err = jpeg_std_error(&jerr);
        /* Now we can initialize the JPEG compression object. */
        jpeg_create_compress(&cinfo);
        
        CC_BREAK_IF((outfile = fopen(filePath.c_str(), "wb")) == NULL);
        
        jpeg_stdio_dest(&cinfo, outfile);
        
        cinfo.image_width = m_uPixelsWide;    /* image width and height, in pixels */
        cinfo.image_height = m_uPixelsHigh;
        cinfo.input_components = 3;       /* # of color components per pixel */
        cinfo.in_color_space = JCS_RGB;       /* colorspace of input image */
        
        jpeg_set_defaults(&cinfo);
        jpeg_set_quality(&cinfo, 90, TRUE);
        
        jpeg_start_compress(&cinfo, TRUE);
        
        row_stride = m_uPixelsWide * 3; /* JSAMPLEs per row in image_buffer */
        
        if (hasAlpha())
        {
            unsigned char *pTempData = static_cast<unsigned char*>(malloc(m_uPixelsWide * m_uPixelsHigh * 3 * sizeof(unsigned char)));
            if (NULL == pTempData)
            {
                jpeg_finish_compress(&cinfo);
                jpeg_destroy_compress(&cinfo);
                fclose(outfile);
                break;
            }
            
            for (int i = 0; i < m_uPixelsHigh; ++i)
            {
                for (int j = 0; j < m_uPixelsWide; ++j)
                    
                {
                    pTempData[(i * m_uPixelsWide + j) * 3] = m_pData[(i * m_uPixelsWide + j) * 4];
                    pTempData[(i * m_uPixelsWide + j) * 3 + 1] = m_pData[(i * m_uPixelsWide + j) * 4 + 1];
                    pTempData[(i * m_uPixelsWide + j) * 3 + 2] = m_pData[(i * m_uPixelsWide + j) * 4 + 2];
                }
            }
            
            while (cinfo.next_scanline < cinfo.image_height) {
                row_pointer[0] = & pTempData[cinfo.next_scanline * row_stride];
                (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
            }
            
            if (pTempData != NULL)
            {
                free(pTempData);
            }
        }
        else
        {
            while (cinfo.next_scanline < cinfo.image_height) {
                row_pointer[0] = & m_pData[cinfo.next_scanline * row_stride];
                (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
            }
        }
        
        jpeg_finish_compress(&cinfo);
        fclose(outfile);
        jpeg_destroy_compress(&cinfo);
        
        bRet = true;
    } while (0);
    
    return bRet;
}

bool CAImage::saveToFile(const std::string& fullPath, bool bIsToRGB)
{
    if (m_ePixelFormat != CAImage::PixelFormat_RGBA8888
        && m_ePixelFormat != CAImage::PixelFormat_RGB888)
    {
        return false;
    }
    
    bool bRet = false;
    
    do
    {
        CC_BREAK_IF(fullPath.size() <= 4);
        
        std::string strLowerCasePath(fullPath);
        for (unsigned int i = 0; i < strLowerCasePath.length(); ++i)
        {
            strLowerCasePath[i] = tolower(fullPath[i]);
        }
        
        if (std::string::npos != strLowerCasePath.find(".jpg")
            || std::string::npos != strLowerCasePath.find(".jpeg"))
        {
            CC_BREAK_IF(!saveImageToJPG(fullPath));
        }
        else
        {
            CC_BREAK_IF(!saveImageToPNG(fullPath, bIsToRGB));
        }
        
        bRet = true;
    } while (0);
    
    return bRet;
}



float CAImage::getAspectRatio()
{
    return m_tContentSize.width / m_tContentSize.height;
}

CAImage* CAImage::copy()
{
    CAImage *newImage = new CAImage();
    newImage->autorelease();
    newImage->m_ePixelFormat = this->m_ePixelFormat;
    newImage->m_uPixelsWide = this->m_uPixelsWide;
    newImage->m_uPixelsHigh = this->m_uPixelsHigh;
    newImage->m_uName = this->m_uName;
    newImage->m_fMaxS = this->m_fMaxS;
    newImage->m_fMaxT = this->m_fMaxT;
    newImage->m_tContentSize = this->m_tContentSize;
    newImage->m_pShaderProgram = this->m_pShaderProgram;
    newImage->m_bMonochrome = this->m_bMonochrome;
    newImage->m_bHasMipmaps = this->m_bHasMipmaps;
    newImage->m_bHasPremultipliedAlpha = this->m_bHasPremultipliedAlpha;
    newImage->m_bHasAlpha = this->m_bHasAlpha;
    newImage->m_nBitsPerComponent = this->m_nBitsPerComponent;
    newImage->m_uDataLenght = this->m_uDataLenght;
    
    newImage->m_pData = static_cast<unsigned char*>(malloc(m_uDataLenght * sizeof(unsigned char)));
    memcpy(newImage->m_pData, this->m_pData, m_uDataLenght);
    
    return newImage;
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
        unsigned char pixels[2][2];
        for (int i=0; i<2; i++)
        {
            for (int j=0; j<2; j++)
            {
                pixels[i][j] = 0xff;
            }
        }
        
        cc_white_image = new CAImage();
        cc_white_image->initWithRawData((const unsigned char *)pixels, CAImage::PixelFormat_A8, 2, 2);
        cc_white_image->m_bMonochrome = true;
    }
    return cc_white_image;
}

CAImage::Format CAImage::detectFormat(const unsigned char * data, unsigned long dataLen)
{
    if (isPng(data, dataLen))
    {
        return CAImage::PNG;
    }
    else if (isJpg(data, dataLen))
    {
        return CAImage::JPG;
    }
    else if (isGif(data, dataLen))
    {
        return CAImage::GIF;
    }
    else if (isTiff(data, dataLen))
    {
        return CAImage::TIFF;
    }
    else if (isWebp(data, dataLen))
    {
        return CAImage::WEBP;
    }
    else if (isEtc(data, dataLen))
    {
        return CAImage::ETC;
    }
    else
    {
        return CAImage::UNKOWN;
    }
}


bool CAImage::isPng(const unsigned char * data, unsigned long dataLen)
{
    if (dataLen <= 8)
    {
        return false;
    }
    
    static const unsigned char PNG_SIGNATURE[] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a};
    
    return memcmp(PNG_SIGNATURE, data, sizeof(PNG_SIGNATURE)) == 0;
}

bool CAImage::isJpg(const unsigned char * data, unsigned long dataLen)
{
    if (dataLen <= 4)
    {
        return false;
    }
    
    static const unsigned char JPG_SOI[] = {0xFF, 0xD8};
    
    return memcmp(data, JPG_SOI, 2) == 0;
}

bool CAImage::isGif(const unsigned char * data, unsigned long dataLen)
{
    if (dataLen <= 6)
    {
        return false;
    }
    
    static const unsigned char JPG_SOI[] = GIF87_STAMP;
    static const unsigned char JPG_SOI2[] = GIF89_STAMP;
    static const unsigned char JPG_SOI3[] = GIF_STAMP;

    return     (memcmp(data, JPG_SOI, 6) == 0)
            || (memcmp(data, JPG_SOI2, 6) == 0)
            || (memcmp(data, JPG_SOI3, 6) == 0);
}

bool CAImage::isEtc(const unsigned char * data, unsigned long dataLen)
{
    //    return etc1_pkm_is_valid((etc1_byte*)data) ? true : false;
    return false;
}

bool CAImage::isTiff(const unsigned char * data, unsigned long dataLen)
{
    if (dataLen <= 4)
    {
        return false;
    }
    
    static const char* TIFF_II = "II";
    static const char* TIFF_MM = "MM";
    
    return (    memcmp(data, TIFF_II, 2) == 0
                &&
                *(static_cast<const unsigned char*>(data) + 2) == 42
                &&
                *(static_cast<const unsigned char*>(data) + 3) == 0)
            || (memcmp(data, TIFF_MM, 2) == 0
                &&
                *(static_cast<const unsigned char*>(data) + 2) == 0
                &&
                *(static_cast<const unsigned char*>(data) + 3) == 42);
}

bool CAImage::isWebp(const unsigned char * data, unsigned long dataLen)
{
    if (dataLen <= 12)
    {
        return false;
    }
    
    static const char* WEBP_RIFF = "RIFF";
    static const char* WEBP_WEBP = "WEBP";
    
    return  memcmp(data, WEBP_RIFF, 4) == 0
            &&
            memcmp(static_cast<const unsigned char*>(data) + 8, WEBP_WEBP, 4) == 0;
}



void CAImage::reloadAllImages()
{
    for (std::set<CAImage*>::iterator itr=s_pImages.begin(); itr!=s_pImages.end(); itr++)
    {
        (*itr)->freeName();
    }
    
    for (std::set<CAImage*>::iterator itr=s_pImages.begin(); itr!=s_pImages.end(); itr++)
    {
        (*itr)->repremultipliedImageData();
    }
}
NS_CC_END
