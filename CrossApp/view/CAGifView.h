//
//  CAGifView.h
//  CrossApp
//
//  Created by qiaoxin on 15-06-01.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CAGIF_VIEW_H__
#define __CAGIF_VIEW_H__

#include "CrossApp.h"
#include "../view/CAImageView.h"

NS_CC_BEGIN

class CAGif;
class CAImage;

class CC_DLL CAGifView : public CAView
{
public:
    CAGifView();
    ~CAGifView();
public:
    static CAGifView* createWithFrame(const CCRect& rect);
    static CAGifView* createWithCenter(const CCRect& rect);
    static CAGifView* createWithGif(CAGif* gif);
    virtual bool init();
    virtual bool initWithGif(CAGif* gif);
    void setGif(CAGif* gif);
    void setTimes(float times);
    void setRepeat(bool repeat);
    virtual void updateByImageViewScaleType();

protected:
    virtual CAImage* createImage(int index);
    virtual void updateGif(float delta);
private:
    CAGif*      m_pGif;
    CAImage*    m_pImage;
    int         m_nGifcount;
    bool        m_bIsRepeat;
    float       m_fDurTime;
    float       m_fTimes;
    
    bool        m_bUpdateByImageViewScaleType;
    
    CC_SYNTHESIZE_PASS_BY_REF(CAImageViewScaleType, m_eImageViewScaleType, ImageViewScaleType);
    float m_fLeft;
    
    float m_fRight;
    
    float m_fTop;
    
    float m_fBottom;

};

NS_CC_END

#endif //__CAGIFVIEW_H__

