//
//  CAGif.h
//  CrossApp
//
//  Created by qiaoxin on 15-06-01.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#include "CAGif.h"

NS_CC_BEGIN

static const unsigned char* s_pData = NULL;
static int  s_nDataMark = 0;

static int DecodeCallBackProc(GifFileType* gif, GifByteType* bytes, int size)
{
    for(int i=0; i<size; i++, s_nDataMark++)
    {
        bytes[i] = s_pData[s_nDataMark];
    }
    return size;
}

CAGif::CAGif()
:m_fDurTime(0.0f)
,m_pImage(NULL)
,m_pDataSize(0)
,m_pImageData(NULL)
{
    m_pImage = new CAImage();
}

CAGif::~CAGif()
{
    CC_SAFE_RELEASE(m_pImage);
    CC_SAFE_DELETE(m_pImageData);
}

CAGif* CAGif::createWithFilePath(std::string filePath)
{
    CAGif* gif = new CAGif();
    if(gif && gif->initWithFilePath(filePath))
    {
        gif->autorelease();
        return gif;
    }
    CC_SAFE_DELETE(gif);
    return NULL;
}

CAGif* CAGif::createWithData(unsigned char* data)
{
    CAGif* gif = new CAGif();
    if(gif && gif->initWithData(data))
    {
        gif->autorelease();
        return gif;
    }
    CC_SAFE_DELETE(gif);
    return NULL;
}

bool CAGif::initWithFilePath(std::string filePath)
{
    if(CAGif::init())
    {
        std::string fileFullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath.c_str());
        s_pData = CCFileUtils::sharedFileUtils()->getFileData(fileFullPath.c_str(), "rb", &m_pDataSize);
        if(!s_pData || 0 == m_pDataSize)
        {
            return false;
        }
        s_nDataMark = 0;
        int error = 0;
        m_pGIF = DGifOpen(NULL,&DecodeCallBackProc,&error);
        if (NULL == m_pGIF || DGifSlurp(m_pGIF) != GIF_OK)
        {
            DGifCloseFile(m_pGIF);
            m_pGIF = NULL;
            return false;
        }
        m_uPixelsWide = m_pGIF->SWidth;
        m_uPixelsHigh = m_pGIF->SHeight;
        m_pImageData = (unsigned char*)malloc(sizeof(unsigned char) * m_uPixelsWide * m_uPixelsHigh * 4);
        for (unsigned int i = 0; i < m_uPixelsWide * m_uPixelsHigh; i++)
        {
            *(m_pImageData + i * 4)     = '\0';
            *(m_pImageData + i * 4 + 1) = '\0';
            *(m_pImageData + i * 4 + 2) = '\0';
            *(m_pImageData + i * 4 + 3) = '\0';
        }
        this->setGifImageWithIndex(0);
        
        return true;
    }
    return false;
}

bool CAGif::initWithData(unsigned char* data)
{
    if(CAGif::init())
    {
        return true;
    }
    return false;
}

unsigned int CAGif::getGifImageIndex()
{
    return m_iGIFIndex;
}

unsigned int CAGif::getGifImageCounts()
{
    return m_pGIF ? m_pGIF->ImageCount : 0;
}

float CAGif::getImageDuration()
{
    return m_fDurTime;
}

int CAGif::getWidth()
{
   // CCLog("%d", m_uPixelsWide);
    return m_uPixelsWide;
}

int CAGif::getHeight()
{
    return m_uPixelsHigh;
}

//const DSize& CAGif::getGifSize()
//{
//    return DSize(m_uPixelsWide, m_uPixelsHigh);
//}

void CAGif::copyLine(unsigned char* dst, const unsigned char* src, const ColorMapObject* cmap, int transparent, int width)
{
    for (; width > 0; width--, src++, dst+=4) {
        if (*src != transparent) {
            const GifColorType& col = cmap->Colors[*src];
            *dst     = col.Red;
            *(dst+1) = col.Green;
            *(dst+2) = col.Blue;
            *(dst+3) = 0xFF;
        }
    }
}

void CAGif::setGifImageWithIndex(unsigned int index)
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
    
    m_fDurTime = getImageDuration(cur);

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
    m_pImage->initWithRawData(m_pImageData, CAImage::PixelFormat_RGBA8888, m_uPixelsWide, m_uPixelsHigh);
}

void CAGif::nextGifImageIndex()
{
    int index = (m_iGIFIndex + 1) % this->getGifImageCounts();
    this->setGifImageWithIndex(index);
}

void CAGif::getTransparencyAndDisposalMethod(const SavedImage* frame, bool* trans, int* disposal)
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

bool CAGif::checkIfCover(const SavedImage* target, const SavedImage* covered)
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

bool CAGif::checkIfWillBeCleared(const SavedImage* frame)
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

float CAGif::getImageDuration(const SavedImage* image)
{
    float duration = 0;
    for (int j = 0; j < image->ExtensionBlockCount; j++)
    {
        if (image->ExtensionBlocks[j].Function == GRAPHICS_EXT_FUNC_CODE)
        {
            int size = image->ExtensionBlocks[j].ByteCount;
            //assert(size >= 4);
            if(size < 4) break;
            const uint8_t* b = (const uint8_t*)image->ExtensionBlocks[j].Bytes;
            duration =  ((b[2] << 8) | b[1]) * 10;
            break;
        }
    }
    duration = duration <= 50 ? 50 : duration;
    return duration;
}


NS_CC_END
