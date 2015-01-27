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

class CC_DLL CAScale9Image: public CAView
{
    
public:
    
    using CAView::setImage;
    
    using CAView::setImageRect;
};

CAScale9ImageView::CAScale9ImageView()
: m_fInsetLeft(0)
, m_fInsetTop(0)
, m_fInsetRight(0)
, m_fInsetBottom(0)
, m_obCapInsets(CCRectZero)
, m_obOriginalSize(CCSizeZero)
, m_pScale9ImageView(NULL)
{
    m_obFrameRect = CCRectZero;
    memset(m_pImageView, NULL, sizeof(m_pImageView));
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

CAScale9ImageView* CAScale9ImageView::createWithFrame(const CCRect& rect)
{
	CAScale9ImageView* pReturn = new CAScale9ImageView();
	if (pReturn && pReturn->CAView::initWithFrame(rect))
	{
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}

CAScale9ImageView* CAScale9ImageView::createWithCenter(const CCRect& rect)
{
    CAScale9ImageView* pReturn = new CAScale9ImageView();
	if (pReturn && pReturn->CAView::initWithCenter(rect))
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
        this->setImage(image);
    }
    
	return true;
}

CAView* CAScale9ImageView::copy()
{
    CAScale9ImageView* pReturn = CAScale9ImageView::createWithImage(this->getImage());
    pReturn->setColor(this->getColor());
    if (m_bFrame)
    {
        pReturn->setFrame(this->getFrame());
    }
    else
    {
        pReturn->setCenter(this->getCenter());
    }
    return pReturn;
}


bool CAScale9ImageView::updateWithImage(CABatchView* batch, CCRect rect, const CCRect& capInsets)
{
    CAColor4B color = getColor();

    this->removeSubview(m_pScale9ImageView);
    
    for (int i=0; i<9; i++)
    {
        CC_SAFE_RELEASE_NULL(m_pImageView[i]);
    }

    if(m_pScale9ImageView != batch)
    {
        CC_SAFE_RELEASE(m_pScale9ImageView);
        CC_SAFE_RETAIN(batch);
        m_pScale9ImageView = batch;
    }
    
    m_obCapInsets = capInsets;
    
    m_obOriginalSize = m_pScale9ImageView->getImageAtlas()->getImage()->getContentSize();
    
    if ( rect.equals(CCRectZero) )
    {
        rect = CCRect(0, 0, m_obOriginalSize.width, m_obOriginalSize.height);
    }
    
    this->updateCapInset();
    
    CAScale9Image* imageView[9] = {};
    
    for (int i=0; i<9; i++)
    {
        imageView[i] = new CAScale9Image();
        imageView[i]->setImage(m_pScale9ImageView->getImage());
    }
    
    
    // Centre
    m_pScale9ImageView->insertSubview(imageView[4], 0);
    
    // Top
    m_pScale9ImageView->insertSubview(imageView[1], 1);
    
    // Bottom
    m_pScale9ImageView->insertSubview(imageView[7], 1);
    
    // Left
    m_pScale9ImageView->insertSubview(imageView[3], 1);
    
    // Right
    m_pScale9ImageView->insertSubview(imageView[5], 1);
    
    // Top left
    m_pScale9ImageView->insertSubview(imageView[0], 2);
    
    // Top right
    m_pScale9ImageView->insertSubview(imageView[2], 2);
    
    // Bottom left
    m_pScale9ImageView->insertSubview(imageView[6], 2);
    
    // Bottom right
    m_pScale9ImageView->insertSubview(imageView[8], 2);
    
    for (int i=0; i<9; i++)
    {
        imageView[i]->setImageRect(m_rFrame[i], false, m_rFrame[i].size);
        imageView[i]->setAnchorPoint(CCPointZero);
        m_pImageView[i] = imageView[i];
    }

    this->addSubview(m_pScale9ImageView);

    this->setBounds(rect);
    
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

void CAScale9ImageView::draw(void)
{

}

void CAScale9ImageView::setCapInsets(const CCRect& capInsets)
{
    CC_RETURN_IF(m_obCapInsets.equals(capInsets));
    m_obCapInsets = capInsets;
    this->updateCapInset();
}

void CAScale9ImageView::updateCapInset()
{
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
    
    for (int i=0; i<9; i++)
    {
        CC_RETURN_IF(m_pImageView[i] == NULL);
    }
    
    if (m_pScale9ImageView)
    {
        for (int i=0; i<9; i++)
        {
            if (CAScale9Image* imageView = dynamic_cast<CAScale9Image*>(m_pImageView[i]))
            {
                imageView->setImageRect(m_rFrame[i], false, m_rFrame[i].size);
            }
        }
        this->updatePositions();
    }
}

void CAScale9ImageView::updateDisplayedColor(const CrossApp::CAColor4B &color)
{
    CAView::updateDisplayedColor(color);
    setColor(color);
}

void CAScale9ImageView::setInsetLeft(float insetLeft)
{
    this->m_fInsetLeft = insetLeft;
    CCRect insets = m_obCapInsets;
    if (m_fInsetLeft != 0 && m_fInsetTop != 0 && m_fInsetRight != 0 && m_fInsetBottom != 0)
    {
        insets = CCRect(m_fInsetLeft,
                        m_fInsetTop,
                        m_obOriginalSize.width-m_fInsetLeft-m_fInsetRight,
                        m_obOriginalSize.height-m_fInsetTop-m_fInsetBottom);
        this->updateCapInset();
    }
}

void CAScale9ImageView::setInsetTop(float insetTop)
{
    this->m_fInsetTop = insetTop;
    CCRect insets = m_obCapInsets;
    if (m_fInsetLeft != 0 && m_fInsetTop != 0 && m_fInsetRight != 0 && m_fInsetBottom != 0)
    {
        insets = CCRect(m_fInsetLeft,
                        m_fInsetTop,
                        m_obOriginalSize.width-m_fInsetLeft-m_fInsetRight,
                        m_obOriginalSize.height-m_fInsetTop-m_fInsetBottom);
        this->updateCapInset();
    }
}

void CAScale9ImageView::setInsetRight(float insetRight)
{
    this->m_fInsetRight = insetRight;
    CCRect insets = m_obCapInsets;
    if (m_fInsetLeft != 0 && m_fInsetTop != 0 && m_fInsetRight != 0 && m_fInsetBottom != 0)
    {
        insets = CCRect(m_fInsetLeft,
                        m_fInsetTop,
                        m_obOriginalSize.width-m_fInsetLeft-m_fInsetRight,
                        m_obOriginalSize.height-m_fInsetTop-m_fInsetBottom);
        this->updateCapInset();
    }
}

void CAScale9ImageView::setInsetBottom(float insetBottom)
{
    this->m_fInsetBottom = insetBottom;
    CCRect insets = m_obCapInsets;
    if (m_fInsetLeft != 0 && m_fInsetTop != 0 && m_fInsetRight != 0 && m_fInsetBottom != 0)
    {
        insets = CCRect(m_fInsetLeft,
                        m_fInsetTop,
                        m_obOriginalSize.width-m_fInsetLeft-m_fInsetRight,
                        m_obOriginalSize.height-m_fInsetTop-m_fInsetBottom);
        this->updateCapInset();
    }
}

void CAScale9ImageView::setColor(const CAColor4B& color)
{
    CAView::setColor(color);
    if (m_pScale9ImageView)
    {
        CAObject* child;
        CCArray* children = m_pScale9ImageView->getSubviews();
        CCARRAY_FOREACH(children, child)
        {
            CARGBAProtocol* pNode = dynamic_cast<CARGBAProtocol*>(child);
            if (pNode)
            {
                pNode->setColor(color);
            }
        }
    }
}

const CAColor4B& CAScale9ImageView::getColor()
{
	return CAView::getColor();
}

void CAScale9ImageView::setImage(CrossApp::CAImage *image)
{
    CC_RETURN_IF(m_pobImage == image);
    CC_SAFE_RETAIN(image);
    CC_SAFE_RELEASE_NULL(m_pobImage);
    m_pobImage = image;
    if (m_pobImage)
    {
        this->updateWithImage(CABatchView::createWithImage(image), this->getBounds(), m_obCapInsets);
        this->updatePositions();
    }
}

CAImage* CAScale9ImageView::getImage()
{
    return m_pScale9ImageView->getImage();
}

void CAScale9ImageView::updateDisplayedAlpha(float parentOpacity)
{
    if (m_pScale9ImageView)
    {
        m_pScale9ImageView->updateDisplayedAlpha(parentOpacity);
    }
}

bool CAScale9ImageView::initWithFrame(const CCRect& rect, const CAColor4B& color4B)
{
    return CAView::initWithFrame(rect);
}

bool CAScale9ImageView::initWithCenter(const CCRect& rect, const CAColor4B& color4B)
{
    return CAView::initWithCenter(rect);
}



NS_CC_END
