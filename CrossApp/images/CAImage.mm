#include "CAImage.h"
#include "ccConfig.h"
#include "ccMacros.h"
#include "platform/platform.h"
#include "png.h"
#include "jpeglib.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

NS_CC_BEGIN

bool CAImage::saveImageToPNG(const std::string& filePath, bool isToRGB)
{
    bool bRet = false;
    
#if TARGET_IPHONE_SIMULATOR
    
    CCLog(" 'CAImage::saveToFile', the method does not support the iOS simulator.");
    
#else
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
    
#endif
    
    return bRet;
}

bool CAImage::saveImageToJPG(const std::string& filePath)
{
    bool bRet = false;
    
#if TARGET_IPHONE_SIMULATOR
    
    CCLog(" 'CAImage::saveToFile', the method does not support the iOS simulator.");
    
#else
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
    
#endif
    
    return bRet;
}

NS_CC_END

#endif