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
,m_fLeft(0.0f)
,m_fRight(0.0f)
,m_fTop(0.0f)
,m_fBottom(0.0f)
{

}

CAGifView::~CAGifView()
{
    CC_SAFE_DELETE(m_pGif);
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

CAGifView* CAGifView::createWithLayout(const CrossApp::DRectLayout &layout)
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
    return this->initWithGif(NULL);
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
    m_sQuad.bl.vertices = vertex3(  m_fLeft,    m_fTop, m_fVertexZ);
    m_sQuad.br.vertices = vertex3( m_fRight,    m_fTop, m_fVertexZ);
    m_sQuad.tl.vertices = vertex3(  m_fLeft, m_fBottom, m_fVertexZ);
    m_sQuad.tr.vertices = vertex3( m_fRight, m_fBottom, m_fVertexZ);
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
        DRect rect = DRectZero;
        rect.size = image->getContentSize();
        this->setImageRect(rect);
        if(m_pGif->getGifImageCounts()>1)
        {
            m_nGifcount = m_pGif->getGifImageCounts();
            CAScheduler::schedule(schedule_selector(CAGifView::updateGif), this, 0);
        }
        this->updateGifSize();
    }
}

void CAGifView::updateGifSize()
{
    if(m_pGif)
    {
        DSize viewSize = m_obContentSize;
        DSize imageSize = DSize(m_pGif->getWidth(),m_pGif->getHeight());
        float viewRatio = viewSize.width / viewSize.height;
        float imageRatio = imageSize.width / imageSize.height;
        
        m_fLeft = 0;
        m_fTop = 0;
        m_fRight = viewSize.width;
        m_fBottom = viewSize.height;
        
        if (imageRatio > viewRatio)
        {
            m_fTop = (viewSize.height - viewSize.width / imageRatio) / 2;
            m_fBottom = m_fTop + viewSize.width / imageRatio;
        }
        else if (imageRatio < viewRatio)
        {
            m_fLeft = (viewSize.width - viewSize.height * imageRatio) / 2;
            m_fRight = m_fLeft + viewSize.height * imageRatio;
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
            DRect rect = DRectZero;
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
