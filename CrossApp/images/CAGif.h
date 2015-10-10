//
//  CAGif.h
//  CrossApp
//
//  Created by qiaoxin on 15-06-01.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CAGIF_H__
#define __CAGIF_H__

#include "CrossApp.h"
#include "gif_lib/gif_lib.h"

NS_CC_BEGIN
class CAImage;

class CC_DLL CAGif : public CAObject
{
public:
    CAGif();
    ~CAGif();
    static CAGif* createWithFilePath(std::string filePath);
    static CAGif* createWithData(unsigned char* data);
    
    bool initWithFilePath(std::string filePath);
    bool initWithData(unsigned char* data);
    
    void setGifImageWithIndex(unsigned int index);
    unsigned int getGifImageIndex();
    unsigned int getGifImageCounts();
    void nextGifImageIndex();
    float getImageDuration();
    CAImage* getImage() { return m_pImage; }
    int getWidth();
    int getHeight();
protected:
    void getTransparencyAndDisposalMethod(const SavedImage* frame, bool* trans, int* disposal);
    bool checkIfCover(const SavedImage* target, const SavedImage* covered);
    bool checkIfWillBeCleared(const SavedImage* frame);
    void copyLine(unsigned char* dst, const unsigned char* src, const ColorMapObject* cmap, int transparent, int width);
    float getImageDuration(const SavedImage* image);

protected:
    CAImage*        m_pImage;
    unsigned char*  m_pImageData;
    unsigned long   m_pDataSize;
    GifFileType*    m_pGIF;
    int             m_iGIFIndex;
   // DSize          m_pSize;
    unsigned int    m_uPixelsWide;
    unsigned int    m_uPixelsHigh;
    float           m_fDurTime;
};

NS_CC_END

#endif //__CAGIF_H__

