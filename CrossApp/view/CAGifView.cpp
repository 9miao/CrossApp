//
//  CAGifView.h
//  CrossApp
//
//  Created by qiaoxin on 15-06-01.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#include "CAGifView.h"
#include "CrossApp.h"
#include "images/gif_lib/gif_lib.h"

NS_CC_BEGIN

CAGifView::CAGifView()
:m_fDurTime(0.0f)
,m_fTimes(1.0f)
,m_pGif(NULL)
,m_bIsRepeatForever(true)
{

}

CAGifView::~CAGifView()
{
    CC_SAFE_DELETE(m_pGif);
}

CAGifView* CAGifView::createWithCenter(const CrossApp::CCRect &rect)
{
    CAGifView * pRet = new CAGifView();
    if (pRet && pRet->init())
    {
        pRet->setCenter(rect);
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CAGifView* CAGifView::createWithFrame(const CrossApp::CCRect &rect)
{
    CAGifView *pRet = new CAGifView();
    if (pRet && pRet->init())
    {
        pRet->setFrame(rect);
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CAGifView* CAGifView::createWithGif(CAGif* gif)
{
    CAGifView *pRet = new CAGifView();
    if (pRet && pRet->initWithGif(gif))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CAGifView::init()
{
    return this->initWithGif(NULL);
}

bool CAGifView::initWithGif(CAGif* gif)
{
    if(CAView::init())
    {
        this->setShaderProgram(CAShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
        this->setGif(gif);
        return true;
    }
    return false;
}

void CAGifView::setGif(CAGif* gif)
{
    CC_SAFE_RETAIN(gif);
    CC_SAFE_DELETE(m_pGif);
    m_pGif = gif;
    if (m_pGif)
    {
        CAImage* image = m_pGif->getImage();
        
        this->setImage(image);
        CCRect rect = CCRectZero;
        rect.size = image->getContentSize();
        this->setImageRect(rect);
        if(m_pGif->getGifImageCounts()>1)
        {
            m_nGifcount = m_pGif->getGifImageCounts();
            CAScheduler::schedule(schedule_selector(CAGifView::updateGif), this, 0);
        }
    }
}

void CAGifView::setTimes(float times)
{
    m_fTimes = times;
}

void CAGifView::setRepeatForever(bool repeatForever)
{
    m_bIsRepeatForever = repeatForever;
}

void CAGifView::updateGif(float delta)
{
    CC_RETURN_IF(!m_pGif);
    float ldelta = (uint32_t)(delta * 1000) * m_fTimes;
    m_fDurTime += ldelta;
    if(m_fDurTime > m_pGif->getImageDuration())
    {
        m_pGif->nextGifImageIndex();
        CAImage* image = m_pGif->getImage();
        this->setImage(image);
        if (image)
        {
            CCRect rect = CCRectZero;
            rect.size = image->getContentSize();
            this->setImageRect(rect);
        }
        m_fDurTime = 0;
    }
    if (!m_bIsRepeatForever && m_pGif->getGifImageIndex() >= m_pGif->getGifImageCounts() - 1)
    {
        CAScheduler::unschedule(schedule_selector(CAGifView::updateGif), this);
    }
}


NS_CC_END
