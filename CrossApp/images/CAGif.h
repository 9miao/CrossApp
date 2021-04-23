//
//  CAGif.h
//  CrossApp
//
//  Created by qiaoxin on 15-06-01.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CAGIF_H__
#define __CAGIF_H__

#include "images/CAImage.h"
#include "basics/CASTLContainer.h"
#include "gif_lib/gif_lib.h"

NS_CC_BEGIN

class CC_DLL CAGif : public CAObject
{
public:
    
    CAGif();
    
    virtual ~CAGif();
    
    static CAGif* create(const std::string& filePath);
    
    CC_DEPRECATED_ATTRIBUTE static CAGif* createWithFilePath(const std::string& filePath) { return CAGif::create(filePath); }
    
    static CAGif* createWithData(CAData* data);

    bool initWithFilePath(const std::string& filePath);
    
    bool initWithData(CAData* data);
    
    CAImage* getImageWithIndex(unsigned int index);
    
    CC_SYNTHESIZE_READONLY(CAVector<CAImage*>, m_vImages, Images);
    
    CC_SYNTHESIZE_READONLY(float, m_fDelay, Delay);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_uPixelsWide, PixelsWide);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_uPixelsHigh, PixelsHigh);

    static const std::map<std::string, CAGif*>& getGIFs();
    
protected:
    
    void getTransparencyAndDisposalMethod(const SavedImage* frame, bool* trans, int* disposal);
    
    bool checkIfCover(const SavedImage* target, const SavedImage* covered);
    
    bool checkIfWillBeCleared(const SavedImage* frame);
    
    void copyLine(unsigned char* dst, const unsigned char* src, const ColorMapObject* cmap, int transparent, int width);
    
    float getImageDelay(const SavedImage* image);

    CAImage* getImage(unsigned int index);
    
protected:

    CAData*         m_pGifData;
    
    unsigned char*  m_pData;
    
    GifFileType*    m_pGIF;
    
    std::string     m_sFilePath;
};

NS_CC_END

#endif //__CAGIF_H__

