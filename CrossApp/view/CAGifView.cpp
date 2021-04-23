//
//  CAGifView.h
//  CrossApp
//
//  Created by qiaoxin on 15-06-01.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#include "CAGifView.h"
#include "basics/CAScheduler.h"
NS_CC_BEGIN

CAGifView::CAGifView()
:m_iGifIndex(0)
,m_fDurTime(0.0f)
,m_fTimes(1.0f)
,m_pGif(nullptr)
,m_bIsRepeatForever(true)
,m_fLeft(0.0f)
,m_fRight(0.0f)
,m_fTop(0.0f)
,m_fBottom(0.0f)
{

}

CAGifView::~CAGifView()
{
    CC_SAFE_RELEASE(m_pGif);
}

CAGifView* CAGifView::createWithFrame(const CrossApp::DRect &rect)
{
    CAGifView *pRet = new CAGifView();
    if (pRet && pRet->initWithFrame(rect))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CAGifView* CAGifView::createWithCenter(const CrossApp::DRect &rect)
{
    CAGifView * pRet = new CAGifView();
    if (pRet && pRet->initWithCenter(rect))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CAGifView* CAGifView::createWithLayout(const CrossApp::DLayout &layout)
{
    CAGifView * pRet = new CAGifView();
    if (pRet && pRet->initWithLayout(layout))
    {
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
    return this->initWithGif(nullptr);
}

bool CAGifView::initWithGif(CAGif* gif)
{
    if(CAView::init())
    {
        this->setGif(gif);
        return true;
    }
    return false;
}

void CAGifView::setContentSize(const DSize& contentSize)
{
    CAView::setContentSize(contentSize);
    this->updateGifSize();
}

void CAGifView::updateImageRect()
{
    // Don't update Z.
    m_sQuad.bl.vertices = DPoint3D(  m_fLeft,    m_fTop, m_fPointZ);
    m_sQuad.br.vertices = DPoint3D( m_fRight,    m_fTop, m_fPointZ);
    m_sQuad.tl.vertices = DPoint3D(  m_fLeft, m_fBottom, m_fPointZ);
    m_sQuad.tr.vertices = DPoint3D( m_fRight, m_fBottom, m_fPointZ);
}

void CAGifView::setGif(CAGif* gif)
{
    CC_SAFE_RETAIN(gif);
    CC_SAFE_RELEASE(m_pGif);
    m_pGif = gif;
    if (m_pGif)
    {
        this->setImage(m_pGif->getImages().front());
        
        DRect rect = DRectZero;
        rect.size.width = m_pGif->getPixelsWide();
        rect.size.height = m_pGif->getPixelsHigh();
        this->setImageRect(rect);
        
        this->updateGifSize();
        
        if(m_pGif->getImages().size() > 0)
        {
            CAScheduler::getScheduler()->scheduleUpdate(this, 0, !m_bRunning);
        }
    }
}

void CAGifView::updateGifSize()
{
    if(m_pGif)
    {
        DSize viewSize = m_obContentSize;
        DSize imageSize = DSize(m_pGif->getPixelsWide(), m_pGif->getPixelsHigh());
        float viewRatio = viewSize.width / viewSize.height;
        float imageRatio = imageSize.width / imageSize.height;
        
        m_fLeft = 0;
        m_fTop = 0;
        m_fRight = viewSize.width;
        m_fBottom = viewSize.height;
        
        if (imageRatio > viewRatio)
        {
            float height = viewSize.width / imageRatio;
            m_fTop = (viewSize.height - height) / 2;
            m_fBottom = m_fTop + height;
        }
        else if (imageRatio < viewRatio)
        {
            float width = viewSize.height * imageRatio;
            m_fLeft = (viewSize.width - width) / 2;
            m_fRight = m_fLeft + width;
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

void CAGifView::update(float delta)
{
    CC_RETURN_IF(!m_pGif);
    float ldelta = (uint32_t)(delta * 1000) * m_fTimes;
    m_fDurTime += ldelta;
    
    if(m_fDurTime > m_pGif->getDelay())
    {
        ++m_iGifIndex;
        m_iGifIndex %= m_pGif->getImages().size();
        this->setImage(m_pGif->getImageWithIndex(m_iGifIndex));
        m_fDurTime -= m_pGif->getDelay();
        
        DRect rect = DRectZero;
        rect.size.width = m_pGif->getPixelsWide();
        rect.size.height = m_pGif->getPixelsHigh();
        this->setImageRect(rect);

    }
    
    if (!m_bIsRepeatForever && m_iGifIndex >= m_pGif->getImages().size() - 1)
    {
        CAScheduler::getScheduler()->unscheduleUpdate(this);
    }
}

void CAGifView::onEnter()
{
    CAView::onEnter();
    CAScheduler::getScheduler()->resumeTarget(this);
}

void CAGifView::onExit()
{
    CAView::onExit();
    CAScheduler::getScheduler()->pauseTarget(this);
}


NS_CC_END
