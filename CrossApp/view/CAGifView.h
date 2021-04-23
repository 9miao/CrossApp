//
//  CAGifView.h
//  CrossApp
//
//  Created by qiaoxin on 15-06-01.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CAGIF_VIEW_H__
#define __CAGIF_VIEW_H__

#include "CAView.h"
#include "images/CAGif.h"

NS_CC_BEGIN

class CC_DLL CAGifView : public CAView
{
public:
    
    CAGifView();
    
    ~CAGifView();
    
public:
    
    static CAGifView* createWithFrame(const DRect& rect);
    
    static CAGifView* createWithCenter(const DRect& rect);
    
    static CAGifView* createWithLayout(const DLayout& layout);
    
    static CAGifView* createWithGif(CAGif* gif);
    
    virtual bool init();
    
    virtual bool initWithGif(CAGif* gif);

    void setGif(CAGif* gif);
    
    CAGif* getGif() { return m_pGif; }
    
    void setTimes(float times);
    
    void setRepeatForever(bool repeatForever);
    
    bool isRepeatForever() { return m_bIsRepeatForever; }

    virtual void onEnter();
    
    virtual void onExit();
    
protected:
    
    virtual void updateImageRect();
    
    virtual void update(float delta);
    
    virtual void setContentSize(const DSize& contentSize);
    
    void updateGifSize();
    
private:
    
    CAGif* m_pGif;

    bool m_bIsRepeatForever;
    
    int m_iGifIndex;
    
    float m_fDurTime;
    
    float m_fTimes;
    
    float m_fLeft;
    
    float m_fRight;
    
    float m_fTop;
    
    float m_fBottom;

};

NS_CC_END

#endif //__CAGIFVIEW_H__

