//
//  CAScale9ImageView.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-6-17.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CAScale9ImageView.h"
#include "CABatchView.h"

NS_CC_BEGIN

CAScale9ImageView::CAScale9ImageView()
: m_fInsetLeft(0)
, m_fInsetTop(0)
, m_fInsetRight(0)
, m_fInsetBottom(0)
, m_pScale9ImageView(NULL)
{
    for (int i=0; i<9; i++)
    {
        m_pImageView[i] = NULL;
    }
}

CAScale9ImageView::~CAScale9ImageView()
{
    for (int i=0; i<9; i++)
    {
        CC_SAFE_RELEASE(m_pImageView[i]);
    }
    CC_SAFE_RELEASE(m_pScale9ImageView);
}

CAScale9ImageView* CAScale9ImageView::create()
{
    CAScale9ImageView *pReturn = new CAScale9ImageView();
    if (pReturn && pReturn->init())
    {
        pReturn->autorelease();
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);
    return NULL;
}

CAScale9ImageView* CAScale9ImageView::createWithImage(CAImage* image)
{
	CAScale9ImageView* pReturn = new CAScale9ImageView();
	if (pReturn && pReturn->initWithImage(image))
	{
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}

bool CAScale9ImageView::init()
{
    return this->initWithImage(NULL);
}

bool CAScale9ImageView::initWithImage(CAImage* image)
{
    if (image)
    {
        this->updateWithImage(CABatchView::createWithImage(image), CCRectZero, CCRectZero);
        this->updatePositions();
    }
    
	return true;
}

CAScale9ImageView* CAScale9ImageView::copy()
{
    CAScale9ImageView* pReturn = new CAScale9ImageView();
    if (pReturn && pReturn->initWithImage(m_pScale9ImageView->getImage()))
    {
        pReturn->updateWithImage(m_pScale9ImageView, this->getBounds(), m_obCapInsets);
        pReturn->autorelease();
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);
    return NULL;
}


bool CAScale9ImageView::updateWithImage(CABatchView* batch, CCRect rect, const CCRect& capInsets)
{
    CAColor4B color = getColor();

    for (int i=0; i<9; i++)
    {
        CC_SAFE_RELEASE(m_pImageView[i]);
    }

    if(m_pScale9ImageView != batch)
    {
        CC_SAFE_RELEASE(m_pScale9ImageView);
        m_pScale9ImageView = batch;
        CC_SAFE_RETAIN(m_pScale9ImageView);
    }
    
    this->removeAllSubviews();
    
    m_obCapInsets = capInsets;
    
    m_obOriginalSize = m_pScale9ImageView->getImageAtlas()->getImage()->getContentSize();
    
    if ( rect.equals(CCRectZero) )
    {
        rect = CCRect(0, 0, m_obOriginalSize.width, m_obOriginalSize.height);
    }
    
    float w = m_obOriginalSize.width;
    float h = m_obOriginalSize.height;

    if ( m_obCapInsets.equals(CCRectZero) )
    {
        m_obCapInsets = CCRect(w/3, h/3, w/3, h/3);
    }

    const float lenghtX1 = m_obCapInsets.origin.x;
    const float lenghtX2 = m_obCapInsets.size.width;
    const float lenghtX3 = w - lenghtX1 - lenghtX2;
    const float lenghtY1 = m_obCapInsets.origin.y;
    const float lenghtY2 = m_obCapInsets.size.height;
    const float lenghtY3 = h - lenghtY1 - lenghtY2;
    
    m_rFrame[0] = CCRect(0, 0, lenghtX1, lenghtY1);
    m_rFrame[1] = CCRect(lenghtX1, 0, lenghtX2, lenghtY1);
    m_rFrame[2] = CCRect(lenghtX1 + lenghtX2, 0, lenghtX3, lenghtY1);
    m_rFrame[3] = CCRect(0, lenghtY1, lenghtX1, lenghtY2);
    m_rFrame[4] = CCRect(lenghtX1, lenghtY1, lenghtX2, lenghtY2);
    m_rFrame[5] = CCRect(lenghtX1 + lenghtX2, lenghtY1, lenghtX3, lenghtY2);
    m_rFrame[6] = CCRect(0, lenghtY1 + lenghtY2, lenghtX1, lenghtY3);
    m_rFrame[7] = CCRect(lenghtX1, lenghtY1 + lenghtY2, lenghtX2, lenghtY3);
    m_rFrame[8] = CCRect(lenghtX1 + lenghtX2, lenghtY1 + lenghtY2, lenghtX3, lenghtY3);
       
    // Centre
    m_pImageView[4] = new CAImageView();
    m_pImageView[4]->initWithImage(m_pScale9ImageView->getImage());
    m_pScale9ImageView->insertSubview(m_pImageView[4], 0);
    
    // Top
    m_pImageView[1] = new CAImageView();
    m_pImageView[1]->initWithImage(m_pScale9ImageView->getImage());
    m_pScale9ImageView->insertSubview(m_pImageView[1], 1);
    
    // Bottom
    m_pImageView[7] = new CAImageView();
    m_pImageView[7]->initWithImage(m_pScale9ImageView->getImage());
    m_pScale9ImageView->insertSubview(m_pImageView[7], 1);
    
    // Left
    m_pImageView[3] = new CAImageView();
    m_pImageView[3]->initWithImage(m_pScale9ImageView->getImage());
    m_pScale9ImageView->insertSubview(m_pImageView[3], 1);
    
    // Right
    m_pImageView[5] = new CAImageView();
    m_pImageView[5]->initWithImage(m_pScale9ImageView->getImage());
    m_pScale9ImageView->insertSubview(m_pImageView[5], 1);
    
    // Top left
    m_pImageView[0] = new CAImageView();
    m_pImageView[0]->initWithImage(m_pScale9ImageView->getImage());
    m_pScale9ImageView->insertSubview(m_pImageView[0], 2);
    
    // Top right
    m_pImageView[2] = new CAImageView();
    m_pImageView[2]->initWithImage(m_pScale9ImageView->getImage());
    m_pScale9ImageView->insertSubview(m_pImageView[2], 2);
    
    // Bottom left
    m_pImageView[6] = new CAImageView();
    m_pImageView[6]->initWithImage(m_pScale9ImageView->getImage());
    m_pScale9ImageView->insertSubview(m_pImageView[6], 2);
    
    // Bottom right
    m_pImageView[8] = new CAImageView();
    m_pImageView[8]->initWithImage(m_pScale9ImageView->getImage());
    m_pScale9ImageView->insertSubview(m_pImageView[8], 2);
    

    for (int i=0; i<9; i++)
    {
        m_pImageView[i]->setImageRect(m_rFrame[i], false, m_rFrame[i].size);
        m_pImageView[i]->setAnchorPoint(CCPointZero);
    }

    this->addSubview(m_pScale9ImageView);

    this->setContentSize(rect.size);
    
    this->setColor(color);
    
    return true;
}

void CAScale9ImageView::updatePositions()
{
    for (int i=0; i<9; i++)
    {
        CC_RETURN_IF(m_pImageView[i] == NULL);
    }

    CCSize size = this->m_obContentSize;
    
    CCRect newRect[9];
    
    for (int i=0; i<9; i++)
    {
        newRect[i] = m_rFrame[i];
    }
    
    if (size.width > m_obOriginalSize.width)
    {
        const float lenghtX1 = m_rFrame[0].size.width;
        const float lenghtX3 = m_rFrame[2].size.width;
        const float lenghtX2 = size.width - lenghtX1 - lenghtX3;
        
        newRect[1].origin.x =
        newRect[4].origin.x =
        newRect[7].origin.x = lenghtX1;
        
        newRect[1].size.width =
        newRect[4].size.width =
        newRect[7].size.width = lenghtX2;
        
        newRect[2].origin.x =
        newRect[5].origin.x =
        newRect[8].origin.x = lenghtX1 + lenghtX2;
        
        newRect[2].size.width =
        newRect[5].size.width =
        newRect[8].size.width = lenghtX3;
    }
    else
    {
        float scaleX = size.width / m_obOriginalSize.width;
        for (int i=0; i<9; i++)
        {
            newRect[i].origin.x *= scaleX;
            newRect[i].size.width *= scaleX;
        }
    }
    
    if (size.height > m_obOriginalSize.height)
    {
        const float lenghtY1 = m_rFrame[0].size.height;
        const float lenghtY3 = m_rFrame[6].size.height;
        const float lenghtY2 = size.height - lenghtY1 - lenghtY3;
        
        newRect[3].origin.y =
        newRect[4].origin.y =
        newRect[5].origin.y = lenghtY1;
        
        newRect[3].size.height =
        newRect[4].size.height =
        newRect[5].size.height = lenghtY2;
        
        newRect[6].origin.y =
        newRect[7].origin.y =
        newRect[8].origin.y = lenghtY1 + lenghtY2;
        
        newRect[6].size.height =
        newRect[7].size.height =
        newRect[8].size.height = lenghtY3;
    }
    else
    {
        float scaleY = size.height / m_obOriginalSize.height;
        for (int i=0; i<9; i++)
        {
            newRect[i].origin.y *= scaleY;
            newRect[i].size.height *= scaleY;
        }

    }
    
    for (int i=0; i<9; i++)
    {
        m_pImageView[i]->setFrameOrigin(newRect[i].origin);
        m_pImageView[i]->setScaleX(newRect[i].size.width / m_rFrame[i].size.width);
        m_pImageView[i]->setScaleY(newRect[i].size.height / m_rFrame[i].size.height);
    }
}

void CAScale9ImageView::setContentSize(const CCSize &size)
{
    CAView::setContentSize(size);
    
    this->updatePositions();
}

void CAScale9ImageView::setCapInsets(const CCRect& capInsets)
{
    do
    {
        CC_BREAK_IF(m_pScale9ImageView == NULL);
        this->updateWithImage(m_pScale9ImageView, this->getBounds(), capInsets);
    }
    while (0);
    
}

void CAScale9ImageView::updateCapInset()
{
    CCRect insets;
    if (   m_fInsetLeft == 0
        && m_fInsetTop == 0
        && m_fInsetRight == 0
        && m_fInsetBottom == 0)
    {
        insets = CCRectZero;
    }
    else
    {
        insets = CCRect(m_fInsetLeft,
                            m_fInsetTop,
                            m_obOriginalSize.width-m_fInsetLeft-m_fInsetRight,
                            m_obOriginalSize.height-m_fInsetTop-m_fInsetBottom);
    }
    this->setCapInsets(insets);
}

void CAScale9ImageView::updateDisplayedColor(const cocos2d::CAColor4B &color)
{
    CAView::updateDisplayedColor(color);
    setColor(color);
}

void CAScale9ImageView::setInsetLeft(float insetLeft)
{
    this->m_fInsetLeft = insetLeft;
    this->updateCapInset();
}

void CAScale9ImageView::setInsetTop(float insetTop)
{
    this->m_fInsetTop = insetTop;
    this->updateCapInset();
}

void CAScale9ImageView::setInsetRight(float insetRight)
{
    this->m_fInsetRight = insetRight;
    this->updateCapInset();
}

void CAScale9ImageView::setInsetBottom(float insetBottom)
{
    this->m_fInsetBottom = insetBottom;
    this->updateCapInset();
}

void CAScale9ImageView::setColor(const CAColor4B& color)
{
    m_pScale9ImageView->setColor(color);
    
    CCObject* child;
    CCArray* children = this->getSubviews();
    CCARRAY_FOREACH(children, child)
    {
        CCRGBAProtocol* pNode = dynamic_cast<CCRGBAProtocol*>(child);
        if (pNode)
        {
            pNode->setColor(color);
        }
    }
}

const CAColor4B& CAScale9ImageView::getColor()
{
	return m_pScale9ImageView ? m_pScale9ImageView->getColor() : CAColor_white;
}


bool CAScale9ImageView::initWithImage(CAImage* image, CCRect rect, CCRect capInsets)
{
    if (image)
    {
        this->updateWithImage(CABatchView::createWithImage(image), rect, capInsets);
        this->updatePositions();
    }
    
	return true;
}

CAScale9ImageView* CAScale9ImageView::createWithImage(CAImage* image, CCRect rect, CCRect capInsets)
{
	CAScale9ImageView* pReturn = new CAScale9ImageView();
	if (pReturn && pReturn->initWithImage(image, rect, capInsets))
	{
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}

bool CAScale9ImageView::initWithImage(CAImage* image, CCRect rect)
{
	bool pReturn = this->initWithImage(image, rect, CCRectZero);
	return pReturn;
}

CAScale9ImageView* CAScale9ImageView::createWithImage(CAImage* image, CCRect rect)
{
	CAScale9ImageView* pReturn = new CAScale9ImageView();
	if (pReturn && pReturn->initWithImage(image, rect))
	{
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}

bool CAScale9ImageView::initWithImage(CCRect capInsets, CAImage* image)
{
	bool pReturn = this->initWithImage(image, CCRectZero, capInsets);
	return pReturn;
}

CAScale9ImageView* CAScale9ImageView::createWithImage(CCRect capInsets, CAImage* image)
{
	CAScale9ImageView* pReturn = new CAScale9ImageView();
	if (pReturn && pReturn->initWithImage(capInsets, image))
	{
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}

NS_CC_END
