//
//  CAGif.h
//  CrossApp
//
//  Created by qiaoxin on 15-06-01.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#include "CAGif.h"
#include "platform/CAFileUtils.h"

NS_CC_BEGIN

static int memReadFuncGif(GifFileType* GifFile, GifByteType* buf, int count)
{
    char* ptr = (char*)(GifFile->UserData);
    memcpy(buf, ptr, count);
    GifFile->UserData = ptr + count;
    return count;
}

static std::map<std::string, CAGif*> s_pGIFs;

const std::map<std::string, CAGif*>& CAGif::getGIFs()
{
    return s_pGIFs;
}

CAGif::CAGif()
:m_fDelay(0.0f)
,m_pGifData(nullptr)
,m_pData(nullptr)
{

}

CAGif::~CAGif()
{
    m_vImages.clear();
    s_pGIFs.erase(m_sFilePath);
}

CAGif* CAGif::create(const std::string& filePath)
{
    if (s_pGIFs.find(filePath) != s_pGIFs.end())
    {
        return s_pGIFs.at(filePath);
    }
    
    CAGif* gif = new CAGif();
    if(gif && gif->initWithFilePath(filePath))
    {
        gif->autorelease();
        return gif;
    }
    CC_SAFE_DELETE(gif);
    return NULL;
}

CAGif* CAGif::createWithData(CAData* data)
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

bool CAGif::initWithFilePath(const std::string& filePath)
{
    if (s_pGIFs.find(filePath) != s_pGIFs.end())
    {
        return false;
    }
    
    CAData* data = FileUtils::getInstance()->getDataFromFile(filePath);
    if (!data->isNull() && this->initWithData(data))
    {
        m_sFilePath = filePath;
        s_pGIFs.insert(std::make_pair(m_sFilePath, this));
        return true;
    }
    
    return false;
}

bool CAGif::initWithData(CAData* data)
{
    if(data == nullptr || data->isNull() || !CAGif::init())
    {
        return false;
    }
    
    int error = 0;
    
    m_pGifData = data;
    
    m_pGIF = DGifOpen(m_pGifData->getBytes(), &memReadFuncGif, &error);

    if (nullptr == m_pGIF || DGifSlurp(m_pGIF) != GIF_OK)
    {
        int ErrorCode;
        DGifCloseFile(m_pGIF, &ErrorCode);
        m_pGifData = nullptr;
        m_pGIF = nullptr;
        return false;
    }
    
    m_uPixelsWide = m_pGIF->SWidth;
    m_uPixelsHigh = m_pGIF->SHeight;
    m_pData = (unsigned char*)malloc(sizeof(unsigned char) * m_uPixelsWide * m_uPixelsHigh * 4);
    
    m_fDelay = this->getImageDelay(&m_pGIF->SavedImages[0]);
    
    for (int i=0; i<m_pGIF->ImageCount; ++i)
    {
        m_vImages.pushBack(this->getImage(i));
    }
    
    int ErrorCode;
    DGifCloseFile(m_pGIF, &ErrorCode);
    
    m_pGifData = nullptr;
    m_pGIF = nullptr;
    CC_SAFE_DELETE(m_pData);
    
    return true;
}

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

CAImage* CAGif::getImageWithIndex(unsigned int index)
{
    if (index < m_vImages.size())
    {
        return m_vImages.at(index);
    }
    return nullptr;
}

