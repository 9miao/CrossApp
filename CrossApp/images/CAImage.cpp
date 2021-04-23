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
#include "platform/CAFileUtils.h"
#include "platform/CAFontProcesstor.h"
#include "platform/CADensityDpi.h"
#include "basics/CAApplication.h"
#include "renderer/CCGLProgram.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCGLProgramCache.h"
#include "view/CARenderImage.h"
#include <ctype.h>
#include <cctype>
#include <png.h>
#include <jpeglib.h>
#include <tiffio.h>
#include <decode.h>
#include "support/image_support/TGAlib.h"

NS_CC_BEGIN


CAImage* CAImage::scaleToNewImageWithImage(CAImage* image, const DSize& size)
{
    CARenderImage* renderImage = CARenderImage::create((size.width), (size.height));
    
    CAApplication* application = CAApplication::getApplication();
    application->getRenderer()->clean();
    experimental::FrameBuffer::clearAllFBOs();
    
    renderImage->beginWithClear(CAColor4B::CLEAR);

    application->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    application->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, Mat4::IDENTITY);
    
    static CustomCommand _customCommand;
    _customCommand.init(0);
    _customCommand.func = [=]()
    {
        
        GLfloat    coordinates[] =
        {
            0.0f, 0.0f,
            (image->getMaxS()) , 0.0f,
            0.0f, (image->getMaxT()),
            (image->getMaxS()), (image->getMaxT())
        };
        
        GLfloat    vertices[] =
        {
            0, 0,                    /*0.0f,*/
            (size.width), 0,                    /*0.0f,*/
            0, (size.height),          /*0.0f,*/
            (size.width), (size.height)           /*0.0f*/
        };
        
        GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD );
        image->getShaderProgram()->use();
        image->getShaderProgram()->setUniformsForBuiltins();
        
        GL::bindTexture2D(image->getName());
        
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, coordinates);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


    };
    application->getRenderer()->addCommand(&_customCommand);
    
    application->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    
    renderImage->end();
    
    application->getRenderer()->render();
    
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
        
        unsigned char* data = new unsigned char[new_pitch * new_h];
        // Resample.  Simple average 2x2 --> 1, in-place.
        int	pitch = image->getPixelsWide() * 3;
        
        for (int j = 0; j < new_h; j++)
        {
            unsigned char*	out = ((unsigned char*)data) + j * new_pitch;
            unsigned char*	in = ((unsigned char*)image->getData()->getBytes()) + (j << 1) * pitch;
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
        CAData* newData = CAData::create();
        newData->fastSet(data, new_pitch * new_h);

        return CAImage::createWithRawDataNoCache(newData, CAImage::PixelFormat::RGB888, new_w, new_h);
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
        unsigned char* data = new unsigned char[new_pitch * new_h];
        
        // Resample.  Simple average 2x2 --> 1, in-place.
        int	pitch = image->getPixelsWide() * 4;
        for (int j = 0; j < new_h; j++)
        {
            unsigned char*	out = ((unsigned char*)data) + j * new_pitch;
            unsigned char*	in = ((unsigned char*)image->getData()->getBytes()) + (j << 1) * pitch;
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
        CAData* newData = CAData::create();
        newData->fastSet(data, new_pitch * new_h);
        
        return CAImage::createWithRawDataNoCache(newData, CAImage::PixelFormat::RGB888, new_w, new_h);
    }

    return 0;
}


