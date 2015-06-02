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
,m_pImage(NULL)
,m_pGif(NULL)
,m_bIsRepeat(true)
,m_bUpdateByImageViewScaleType(false)
{

}

CAGifView::~CAGifView()
{
    CC_SAFE_DELETE(m_pGif);
    CC_SAFE_RELEASE(m_pImage);
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
        pRet->setFrame(CCRect(0,0,gif->getWidth(), gif->getHeith()));
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
        m_pImage = new CAImage();
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
        CAImage* texture = createImage(0);
        this->setShaderProgram(CAShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
        this->setImage(texture);
        CCRect rect = CCRectZero;
        rect.size = texture->getContentSize();
        this->setVertexRect(rect);
        this->updateByImageViewScaleType();
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

void CAGifView::setRepeat(bool repeat)
{
    m_bIsRepeat = repeat;
}

CAImage* CAGifView::createImage(int index)
{
    if(m_pGif == NULL)
    {
        return NULL;
    }
    m_pGif->setGifImageWithIndex(index);
    m_pImage->initWithRawData(m_pGif->getImageData(), CAImage::PixelFormat_RGBA8888, m_pGif->getWidth(), m_pGif->getHeith());
    return m_pImage;
}

void CAGifView::updateGif(float delta)
{
    CC_RETURN_IF(!m_pGif);
    float ldelta = (uint32_t)(delta * 1000) * m_fTimes;
    m_fDurTime += ldelta;
    if(m_fDurTime > m_pGif->getImageDuration())
    {
        CAImage* texture = createImage(m_pGif->getGifImageNextIndex());
        this->setShaderProgram(CAShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
        this->setImage(texture);
        if (texture)
        {
            CCRect rect = CCRectZero;
            rect.size = texture->getContentSize();
            this->setVertexRect(rect);
            this->updateByImageViewScaleType();
        }
        m_fDurTime = 0;
    }
    if (!m_bIsRepeat&&m_pGif->getGifImageIndex()>=m_pGif->getGifImageCounts()-1) {
        CAScheduler::unschedule(schedule_selector(CAGifView::updateGif), this);
    }
}

void CAGifView::updateByImageViewScaleType()
{
    CC_RETURN_IF(m_pobImage == NULL);
    CC_RETURN_IF(m_bUpdateByImageViewScaleType);
    m_bUpdateByImageViewScaleType = true;
    
    CCSize viewSize = m_obContentSize;
    CCRect rect = m_obRect;
    CCSize imageSize = m_obRect.size;
    float viewRatio = viewSize.width / viewSize.height;
    float imageRatio = imageSize.width / imageSize.height;
    
    switch (m_eImageViewScaleType)
    {
        case CAImageViewScaleTypeFitViewByHorizontal:
        {
            viewSize.width = viewSize.height * imageRatio;
        }
            break;
        case CAImageViewScaleTypeFitViewByVertical:
        {
            viewSize.height = viewSize.width / imageRatio;
        }
            break;
        default:
            break;
    }
    
    m_fLeft = 0;
    m_fTop = 0;
    m_fRight = viewSize.width;
    m_fBottom = viewSize.height;
    
    switch (m_eImageViewScaleType)
    {
        case CAImageViewScaleTypeFitImageCrop:
        {
            if (imageRatio > viewRatio)
            {
                rect.size.width = imageSize.height * viewRatio;
                rect.origin.x = (imageSize.width - rect.size.width) / 2;
            }
            else if (imageRatio < viewRatio)
            {
                rect.size.height = imageSize.width / viewRatio;
                rect.origin.y = (imageSize.height - rect.size.height) / 2;
            }
        }
            break;
        case CAImageViewScaleTypeFitImageInside:
        {
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
            break;
        default:
            break;
    }
    this->setImageRect(rect, false, viewSize);
    m_bUpdateByImageViewScaleType = false;
}


NS_CC_END