CAImage* CAGif::getImage(unsigned int index)
{
    index = MIN(index, m_pGIF->ImageCount - 1);
 
    CAColor4B bgColor;
    if (m_pGIF->SColorMap != NULL)
    {
        const GifColorType& col = m_pGIF->SColorMap->Colors[m_pGIF->SBackGroundColor];
        bgColor = CAColor4B(col.Red, col.Green, col.Blue, 0xFF);
    }
    
    static char paintingColor[4] = {0, 0, 0, 0};
    
    
    const SavedImage* curr = &m_pGIF->SavedImages[index];

    if (index == 0)
    {
        bool trans;
        int disposal;
        this->getTransparencyAndDisposalMethod(curr, &trans, &disposal);
        
        for (unsigned int i = 0; i < m_uPixelsWide * m_uPixelsHigh; i++)
        {
            *(m_pData + i * 4)     = '\0';
            *(m_pData + i * 4 + 1) = '\0';
            *(m_pData + i * 4 + 2) = '\0';
            *(m_pData + i * 4 + 3) = '\0';
        }
    }
    else
    {
        const SavedImage* prev = &m_pGIF->SavedImages[index - 1];
        
        bool prevTrans;
        int prevDisposal;
        this->getTransparencyAndDisposalMethod(prev, &prevTrans, &prevDisposal);
        
        bool currTrans;
        int currDisposal;
        this->getTransparencyAndDisposalMethod(curr, &currTrans, &currDisposal);
        
        
        if (currTrans || !checkIfCover(curr, prev))
        {
            if (prevDisposal == 2)
            {
                for (int top = prev->ImageDesc.Top; top < MIN(m_uPixelsHigh, prev->ImageDesc.Top + prev->ImageDesc.Height); top++)
                {
                    for(int left = prev->ImageDesc.Left; left < MIN(m_uPixelsWide, prev->ImageDesc.Left + prev->ImageDesc.Width); left++)
                    {
                        unsigned char* dst = &m_pData[(top * m_uPixelsWide + left) * 4];
                        *dst     = paintingColor[0];
                        *(dst+1) = paintingColor[1];
                        *(dst+2) = paintingColor[2];
                        *(dst+3) = paintingColor[3];
                    }
                }
            }
        }
    }
    
    {
        int transparent = -1;
        for (int i = 0; i < curr->ExtensionBlockCount; ++i)
        {
            ExtensionBlock* eb = curr->ExtensionBlocks + i;
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
        
        if (curr->ImageDesc.ColorMap != NULL)
        {
            GifFreeMapObject(m_pGIF->SColorMap);
            m_pGIF->SColorMap = GifMakeMapObject(curr->ImageDesc.ColorMap->ColorCount,
                                                 curr->ImageDesc.ColorMap->Colors);
        }
        
        if (m_pGIF->SColorMap && m_pGIF->SColorMap->ColorCount == (1 << m_pGIF->SColorMap->BitsPerPixel))
        {
            unsigned char* src = (unsigned char*)curr->RasterBits;
            unsigned char* dst = &m_pData[(curr->ImageDesc.Top * m_uPixelsWide + curr->ImageDesc.Left) * 4];
            
            GifWord copyWidth = curr->ImageDesc.Width;
            if (curr->ImageDesc.Left + copyWidth > m_uPixelsWide)
            {
                copyWidth = m_uPixelsWide - curr->ImageDesc.Left;
            }
            
            GifWord copyHeight = curr->ImageDesc.Height;
            if (curr->ImageDesc.Top + copyHeight > m_uPixelsHigh)
            {
                copyHeight = m_uPixelsHigh - curr->ImageDesc.Top;
            }
            
            for (; copyHeight > 0; copyHeight--)
            {
                copyLine(dst, src, m_pGIF->SColorMap, transparent, copyWidth);
                src += curr->ImageDesc.Width;
                dst += m_uPixelsWide*4;
            }
        }
    }

    CAData* data = CAData::create();
    data->copy(m_pData, m_uPixelsWide * m_uPixelsHigh * 4);
    
    return CAImage::createWithRawDataNoCache(data, CAImage::PixelFormat::RGBA8888, m_uPixelsWide, m_uPixelsHigh);
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

float CAGif::getImageDelay(const SavedImage* image)
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
            duration =  ((b[2] << 8) | b[1]) * 5.5;
            break;
        }
    }
    duration = duration <= 100 ? 100 : duration;
    return duration;
}


NS_CC_END