CAImage* CAImage::generateMipmapsWithImage(CAImage* image)
{
    if (DPoint(image->getContentSize()).getLength() < 2895)
    {
        return image;
    }
    
    if (image->getPixelFormat() == CAImage::PixelFormat::RGB888)
    {
        return generateMipmapsWithImageRGB(image);
    }
    else if (image->getPixelFormat() == CAImage::PixelFormat::RGBA8888)
    {
        return generateMipmapsWithImageRGBA(image);
    }
    
    return NULL;
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
extern "C"
{
    FILE *fopen$UNIX2003( const char *filename, const char *mode )
    {
        return fopen(filename, mode);
    }
    size_t fwrite$UNIX2003( const void *a, size_t b, size_t c, FILE *d )
    {
        return fwrite(a, b, c, d);
    }
    
    char* strerror$UNIX2003( int errnum )
    {
        return strerror(errnum);
    }
}
#endif
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

/*
void convertYUVToRGB888(int y, int u, int v, unsigned char* r, unsigned char* g, unsigned char* b)
{
    u = u - 128;
    v = v - 128;
    
    *r    = (unsigned char)(y + v + ((v * 103) >> 8));
    *g  = (unsigned char)(y - ((u * 88) >> 8) - ((v * 183) >> 8));
    *b   = (unsigned char)(y + u +((u * 198) >> 8));
}

//I420是yuv420格式，是3个plane，排列方式为(Y)(U)(V)
void convertI420ToRGB888(const unsigned char* src, unsigned char* outData, unsigned int width, unsigned int height)
{
    unsigned int numOfPixel = width * height;
    unsigned int positionOfV = numOfPixel;
    unsigned int positionOfU = numOfPixel/4 + numOfPixel;
    
    for(int i=0; i<height; i++)
    {
        unsigned int startY = i*width;
        unsigned int step = (i/2)*(width/2);
        unsigned int startU = positionOfV + step;
        unsigned int startV = positionOfU + step;
        for(unsigned int j = 0; j < width; j++)
        {
            unsigned int Y = startY + j;
            unsigned int U = startU + j/2;
            unsigned int V = startV + j/2;
            unsigned int index = Y * 3;
            
            this->convertYUVToRGB888(src[Y], src[U], src[V], &outData[index+0], &outData[index+1], &outData[index+2]);
            
        }
    }
}
*/
CAImage::PixelFormat CAImage::convertI8ToFormat(const unsigned char* data, unsigned long dataLen,  CAImage::PixelFormat format, unsigned char** outData, unsigned long* outDataLen)
{
    switch (format)
    {
        case CAImage::PixelFormat::RGBA8888:
            *outDataLen = dataLen*4;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertI8ToRGBA8888(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::RGB888:
            *outDataLen = dataLen*3;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertI8ToRGB888(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::RGB565:
            *outDataLen = dataLen*2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertI8ToRGB565(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::AI88:
            *outDataLen = dataLen*2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertI8ToAI88(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::RGBA4444:
            *outDataLen = dataLen*2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertI8ToRGBA4444(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::RGB5A1:
            *outDataLen = dataLen*2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertI8ToRGB5A1(data, dataLen, *outData);
            break;
        default:
            // unsupport convertion or don't need to convert
            if (format != CAImage::PixelFormat::DEFAULT && format != CAImage::PixelFormat::I8)
            {
                CCLOG("Can not convert image format CAImage::PixelFormat::I8 to format ID:%d, we will use it's origin format CAImage::PixelFormat::I8", format);
            }
            
            *outData = (unsigned char*)data;
            *outDataLen = dataLen;
            return CAImage::PixelFormat::I8;
    }
    
    return format;
}

CAImage::PixelFormat CAImage::convertAI88ToFormat(const unsigned char* data, unsigned long dataLen,  CAImage::PixelFormat format, unsigned char** outData, unsigned long* outDataLen)
{
    switch (format)
    {
        case CAImage::PixelFormat::RGBA8888:
            *outDataLen = dataLen*2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertAI88ToRGBA8888(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::RGB888:
            *outDataLen = dataLen/2*3;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertAI88ToRGB888(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::RGB565:
            *outDataLen = dataLen;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertAI88ToRGB565(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::A8:
            *outDataLen = dataLen/2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertAI88ToA8(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::I8:
            *outDataLen = dataLen/2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertAI88ToI8(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::RGBA4444:
            *outDataLen = dataLen;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertAI88ToRGBA4444(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::RGB5A1:
            *outDataLen = dataLen;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertAI88ToRGB5A1(data, dataLen, *outData);
            break;
        default:
            // unsupport convertion or don't need to convert
            if (format != CAImage::PixelFormat::DEFAULT && format != CAImage::PixelFormat::AI88)
            {
                CCLOG("Can not convert image format CAImage::PixelFormat::AI88 to format ID:%d, we will use it's origin format CAImage::PixelFormat::AI88", format);
            }
            
            *outData = (unsigned char*)data;
            *outDataLen = dataLen;
            return CAImage::PixelFormat::AI88;
            break;
    }
    
    return format;
}

CAImage::PixelFormat CAImage::convertRGB888ToFormat(const unsigned char* data, unsigned long dataLen, CAImage::PixelFormat format, unsigned char** outData, unsigned long* outDataLen)
{
    switch (format)
    {
        case CAImage::PixelFormat::RGBA8888:
            *outDataLen = dataLen/3*4;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGB888ToRGBA8888(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::RGB565:
            *outDataLen = dataLen/3*2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGB888ToRGB565(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::I8:
            *outDataLen = dataLen/3;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGB888ToI8(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::AI88:
            *outDataLen = dataLen/3*2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGB888ToAI88(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::RGBA4444:
            *outDataLen = dataLen/3*2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGB888ToRGBA4444(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::RGB5A1:
            *outDataLen = dataLen;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGB888ToRGB5A1(data, dataLen, *outData);
            break;
        default:
            // unsupport convertion or don't need to convert
            if (format != CAImage::PixelFormat::DEFAULT && format != CAImage::PixelFormat::RGB888)
            {
                CCLOG("Can not convert image format CAImage::PixelFormat::RGB888 to format ID:%d, we will use it's origin format CAImage::PixelFormat::RGB888", format);
            }
            
            *outData = (unsigned char*)data;
            *outDataLen = dataLen;
            return CAImage::PixelFormat::RGB888;
    }
    return format;
}

CAImage::PixelFormat CAImage::convertRGBA8888ToFormat(const unsigned char* data, unsigned long dataLen, CAImage::PixelFormat format, unsigned char** outData, unsigned long* outDataLen)
{
    
    switch (format)
    {
        case CAImage::PixelFormat::RGB888:
            *outDataLen = dataLen/4*3;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGBA8888ToRGB888(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::RGB565:
            *outDataLen = dataLen/2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGBA8888ToRGB565(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::A8:
            *outDataLen = dataLen/4;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGBA8888ToA8(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::I8:
            *outDataLen = dataLen/4;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGBA8888ToI8(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::AI88:
            *outDataLen = dataLen/2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGBA8888ToAI88(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::RGBA4444:
            *outDataLen = dataLen/2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGBA8888ToRGBA4444(data, dataLen, *outData);
            break;
        case CAImage::PixelFormat::RGB5A1:
            *outDataLen = dataLen/2;
            *outData = (unsigned char*)malloc(sizeof(unsigned char) * (*outDataLen));
            convertRGBA8888ToRGB5A1(data, dataLen, *outData);
            break;
        default:
            // unsupport convertion or don't need to convert
            if (format != CAImage::PixelFormat::DEFAULT && format != CAImage::PixelFormat::RGBA8888)
            {
                CCLOG("Can not convert image format CAImage::PixelFormat::RGBA8888 to format ID:%d, we will use it's origin format CAImage::PixelFormat::RGBA8888", format);
            }
            
            *outData = (unsigned char*)data;
            *outDataLen = dataLen;
            return CAImage::PixelFormat::RGBA8888;
    }
    
    return format;
}

/*
 convert map:
 1.CAImage::PixelFormat::RGBA8888
 2.CAImage::PixelFormat::RGB888
 3.CAImage::PixelFormat::RGB565
 4.CAImage::PixelFormat::A8
 5.CAImage::PixelFormat::I8
 6.CAImage::PixelFormat::AI88
 7.CAImage::PixelFormat::RGBA4444
 8.CAImage::PixelFormat::RGB5A1
 
 gray(5) -> 1235678
 gray alpha(6) -> 12345678
 rgb(2) -> 1235678
 rgba(1) -> 12345678
 
 */
CAImage::PixelFormat CAImage::convertDataToFormat(const unsigned char* data, unsigned long dataLen, CAImage::PixelFormat originFormat, CAImage::PixelFormat format, unsigned char** outData, unsigned long* outDataLen)
{
    // don't need to convert
    if (format == originFormat || format == CAImage::PixelFormat::DEFAULT)
    {
        *outData = (unsigned char*)data;
        *outDataLen = dataLen;
        return originFormat;
    }
    
    switch (originFormat)
    {
        case CAImage::PixelFormat::I8:
            return convertI8ToFormat(data, dataLen, format, outData, outDataLen);
        case CAImage::PixelFormat::AI88:
            return convertAI88ToFormat(data, dataLen, format, outData, outDataLen);
        case CAImage::PixelFormat::RGB888:
            return convertRGB888ToFormat(data, dataLen, format, outData, outDataLen);
        case CAImage::PixelFormat::RGBA8888:
            return convertRGBA8888ToFormat(data, dataLen, format, outData, outDataLen);
        default:
            *outData = (unsigned char*)data;
            *outDataLen = dataLen;
            return originFormat;
    }
}

static std::set<CAImage*> s_pImages;

const std::set<CAImage*>& CAImage::getImagesSet()
{
    return s_pImages;
}

CAImage::CAImage()
: m_uPixelsWide(0)
, m_uPixelsHigh(0)
, m_uName(0)
, m_fMaxS(0.0)
, m_fMaxT(0.0)
, m_bHasPremultipliedAlpha(false)
, m_bHasMipmaps(false)
, m_pShaderProgram(nullptr)
, m_bMonochrome(false)
, m_pData(nullptr)
, m_nBitsPerComponent(0)
, m_bPremultiplied(false)
, m_bTextImage(false)
{
    s_pImages.insert(this);
}

CAImage::~CAImage()
{
    CCLOG("CrossApp: deallocing CAImage %u.", m_uName);
    CC_SAFE_RELEASE(m_pShaderProgram);
    
    this->freeName();

    CC_SAFE_RELEASE_NULL(m_pData);

    s_pImages.erase(this);
}

int CAImage::getFontHeight(const char* pFontName, float nSize)
{
    CAFont font;
    font.fontName = pFontName;
    font.fontSize = nSize;
    return CAFontProcesstor::heightForFont(font);
}

int CAImage::getStringWidth(const char* pFontName, float nSize, const std::string& pText)
{
    CAFont font;
    font.fontName = pFontName;
    font.fontSize = nSize;
    return CAFontProcesstor::widthForTextAtOneLine(pText, font);
}

int CAImage::getStringHeight(const char* pFontName, float nSize, const std::string& pText, float iLimitWidth, int iLineSpace, bool bWordWrap)
{
    CAFont font;
    font.fontName = pFontName;
    font.fontSize = nSize;
    font.wordWrap = bWordWrap;
    return CAFontProcesstor::heightForTextAtWidth(pText, font, iLimitWidth);
}

CAImage* CAImage::createWithString(const std::string& text, const CAFont& font, DSize& dimensions, CATextAlignment textAlignment)
{
    return CAFontProcesstor::imageForText(text, font, dimensions, textAlignment);
}

CAImage* CAImage::create(const std::string& file)
{
    return CAApplication::getApplication()->getImageCache()->addImage(file);
}

CAImage* CAImage::createWithImageDataNoCache(CAData* data)
{
    CAImage* image = new CAImage();
    if (image && image->initWithImageData(data))
    {
        image->autorelease();
        return image;
    }
    CC_SAFE_DELETE(image);
    return NULL;
}

CAImage* CAImage::createWithImageData(CAData* data, const std::string& key)
{
    CAImage* image = CAImage::createWithImageDataNoCache(data);
    if (image)
    {
        CAApplication::getApplication()->getImageCache()->setImageForKey(image, key.c_str());
    }
    return image;
}

CAImage* CAImage::createWithRawDataNoCache(CAData* data,
                                           const CAImage::PixelFormat& pixelFormat,
                                           unsigned int pixelsWide,
                                           unsigned int pixelsHigh,
                                           bool hasPremultipliedAlpha)
{
    CAImage* image = new CAImage();
    if (image && image->initWithRawData(data, pixelFormat, pixelsWide, pixelsHigh, hasPremultipliedAlpha))
    {
        image->autorelease();
        return image;
    }
    CC_SAFE_DELETE(image);
    return NULL;
}

CAImage* CAImage::createWithRawData(CAData* data,
                                    const CAImage::PixelFormat& pixelFormat,
                                    unsigned int pixelsWide,
                                    unsigned int pixelsHigh,
                                    const std::string& key,
                                    bool hasPremultipliedAlpha)
{
    CAImage* image = CAImage::createWithRawDataNoCache(data, pixelFormat, pixelsWide, pixelsHigh, hasPremultipliedAlpha);
    if (image)
    {
        CAApplication::getApplication()->getImageCache()->setImageForKey(image, key.c_str());
    }
    return image;
}

CAImage* CAImage::createWithColor4B(const CrossApp::CAColor4B &color)
{
    CAImage* image = new CAImage();
    if (image && image->initWithColor4B(color))
    {
        image->autorelease();
        return image;
    }
    CC_SAFE_DELETE(image);
    return image;
}

bool CAImage::initWithImageFile(const std::string& file, bool isOpenGLThread)
{
    CAData* data = new CAData();
    unsigned long pSize = 0;
    unsigned char* pData = FileUtils::getInstance()->getFileData(file, "rb", &pSize);
    
    if (pSize > 0)
    { 
        data->fastSet(pData, pSize);
    }
    else
    {
        CCLog("CAImage:: No files found %s", file.c_str());
    }
    bool bRet = initWithImageData(data, isOpenGLThread);
    data->release();
	return bRet;
}

bool CAImage::initWithImageData(CAData* data, bool isOpenGLThread)
{
    bool ret = false;
    
    do
    {
        CC_BREAK_IF(data == nullptr || data->isNull());
        
		unsigned char* unpackedData = data->getBytes();
		unsigned long unpackedLen = data->getLength();
        
        Format type = this->detectFormat(unpackedData, unpackedLen);
        
        switch (type)
        {
            case CAImage::Format::JPG:
                ret = this->initWithJpgData(unpackedData, unpackedLen);
                break;
            case CAImage::Format::PNG:
                ret = this->initWithPngData(unpackedData, unpackedLen);
                break;
            case CAImage::Format::TIFF:
                ret = this->initWithTiffData(unpackedData, unpackedLen);
                break;
            case CAImage::Format::WEBP:
                ret = this->initWithWebpData(unpackedData, unpackedLen);
                break;
            case CAImage::Format::ETC:
            default:
            {
                // load and detect image format
                tImageTGA* tgaData = tgaLoadBuffer(unpackedData, unpackedLen);
                
                if (tgaData != nullptr && tgaData->status == TGA_OK)
                {
                    ret = initWithTGAData(tgaData);
                }
                else
                {
                    ret = false;
                }
                break;
            }
        }
        if (ret)
        {
            if (isOpenGLThread)
            {
                this->premultipliedImageData();
            }
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
    
    bool bRet = false;
    do
    {
        /* We set up the normal JPEG error routines, then override error_exit. */
        cinfo.err = jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = myErrorExit;
        /* Establish the setjmp return context for MyErrorExit to use. */
        if (setjmp(jerr.setjmp_buffer))
        {
            /* If we get here, the JPEG code has signaled an error.
             * We need to clean up the JPEG object, close the input file, and return.
             */
            jpeg_destroy_decompress(&cinfo);
            break;
        }
        
        /* setup decompression process and source, then read JPEG header */
        jpeg_create_decompress( &cinfo );
        
#ifndef CC_TARGET_QT5
        jpeg_mem_src(&cinfo, const_cast<unsigned char*>(data), dataLen);
#endif /* CC_TARGET_QT5 */
        
        /* reading the image header which contains image information */
#if (JPEG_LIB_VERSION >= 90)
        // libjpeg 0.9 adds stricter types.
        jpeg_read_header(&cinfo, TRUE);
#else
        jpeg_read_header(&cinfo, TRUE);
#endif
        
        // we only support RGB or grayscale
        if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
        {
            m_ePixelFormat = CAImage::PixelFormat::I8;
        }
        else
        {
            cinfo.out_color_space = JCS_RGB;
            m_ePixelFormat = CAImage::PixelFormat::RGB888;
        }
        
        /* Start decompression jpeg here */
        jpeg_start_decompress( &cinfo );
        
        /* init image info */
        m_uPixelsWide  = cinfo.output_width;
        m_uPixelsHigh = cinfo.output_height;
        m_bHasPremultipliedAlpha = false;
        m_nBitsPerComponent = 8;
        
        ssize_t length = cinfo.output_width*cinfo.output_height*cinfo.output_components;
        unsigned char* pData = static_cast<unsigned char*>(malloc(length * sizeof(unsigned char)));
        CC_BREAK_IF(! pData);
        
        /* now actually read the jpeg into the raw buffer */
        /* read one scan line at a time */
        while (cinfo.output_scanline < cinfo.output_height)
        {
            row_pointer[0] = pData + location;
            location += cinfo.output_width*cinfo.output_components;
            jpeg_read_scanlines(&cinfo, row_pointer, 1);
        }
        
        /* When read image file with broken data, jpeg_finish_decompress() may cause error.
         * Besides, jpeg_destroy_decompress() shall deallocate and release all memory associated
         * with the decompression object.
         * So it doesn't need to call jpeg_finish_decompress().
         */
        //jpeg_finish_decompress( &cinfo );
        jpeg_destroy_decompress( &cinfo );
        /* wrap up decompression, destroy objects, free pointers and close open files */
        
        m_pData = new CAData();
        m_pData->fastSet(pData, length);
        
        bRet = true;
    } while (0);

    return bRet;
}

bool CAImage::initWithPngData(const unsigned char * data, unsigned long dataLen)
{
    // length of bytes to check if it is a valid png file
#define PNGSIGSIZE  8
    bool ret = false;
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
        
#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA && CC_TARGET_PLATFORM != CC_PLATFORM_NACL && CC_TARGET_PLATFORM != CC_PLATFORM_TIZEN)
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
        // Expanded earlier for grayscale, now take care of palette and rgb
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
                m_ePixelFormat = CAImage::PixelFormat::I8;
                break;
            case PNG_COLOR_TYPE_GRAY_ALPHA:
                m_ePixelFormat = CAImage::PixelFormat::AI88;
                break;
            case PNG_COLOR_TYPE_RGB:
                m_ePixelFormat = CAImage::PixelFormat::RGB888;
                break;
            case PNG_COLOR_TYPE_RGB_ALPHA:
                m_ePixelFormat = CAImage::PixelFormat::RGBA8888;
                break;
            default:
                break;
        }
        
        // read png data
        m_nBitsPerComponent = bit_depth;
        png_size_t rowbytes;
        png_bytep* row_pointers = (png_bytep*)malloc( sizeof(png_bytep) * m_uPixelsHigh);
        
        rowbytes = png_get_rowbytes(png_ptr, info_ptr);
        
        ssize_t length = rowbytes * m_uPixelsHigh;
        unsigned char* pData = static_cast<unsigned char*>(malloc(length * sizeof(unsigned char)));
        if(!pData)
        {
            if (row_pointers != nullptr)
            {
                free(row_pointers);
            }
            break;
        }
        
        for (unsigned short i = 0; i < m_uPixelsHigh; ++i)
        {
            row_pointers[i] = pData + i*rowbytes;
        }
        png_read_image(png_ptr, row_pointers);
        
        png_read_end(png_ptr, nullptr);
        
        // premultiplied alpha for RGBA8888
        if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        {
            unsigned int* fourBytes = (unsigned int*)pData;
            for(int i = 0; i < m_uPixelsWide * m_uPixelsHigh; i++)
            {
                unsigned char* p = pData + i * 4;
                fourBytes[i] = (unsigned)(((unsigned)((unsigned char)(p[0]) * ((unsigned char)(p[3]) + 1)) >> 8) |
                                          ((unsigned)((unsigned char)(p[1]) * ((unsigned char)(p[3]) + 1) >> 8) << 8) |
                                          ((unsigned)((unsigned char)(p[2]) * ((unsigned char)(p[3]) + 1) >> 8) << 16) |
                                          ((unsigned)(unsigned char)(p[3]) << 24));
            }
            
            m_bHasPremultipliedAlpha = true;
        }
        else
        {
            m_bHasPremultipliedAlpha = false;
        }
        

        if (row_pointers != nullptr)
        {
            free(row_pointers);
        }
        
        m_pData = new CAData();
        m_pData->fastSet(pData, length);
        
        ret = true;
    } while (0);
    
    if (png_ptr)
    {
        png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
    }
    return ret;
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
        
        m_ePixelFormat = CAImage::PixelFormat::RGBA8888;
        m_uPixelsWide = w;
        m_uPixelsHigh = h;
        m_nBitsPerComponent = 8;
        
        ssize_t length = npixels * sizeof (uint32);
        unsigned char* pData = static_cast<unsigned char*>(malloc(length * sizeof(unsigned char)));
        
        uint32* raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
        if (raster != NULL)
        {
            if (TIFFReadRGBAImageOriented(tif, w, h, raster, ORIENTATION_TOPLEFT, 0))
            {
                /* the raster data is pre-multiplied by the alpha component
                 after invoking TIFFReadRGBAImageOriented*/
                m_bHasPremultipliedAlpha = true;
                
                memcpy(pData, raster, npixels*sizeof (uint32));
            }
            
            _TIFFfree(raster);
        }
        
        
        TIFFClose(tif);
        
        m_pData = new CAData();
        m_pData->fastSet(pData, length);
        
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
        m_ePixelFormat = CAImage::PixelFormat::RGBA8888;
        m_uPixelsWide    = config.input.width;
        m_uPixelsHigh   = config.input.height;
        m_nBitsPerComponent = 8;
        
        ssize_t length = m_uPixelsWide * m_uPixelsHigh * 4;
        unsigned char* pData = static_cast<unsigned char*>(malloc(length * sizeof(unsigned char)));
        
        config.output.u.RGBA.rgba = static_cast<uint8_t*>(pData);
        config.output.u.RGBA.stride = m_uPixelsWide * 4;
        config.output.u.RGBA.size = length;
        config.output.is_external_memory = 1;
        
        if (WebPDecode(static_cast<const uint8_t*>(data), dataLen, &config) != VP8_STATUS_OK)
        {
            free(pData);
            break;
        }
        
        m_pData = new CAData();
        m_pData->fastSet(pData, length);
        
        bRet = true;
    } while (0);
    return bRet;
}

bool CAImage::initWithTGAData(tImageTGA* tgaData)
{
    bool ret = false;
    
    do
    {
        CC_BREAK_IF(tgaData == nullptr);
        
        // tgaLoadBuffer only support type 2, 3, 10
        if (2 == tgaData->type || 10 == tgaData->type)
        {
            // true color
            // unsupported RGB555
            if (tgaData->pixelDepth == 16)
            {
                m_ePixelFormat = CAImage::PixelFormat::RGB5A1;
            }
            else if(tgaData->pixelDepth == 24)
            {
                m_ePixelFormat = CAImage::PixelFormat::RGB888;
            }
            else if(tgaData->pixelDepth == 32)
            {
                m_ePixelFormat = CAImage::PixelFormat::RGBA8888;
            }
            else
            {
                break;
            }
        }
        else if(3 == tgaData->type)
        {
            // gray
            if (8 == tgaData->pixelDepth)
            {
                m_ePixelFormat = CAImage::PixelFormat::I8;
            }
            else
            {
                break;
            }
        }
        
        m_uPixelsWide = tgaData->width;
        m_uPixelsHigh = tgaData->height;
        m_bHasPremultipliedAlpha = false;
        
        m_pData = new CAData();
        m_pData->fastSet(tgaData->imageData, m_uPixelsWide * m_uPixelsHigh * tgaData->pixelDepth / 8);

        ret = true;
        
    }while(false);
    
    return ret;
}

bool CAImage::initWithRawData(CAData* data,
                              const CAImage::PixelFormat& pixelFormat,
                              unsigned int pixelsWide,
                              unsigned int pixelsHigh,
                              bool hasPremultipliedAlpha)
{
    CC_SAFE_RETAIN(data);
    CC_SAFE_RELEASE(m_pData);
    m_pData = data;
    
    if (m_pData == nullptr || m_pData->isNull())
    {
        return false;
    }
    
    m_tContentSize = DSize(pixelsWide, pixelsHigh);
    m_uPixelsWide = pixelsWide;
    m_uPixelsHigh = pixelsHigh;
    m_ePixelFormat = pixelFormat;
    m_fMaxS = 1;
    m_fMaxT = 1;
    
    m_bHasMipmaps = false;
    m_bHasPremultipliedAlpha = hasPremultipliedAlpha;
    this->repremultipliedImageData();
    
    return true;
}

bool CAImage::initWithColor4B(const CAColor4B& color)
{
    unsigned char pixels[4] = {color.r, color.g, color.b, color.a};
    m_pData = new CAData();
    m_pData->copy((const unsigned char *)&pixels, 4);
    
    m_tContentSize = DSize(1, 1);
    m_uPixelsWide = 1;
    m_uPixelsHigh = 1;
    m_ePixelFormat = CAImage::PixelFormat::RGBA8888;
    m_fMaxS = 1;
    m_fMaxT = 1;
    
    m_bHasMipmaps = false;
    m_bHasPremultipliedAlpha = false;
    this->repremultipliedImageData();
    
    return true;
}

void CAImage::convertToRawData()
{
    m_tContentSize = DSize(m_uPixelsWide, m_uPixelsHigh);
    m_fMaxS = 1;
    m_fMaxT = 1;
    
    m_bHasMipmaps = false;
}

void CAImage::premultipliedImageData()
{
    this->convertToRawData();
    
    CC_RETURN_IF(m_bPremultiplied);
    m_bPremultiplied = true;
    
    unsigned int bitsPerPixel;
    if(m_ePixelFormat == CAImage::PixelFormat::RGB888)
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
    GL::bindTexture2D(m_uName);
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    
    // Specify OpenGL texture image
    
    switch(m_ePixelFormat)
    {
        case CAImage::PixelFormat::RGBA8888:
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGBA,
                         (GLsizei)m_uPixelsWide,
                         (GLsizei)m_uPixelsHigh,
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         (const GLvoid *)m_pData->getBytes());
            break;
        case CAImage::PixelFormat::RGB888:
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGB,
                         (GLsizei)m_uPixelsWide,
                         (GLsizei)m_uPixelsHigh,
                         0,
                         GL_RGB,
                         GL_UNSIGNED_BYTE,
                         (const GLvoid *)m_pData->getBytes());
            break;
        case CAImage::PixelFormat::RGBA4444:
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGBA,
                         (GLsizei)m_uPixelsWide,
                         (GLsizei)m_uPixelsHigh,
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_SHORT_4_4_4_4,
                         (const GLvoid *)m_pData->getBytes());
            break;
        case CAImage::PixelFormat::RGB5A1:
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGBA,
                         (GLsizei)m_uPixelsWide,
                         (GLsizei)m_uPixelsHigh,
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_SHORT_5_5_5_1,
                         (const GLvoid *)m_pData->getBytes());
            break;
        case CAImage::PixelFormat::RGB565:
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGB,
                         (GLsizei)m_uPixelsWide,
                         (GLsizei)m_uPixelsHigh,
                         0,
                         GL_RGB,
                         GL_UNSIGNED_SHORT_5_6_5,
                         (const GLvoid *)m_pData->getBytes());
            break;
        case CAImage::PixelFormat::AI88:
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_LUMINANCE_ALPHA,
                         (GLsizei)m_uPixelsWide,
                         (GLsizei)m_uPixelsHigh,
                         0,
                         GL_LUMINANCE_ALPHA,
                         GL_UNSIGNED_BYTE,
                         (const GLvoid *)m_pData->getBytes());
            break;
        case CAImage::PixelFormat::A8:
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_ALPHA,
                         (GLsizei)m_uPixelsWide,
                         (GLsizei)m_uPixelsHigh,
                         0,
                         GL_ALPHA,
                         GL_UNSIGNED_BYTE,
                         (const GLvoid *)m_pData->getBytes());
            break;
        case CAImage::PixelFormat::I8:
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_LUMINANCE,
                         (GLsizei)m_uPixelsWide,
                         (GLsizei)m_uPixelsHigh,
                         0,
                         GL_LUMINANCE,
                         GL_UNSIGNED_BYTE,
                         (const GLvoid *)m_pData->getBytes());
            break;
        default:
            CCAssert(0, "NSInternalInconsistencyException");
            
    }
    
    setShaderProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE));
}

void CAImage::freeName()
{
    if(m_uName)
    {
        GL::deleteTexture(m_uName);
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

GLProgram* CAImage::getShaderProgram(void)
{
    return m_pShaderProgram;
}

void CAImage::setShaderProgram(GLProgram* pShaderProgram)
{
    CC_SAFE_RETAIN(pShaderProgram);
    CC_SAFE_RELEASE(m_pShaderProgram);
    m_pShaderProgram = pShaderProgram;
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
    
    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD );
    m_pShaderProgram->use();
    m_pShaderProgram->setUniformsForBuiltins();
    
    GL::bindTexture2D( m_uName );
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, coordinates);
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
    
    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD );
    m_pShaderProgram->use();
    m_pShaderProgram->setUniformsForBuiltins();
    
    GL::bindTexture2D( m_uName );
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, coordinates);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void CAImage::generateMipmap()
{
    CCAssert( m_uPixelsWide == ccNextPOT(m_uPixelsWide) && m_uPixelsHigh == ccNextPOT(m_uPixelsHigh), "Mipmap texture only works in POT textures");
    GL::bindTexture2D( m_uName );
    glGenerateMipmap(GL_TEXTURE_2D);
    m_bHasMipmaps = true;
}

bool CAImage::hasMipmaps()
{
    return m_bHasMipmaps;
}

void CAImage::setTexParameters(CAImage::TexParams* texParams)
{
    CCAssert( (m_uPixelsWide == ccNextPOT(m_uPixelsWide) || texParams->wrapS == GL_CLAMP_TO_EDGE) &&
             (m_uPixelsHigh == ccNextPOT(m_uPixelsHigh) || texParams->wrapT == GL_CLAMP_TO_EDGE),
             "GL_CLAMP_TO_EDGE should be used in NPOT dimensions");
    
    GL::bindTexture2D( m_uName );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texParams->minFilter );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texParams->magFilter );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texParams->wrapS );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texParams->wrapT );
    
}

void CAImage::setAliasTexParameters()
{
    GL::bindTexture2D( m_uName );
    
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
    GL::bindTexture2D( m_uName );
    
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
        case CAImage::PixelFormat::RGBA8888:
            return  "RGBA8888";
            
        case CAImage::PixelFormat::RGB888:
            return  "RGB888";
            
        case CAImage::PixelFormat::RGB565:
            return  "RGB565";
            
        case CAImage::PixelFormat::RGBA4444:
            return  "RGBA4444";
            
        case CAImage::PixelFormat::RGB5A1:
            return  "RGB5A1";
            
        case CAImage::PixelFormat::AI88:
            return  "AI88";
            
        case CAImage::PixelFormat::A8:
            return  "A8";
            
        case CAImage::PixelFormat::I8:
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
        case CAImage::PixelFormat::RGBA8888:
            ret = 32;
            break;
        case CAImage::PixelFormat::RGB888:
            // It is 32 and not 24, since its internal representation uses 32 bits.
            ret = 32;
            break;
        case CAImage::PixelFormat::RGB565:
            ret = 16;
            break;
        case CAImage::PixelFormat::RGBA4444:
            ret = 16;
            break;
        case CAImage::PixelFormat::RGB5A1:
            ret = 16;
            break;
        case CAImage::PixelFormat::AI88:
            ret = 16;
            break;
        case CAImage::PixelFormat::A8:
            ret = 8;
            break;
        case CAImage::PixelFormat::I8:
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
                row_pointers[i] = (png_bytep)m_pData->getBytes() + i * m_uPixelsWide * 3;
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
                        pTempData[(i * m_uPixelsWide + j) * 3] = m_pData->getBytes()[(i * m_uPixelsWide + j) * 4];
                        pTempData[(i * m_uPixelsWide + j) * 3 + 1] = m_pData->getBytes()[(i * m_uPixelsWide + j) * 4 + 1];
                        pTempData[(i * m_uPixelsWide + j) * 3 + 2] = m_pData->getBytes()[(i * m_uPixelsWide + j) * 4 + 2];
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
                    row_pointers[i] = (png_bytep)m_pData->getBytes() + i * m_uPixelsWide * 4;
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
                    pTempData[(i * m_uPixelsWide + j) * 3] = m_pData->getBytes()[(i * m_uPixelsWide + j) * 4];
                    pTempData[(i * m_uPixelsWide + j) * 3 + 1] = m_pData->getBytes()[(i * m_uPixelsWide + j) * 4 + 1];
                    pTempData[(i * m_uPixelsWide + j) * 3 + 2] = m_pData->getBytes()[(i * m_uPixelsWide + j) * 4 + 2];
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
                row_pointer[0] = & m_pData->getBytes()[cinfo.next_scanline * row_stride];
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
    if (m_ePixelFormat != CAImage::PixelFormat::RGBA8888
        && m_ePixelFormat != CAImage::PixelFormat::RGB888)
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
    newImage->m_nBitsPerComponent = this->m_nBitsPerComponent;
    newImage->m_pData = new CAData();
    newImage->m_pData->copy(this->m_pData->getBytes(), this->m_pData->getLength());
    
    return newImage;
}

const char* CAImage::getImageFileType()
{
    const char* text = NULL;
    
    unsigned char p = m_pData->getBytes()[0] ;
    
    switch (p)
    {
        case 0xFF:
            text = "jpeg";
            break;
        case 0x89:
            text = "png";
            break;
        case 0x49:
        case 0x4D:
            text = "tiff";
        default:
            break;
    }
    return text;
}
static CAImage* s_white_image           = nullptr;
static CAImage* s_black_image           = nullptr;
static CAImage* s_clear_image           = nullptr;
static CAImage* s_shadow_left_image     = nullptr;
static CAImage* s_shadow_right_image    = nullptr;
static CAImage* s_shadow_top_image      = nullptr;
static CAImage* s_shadow_bottom_image   = nullptr;
static CAImage* s_shadow_image      = nullptr;

CAImage* CAImage::WHITE_IMAGE()
{
    if (s_white_image == nullptr)
    {
        unsigned char pixels = 0xff;
        CAData* data = CAData::create();
        data->copy((const unsigned char *)&pixels, 3);
        
        s_white_image = new CAImage();
        s_white_image->initWithRawData(data, CAImage::PixelFormat::I8, 1, 1);
        s_white_image->m_bMonochrome = true;
    }
    return s_white_image;
}

CAImage* CAImage::BLACK_IMAGE()
{
    if (s_black_image == nullptr)
    {
        unsigned char pixels = 0x0;
        CAData* data = CAData::create();
        data->copy((const unsigned char *)&pixels, 3);
        
        s_black_image = new CAImage();
        s_black_image->initWithRawData(data, CAImage::PixelFormat::I8, 1, 1);
        s_black_image->m_bMonochrome = true;
    }
    return s_black_image;
}

CAImage* CAImage::CLEAR_IMAGE()
{
    if (s_clear_image == nullptr)
    {
        unsigned char pixels = 0x0;
        CAData* data = CAData::create();
        data->copy((const unsigned char *)&pixels, 1);
        
        s_clear_image = new CAImage();
        s_clear_image->initWithRawData(data, CAImage::PixelFormat::A8, 1, 1);
        s_clear_image->m_bMonochrome = true;
    }
    return s_clear_image;
}

CAImage* CAImage::CC_SHADOW_IMAGE()
{
    if (s_shadow_image == nullptr)
    {
        unsigned int pixels[2500] = {
            0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1010101, 0x1010101, 0x1010101, 0x2020202, 0x2020202, 0x2020202, 0x3020202, 0x3020202, 0x3020202, 0x3020202, 0x3020202, 0x3020202, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x3020202, 0x3020202, 0x3020202, 0x3020202, 0x3020202, 0x3020202, 0x2020202, 0x2020202, 0x2020202, 0x1010101, 0x1010101, 0x1010101, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1010101, 0x1010101, 0x2020202, 0x2020202, 0x3020202, 0x4030303, 0x4040404, 0x5040404, 0x6050505, 0x7050505, 0x7050505, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x7050505, 0x7050505, 0x6050505, 0x5040404, 0x4040404, 0x4030303, 0x3020202, 0x2020202, 0x2020202, 0x1010101, 0x1010101, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1010101, 0x1010101, 0x2020202, 0x3020202, 0x4040404, 0x5040404, 0x7050505, 0x8070707, 0xa080808, 0xb090909, 0xc0a0a0a, 0xd0a0a0a, 0xe0b0b0b, 0xe0b0b0b, 0xe0b0b0b, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xe0b0b0b, 0xe0b0b0b, 0xe0b0b0b, 0xd0a0a0a, 0xc0a0a0a, 0xb090909, 0xa080808, 0x8070707, 0x7050505, 0x5040404, 0x4040404, 0x3020202, 0x2020202, 0x1010101, 0x1010101, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1010101, 0x2020202, 0x4030303, 0x4040404, 0x7050505, 0x8070707, 0xb090909, 0xd0a0a0a, 0x100c0c0c, 0x120e0e0e, 0x140f0f0f, 0x15111111, 0x17121212, 0x18121212, 0x18121212, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18121212, 0x18121212, 0x17121212, 0x15111111, 0x140f0f0f, 0x120e0e0e, 0x100c0c0c, 0xd0a0a0a, 0xb090909, 0x8070707, 0x7050505, 0x4040404, 0x4030303, 0x2020202, 0x1010101, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1010101, 0x2020202, 0x3020202, 0x4040404, 0x7050505, 0xa080808, 0xd0a0a0a, 0x100d0d0d, 0x14101010, 0x18131313, 0x1c151515, 0x1e171717, 0x20191919, 0x231b1b1b, 0x241c1c1c, 0x241c1c1c, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x241c1c1c, 0x241c1c1c, 0x231b1b1b, 0x20191919, 0x1e171717, 0x1c151515, 0x18131313, 0x14101010, 0x100d0d0d, 0xd0a0a0a, 0xa080808, 0x7050505, 0x4040404, 0x3020202, 0x2020202, 0x1010101, 0x0, 0x0, 0x0, 0x0, 0x1010101, 0x2020202, 0x4040404, 0x7050505, 0xa080808, 0xe0b0b0b, 0x130f0f0f, 0x18121212, 0x1d171717, 0x231b1b1b, 0x281f1f1f, 0x2c222222, 0x2f242424, 0x31262626, 0x33272727, 0x34292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x34292929, 0x33272727, 0x31262626, 0x2f242424, 0x2c222222, 0x281f1f1f, 0x231b1b1b, 0x1d171717, 0x18121212, 0x130f0f0f, 0xe0b0b0b, 0xa080808, 0x7050505, 0x4040404, 0x2020202, 0x1010101, 0x0, 0x0, 0x0, 0x0, 0x1010101, 0x3020202, 0x5040404, 0x8070707, 0xd0a0a0a, 0x130f0f0f, 0x19141414, 0x20191919, 0x281f1f1f, 0x2f242424, 0x35292929, 0x3b2d2d2d, 0x3f313131, 0x43343434, 0x45353535, 0x46363636, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x46363636, 0x45353535, 0x43343434, 0x3f313131, 0x3b2d2d2d, 0x35292929, 0x2f242424, 0x281f1f1f, 0x20191919, 0x19141414, 0x130f0f0f, 0xd0a0a0a, 0x8070707, 0x5040404, 0x3020202, 0x1010101, 0x0, 0x0, 0x0, 0x0, 0x2020202, 0x4030303, 0x7050505, 0xb090909, 0x110d0d0d, 0x18131313, 0x20191919, 0x29202020, 0x33272727, 0x3c2e2e2e, 0x44353535, 0x4c3a3a3a, 0x513f3f3f, 0x55424242, 0x58444444, 0x5a454545, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5a454545, 0x58444444, 0x55424242, 0x513f3f3f, 0x4c3a3a3a, 0x44353535, 0x3c2e2e2e, 0x33272727, 0x29202020, 0x20191919, 0x18131313, 0x110d0d0d, 0xb090909, 0x7050505, 0x4030303, 0x2020202, 0x0, 0x0, 0x0, 0x0, 0x2020202, 0x4040404, 0x8070707, 0xe0b0b0b, 0x14101010, 0x1d171717, 0x281f1f1f, 0x33272727, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x33272727, 0x281f1f1f, 0x1d171717, 0x14101010, 0xe0b0b0b, 0x8070707, 0x4040404, 0x2020202, 0x0, 0x0, 0x0, 0x0, 0x3020202, 0x5040404, 0xa080808, 0x100d0d0d, 0x18131313, 0x231b1b1b, 0x2f242424, 0x3c2e2e2e, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3c2e2e2e, 0x2f242424, 0x231b1b1b, 0x18131313, 0x100d0d0d, 0xa080808, 0x5040404, 0x3020202, 0x0, 0x0, 0x0, 0x0, 0x3020202, 0x6050505, 0xb090909, 0x120e0e0e, 0x1c151515, 0x281f1f1f, 0x35292929, 0x44353535, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x44353535, 0x35292929, 0x281f1f1f, 0x1c151515, 0x120e0e0e, 0xb090909, 0x6050505, 0x3020202, 0x0, 0x0, 0x0, 0x0, 0x3020202, 0x6050505, 0xc0a0a0a, 0x14101010, 0x1f181818, 0x2c222222, 0x3b2d2d2d, 0x4b3a3a3a, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4b3a3a3a, 0x3b2d2d2d, 0x2c222222, 0x1f181818, 0x14101010, 0xc0a0a0a, 0x6050505, 0x3020202, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x7050505, 0xd0a0a0a, 0x15111111, 0x211a1a1a, 0x2f242424, 0x40313131, 0x513f3f3f, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x513f3f3f, 0x40313131, 0x2f242424, 0x211a1a1a, 0x15111111, 0xd0a0a0a, 0x7050505, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x7050505, 0xe0b0b0b, 0x17121212, 0x231b1b1b, 0x31262626, 0x43343434, 0x55424242, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x55424242, 0x43343434, 0x31262626, 0x231b1b1b, 0x17121212, 0xe0b0b0b, 0x7050505, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x8060606, 0xe0b0b0b, 0x18121212, 0x241c1c1c, 0x34282828, 0x45353535, 0x58444444, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x58444444, 0x45353535, 0x34282828, 0x241c1c1c, 0x18121212, 0xe0b0b0b, 0x8060606, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x8060606, 0xf0c0c0c, 0x18131313, 0x241c1c1c, 0x34292929, 0x47373737, 0x5a454545, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5a454545, 0x47373737, 0x34292929, 0x241c1c1c, 0x18131313, 0xf0c0c0c, 0x8060606, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x8060606, 0xf0c0c0c, 0x18131313, 0x251d1d1d, 0x35292929, 0x48373737, 0x5c474747, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5c474747, 0x48373737, 0x35292929, 0x251d1d1d, 0x18131313, 0xf0c0c0c, 0x8060606, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x8060606, 0xf0c0c0c, 0x18131313, 0x251d1d1d, 0x35292929, 0x48373737, 0x5c474747, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5c474747, 0x48373737, 0x35292929, 0x251d1d1d, 0x18131313, 0xf0c0c0c, 0x8060606, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x8060606, 0xf0c0c0c, 0x18131313, 0x251d1d1d, 0x35292929, 0x48373737, 0x5c474747, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5c474747, 0x48373737, 0x35292929, 0x251d1d1d, 0x18131313, 0xf0c0c0c, 0x8060606, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x8060606, 0xf0c0c0c, 0x18131313, 0x251d1d1d, 0x35292929, 0x48373737, 0x5c474747, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5c474747, 0x48373737, 0x35292929, 0x251d1d1d, 0x18131313, 0xf0c0c0c, 0x8060606, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x8060606, 0xf0c0c0c, 0x18131313, 0x251d1d1d, 0x35292929, 0x48373737, 0x5c474747, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5c474747, 0x48373737, 0x35292929, 0x251d1d1d, 0x18131313, 0xf0c0c0c, 0x8060606, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x8060606, 0xf0c0c0c, 0x18131313, 0x251d1d1d, 0x35292929, 0x48373737, 0x5c474747, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5c474747, 0x48373737, 0x35292929, 0x251d1d1d, 0x18131313, 0xf0c0c0c, 0x8060606, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x8060606, 0xf0c0c0c, 0x18131313, 0x251d1d1d, 0x35292929, 0x48373737, 0x5c474747, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5c474747, 0x48373737, 0x35292929, 0x251d1d1d, 0x18131313, 0xf0c0c0c, 0x8060606, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x8060606, 0xf0c0c0c, 0x18131313, 0x251d1d1d, 0x35292929, 0x48373737, 0x5c474747, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5c474747, 0x48373737, 0x35292929, 0x251d1d1d, 0x18131313, 0xf0c0c0c, 0x8060606, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x8060606, 0xf0c0c0c, 0x18131313, 0x251d1d1d, 0x35292929, 0x48373737, 0x5c474747, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5c474747, 0x48373737, 0x35292929, 0x251d1d1d, 0x18131313, 0xf0c0c0c, 0x8060606, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x8060606, 0xf0c0c0c, 0x18131313, 0x251d1d1d, 0x35292929, 0x48373737, 0x5c474747, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5c474747, 0x48373737, 0x35292929, 0x251d1d1d, 0x18131313, 0xf0c0c0c, 0x8060606, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x8060606, 0xf0c0c0c, 0x18131313, 0x251d1d1d, 0x35292929, 0x48373737, 0x5c474747, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5c474747, 0x48373737, 0x35292929, 0x251d1d1d, 0x18131313, 0xf0c0c0c, 0x8060606, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x8060606, 0xf0c0c0c, 0x18131313, 0x251d1d1d, 0x35292929, 0x48373737, 0x5c474747, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5c474747, 0x48373737, 0x35292929, 0x251d1d1d, 0x18131313, 0xf0c0c0c, 0x8060606, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x8060606, 0xf0c0c0c, 0x18131313, 0x251d1d1d, 0x35292929, 0x48373737, 0x5c474747, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5c474747, 0x48373737, 0x35292929, 0x251d1d1d, 0x18131313, 0xf0c0c0c, 0x8060606, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x8060606, 0xf0c0c0c, 0x18131313, 0x251d1d1d, 0x35292929, 0x48373737, 0x5c474747, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5c474747, 0x48373737, 0x35292929, 0x251d1d1d, 0x18131313, 0xf0c0c0c, 0x8060606, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x8060606, 0xf0c0c0c, 0x18131313, 0x241c1c1c, 0x34292929, 0x47373737, 0x5a454545, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5a454545, 0x47373737, 0x34292929, 0x241c1c1c, 0x18131313, 0xf0c0c0c, 0x8060606, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x8060606, 0xe0b0b0b, 0x18121212, 0x241c1c1c, 0x34282828, 0x45353535, 0x58444444, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x58444444, 0x45353535, 0x34282828, 0x241c1c1c, 0x18121212, 0xe0b0b0b, 0x8060606, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x7050505, 0xe0b0b0b, 0x17121212, 0x231b1b1b, 0x31262626, 0x43343434, 0x55424242, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x55424242, 0x43343434, 0x31262626, 0x231b1b1b, 0x17121212, 0xe0b0b0b, 0x7050505, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x4030303, 0x7050505, 0xd0a0a0a, 0x15111111, 0x211a1a1a, 0x2f242424, 0x40313131, 0x513f3f3f, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x513f3f3f, 0x40313131, 0x2f242424, 0x211a1a1a, 0x15111111, 0xd0a0a0a, 0x7050505, 0x4030303, 0x0, 0x0, 0x0, 0x0, 0x3020202, 0x6050505, 0xc0a0a0a, 0x14101010, 0x1f181818, 0x2c222222, 0x3b2d2d2d, 0x4b3a3a3a, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4b3a3a3a, 0x3b2d2d2d, 0x2c222222, 0x1f181818, 0x14101010, 0xc0a0a0a, 0x6050505, 0x3020202, 0x0, 0x0, 0x0, 0x0, 0x3020202, 0x6050505, 0xb090909, 0x120e0e0e, 0x1c151515, 0x281f1f1f, 0x35292929, 0x44353535, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x44353535, 0x35292929, 0x281f1f1f, 0x1c151515, 0x120e0e0e, 0xb090909, 0x6050505, 0x3020202, 0x0, 0x0, 0x0, 0x0, 0x3020202, 0x5040404, 0xa080808, 0x100d0d0d, 0x18131313, 0x231b1b1b, 0x2f242424, 0x3c2e2e2e, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3c2e2e2e, 0x2f242424, 0x231b1b1b, 0x18131313, 0x100d0d0d, 0xa080808, 0x5040404, 0x3020202, 0x0, 0x0, 0x0, 0x0, 0x2020202, 0x4040404, 0x8070707, 0xe0b0b0b, 0x14101010, 0x1d171717, 0x281f1f1f, 0x33272727, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x33272727, 0x281f1f1f, 0x1d171717, 0x14101010, 0xe0b0b0b, 0x8070707, 0x4040404, 0x2020202, 0x0, 0x0, 0x0, 0x0, 0x2020202, 0x4030303, 0x7050505, 0xb090909, 0x110d0d0d, 0x18131313, 0x20191919, 0x29202020, 0x33272727, 0x3c2e2e2e, 0x44353535, 0x4c3a3a3a, 0x513f3f3f, 0x55424242, 0x58444444, 0x5a454545, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5c474747, 0x5a454545, 0x58444444, 0x55424242, 0x513f3f3f, 0x4c3a3a3a, 0x44353535, 0x3c2e2e2e, 0x33272727, 0x29202020, 0x20191919, 0x18131313, 0x110d0d0d, 0xb090909, 0x7050505, 0x4030303, 0x2020202, 0x0, 0x0, 0x0, 0x0, 0x1010101, 0x3020202, 0x5040404, 0x8070707, 0xd0a0a0a, 0x130f0f0f, 0x19141414, 0x20191919, 0x281f1f1f, 0x2f242424, 0x35292929, 0x3b2d2d2d, 0x3f313131, 0x43343434, 0x45353535, 0x46363636, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x48373737, 0x46363636, 0x45353535, 0x43343434, 0x3f313131, 0x3b2d2d2d, 0x35292929, 0x2f242424, 0x281f1f1f, 0x20191919, 0x19141414, 0x130f0f0f, 0xd0a0a0a, 0x8070707, 0x5040404, 0x3020202, 0x1010101, 0x0, 0x0, 0x0, 0x0, 0x1010101, 0x2020202, 0x4040404, 0x7050505, 0xa080808, 0xe0b0b0b, 0x130f0f0f, 0x18121212, 0x1d171717, 0x231b1b1b, 0x281f1f1f, 0x2c222222, 0x2f242424, 0x31262626, 0x33272727, 0x34292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x35292929, 0x34292929, 0x33272727, 0x31262626, 0x2f242424, 0x2c222222, 0x281f1f1f, 0x231b1b1b, 0x1d171717, 0x18121212, 0x130f0f0f, 0xe0b0b0b, 0xa080808, 0x7050505, 0x4040404, 0x2020202, 0x1010101, 0x0, 0x0, 0x0, 0x0, 0x1010101, 0x2020202, 0x3020202, 0x4040404, 0x7050505, 0xa080808, 0xd0a0a0a, 0x100d0d0d, 0x14101010, 0x18131313, 0x1c151515, 0x1e171717, 0x20191919, 0x231b1b1b, 0x241c1c1c, 0x241c1c1c, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x251d1d1d, 0x241c1c1c, 0x241c1c1c, 0x231b1b1b, 0x20191919, 0x1e171717, 0x1c151515, 0x18131313, 0x14101010, 0x100d0d0d, 0xd0a0a0a, 0xa080808, 0x7050505, 0x4040404, 0x3020202, 0x2020202, 0x1010101, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1010101, 0x2020202, 0x4030303, 0x4040404, 0x7050505, 0x8070707, 0xb090909, 0xd0a0a0a, 0x100c0c0c, 0x120e0e0e, 0x140f0f0f, 0x15111111, 0x17121212, 0x18121212, 0x18121212, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18131313, 0x18121212, 0x18121212, 0x17121212, 0x15111111, 0x140f0f0f, 0x120e0e0e, 0x100c0c0c, 0xd0a0a0a, 0xb090909, 0x8070707, 0x7050505, 0x4040404, 0x4030303, 0x2020202, 0x1010101, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1010101, 0x1010101, 0x2020202, 0x3020202, 0x4040404, 0x5040404, 0x7050505, 0x8070707, 0xa080808, 0xb090909, 0xc0a0a0a, 0xd0a0a0a, 0xe0b0b0b, 0xe0b0b0b, 0xe0b0b0b, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xf0c0c0c, 0xe0b0b0b, 0xe0b0b0b, 0xe0b0b0b, 0xd0a0a0a, 0xc0a0a0a, 0xb090909, 0xa080808, 0x8070707, 0x7050505, 0x5040404, 0x4040404, 0x3020202, 0x2020202, 0x1010101, 0x1010101, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1010101, 0x1010101, 0x2020202, 0x2020202, 0x3020202, 0x4030303, 0x4040404, 0x5040404, 0x6050505, 0x7050505, 0x7050505, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x8060606, 0x7050505, 0x7050505, 0x6050505, 0x5040404, 0x4040404, 0x4030303, 0x3020202, 0x2020202, 0x2020202, 0x1010101, 0x1010101, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1010101, 0x1010101, 0x1010101, 0x2020202, 0x2020202, 0x2020202, 0x3020202, 0x3020202, 0x3020202, 0x3020202, 0x3020202, 0x3020202, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x4030303, 0x3020202, 0x3020202, 0x3020202, 0x3020202, 0x3020202, 0x3020202, 0x2020202, 0x2020202, 0x2020202, 0x1010101, 0x1010101, 0x1010101, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
            
        };
        CAData* data = CAData::create();
        data->copy((const unsigned char *)&pixels, 4 * 2500);
        
        s_shadow_image = new CAImage();
        s_shadow_image->initWithRawData(data, CAImage::PixelFormat::RGBA8888, 50, 50);
        s_shadow_image->m_bMonochrome = true;
    }
    return s_shadow_image;
}

CAImage::Format CAImage::detectFormat(const unsigned char * data, unsigned long dataLen)
{
    if (isPng(data, dataLen))
    {
        return CAImage::Format::PNG;
    }
    else if (isJpg(data, dataLen))
    {
        return CAImage::Format::JPG;
    }
    else if (isTiff(data, dataLen))
    {
        return CAImage::Format::TIFF;
    }
    else if (isWebp(data, dataLen))
    {
        return CAImage::Format::WEBP;
    }
    else if (isEtc(data, dataLen))
    {
        return CAImage::Format::ETC;
    }
    else
    {
        return CAImage::Format::UNKOWN;
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

bool CAImage::isEtc(const unsigned char * data, unsigned long dataLen)
{
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

bool CAImage::isPvr(const unsigned char *data, unsigned long dataLen)
{
    return false;
}

namespace
{
    typedef CAImage::PixelFormatInfoMap::value_type PixelFormatInfoMapValue;
    static const PixelFormatInfoMapValue CAImagePixelFormatInfoTablesValue[] =
    {
        PixelFormatInfoMapValue(CAImage::PixelFormat::RGBA8888, CAImage::PixelFormatInfo(GL_BGRA, GL_BGRA, GL_UNSIGNED_BYTE, 32, false, true)),
        PixelFormatInfoMapValue(CAImage::PixelFormat::RGBA8888, CAImage::PixelFormatInfo(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 32, false, true)),
        PixelFormatInfoMapValue(CAImage::PixelFormat::RGBA4444, CAImage::PixelFormatInfo(GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, 16, false, true)),
        PixelFormatInfoMapValue(CAImage::PixelFormat::RGB5A1, CAImage::PixelFormatInfo(GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, 16, false, true)),
        PixelFormatInfoMapValue(CAImage::PixelFormat::RGB565, CAImage::PixelFormatInfo(GL_RGB, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, 16, false, false)),
        PixelFormatInfoMapValue(CAImage::PixelFormat::RGB888, CAImage::PixelFormatInfo(GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, 24, false, false)),
        PixelFormatInfoMapValue(CAImage::PixelFormat::A8, CAImage::PixelFormatInfo(GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE, 8, false, false)),
        PixelFormatInfoMapValue(CAImage::PixelFormat::I8, CAImage::PixelFormatInfo(GL_LUMINANCE, GL_LUMINANCE, GL_UNSIGNED_BYTE, 8, false, false)),
        PixelFormatInfoMapValue(CAImage::PixelFormat::AI88, CAImage::PixelFormatInfo(GL_LUMINANCE_ALPHA, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, 16, false, true)),
    };
}

const CAImage::PixelFormatInfoMap CAImage::s_pixelFormatInfoTables
        (CAImagePixelFormatInfoTablesValue, CAImagePixelFormatInfoTablesValue + sizeof(CAImagePixelFormatInfoTablesValue) / sizeof(CAImagePixelFormatInfoTablesValue[0]));

const CAImage::PixelFormatInfoMap& CAImage::getPixelFormatInfoMap()
{
    return s_pixelFormatInfoTables;
}

int CAImage::getBitPerPixel()
{
    return CAImage::getPixelFormatInfoMap().at(m_ePixelFormat).bpp;
}

bool CAImage::hasAlpha()
{
    return CAImage::getPixelFormatInfoMap().at(m_ePixelFormat).alpha;
}

bool CAImage::isCompressed()
{
    return CAImage::getPixelFormatInfoMap().at(m_ePixelFormat).compressed;
}

void CAImage::reloadAllImages()
{
    for (auto& image : s_pImages)
    {
        image->freeName();
    }
    
    for (auto& image : s_pImages)
    {
        image->repremultipliedImageData();
    }
}

void CAImage::purgeCAImage()
{
    CC_SAFE_DELETE(s_white_image);
    s_white_image = nullptr;
    CC_SAFE_DELETE(s_black_image);
    s_black_image = nullptr;
    CC_SAFE_DELETE(s_clear_image);
    s_clear_image = nullptr;
    CC_SAFE_DELETE(s_shadow_left_image);
    s_shadow_left_image = nullptr;
    CC_SAFE_DELETE(s_shadow_right_image);
    s_shadow_right_image = nullptr;
    CC_SAFE_DELETE(s_shadow_top_image);
    s_shadow_top_image = nullptr;
    CC_SAFE_DELETE(s_shadow_bottom_image);
    s_shadow_bottom_image = nullptr;
}

NS_CC_END
