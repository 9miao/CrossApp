/****************************************************************************
Copyright (c) 2012 cocos2d-x.org

http://www.cocos2d-x.org

Created by Jung Sang-Taik on 12. 3. 16..
Copyright (c) 2012 Neofect. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CAScale9ImageView.h"
#include "CABatchView.h"

NS_CC_BEGIN

enum positions
{
    pCentre = 0,
    pTop,
    pLeft,
    pRight,
    pBottom,
    pTopRight,
    pTopLeft,
    pBottomRight,
    pBottomLeft
};

CAScale9ImageView::CAScale9ImageView()
: m_insetLeft(0)
, m_insetTop(0)
, m_insetRight(0)
, m_insetBottom(0)
, m_bSpritesGenerated(false)
, m_positionsAreDirty(false)
, _scale9Image(NULL)
, _topLeft(NULL)
, _top(NULL)
, _topRight(NULL)
, _left(NULL)
, _centre(NULL)
, _right(NULL)
, _bottomLeft(NULL)
, _bottom(NULL)
, _bottomRight(NULL)
, _opacityModifyRGB(false)
, _opacity(255)
, _color(CAColor_white)
{

}

CAScale9ImageView::~CAScale9ImageView()
{
    CC_SAFE_RELEASE(_topLeft);
    CC_SAFE_RELEASE(_top);
    CC_SAFE_RELEASE(_topRight);
    CC_SAFE_RELEASE(_left);
    CC_SAFE_RELEASE(_centre);
    CC_SAFE_RELEASE(_right);
    CC_SAFE_RELEASE(_bottomLeft);
    CC_SAFE_RELEASE(_bottom);
    CC_SAFE_RELEASE(_bottomRight);
    CC_SAFE_RELEASE(_scale9Image);
}

bool CAScale9ImageView::init()
{
    return this->initWithImage(NULL, CCRectZero, CCRectZero);
}


#define    TRANSLATE_X(x, y, xtranslate) \
    x+=xtranslate;                       \

#define    TRANSLATE_Y(x, y, ytranslate) \
    y+=ytranslate;                       \

bool CAScale9ImageView::updateWithImage(CABatchView* batch, CCRect rect, CCRect capInsets)
{
    CAColor4B color = getColor();

    // Release old sprites
    this->removeAllSubviews();

    CC_SAFE_RELEASE(this->_centre);
    CC_SAFE_RELEASE(this->_top);
    CC_SAFE_RELEASE(this->_topLeft);
    CC_SAFE_RELEASE(this->_topRight);
    CC_SAFE_RELEASE(this->_left);
    CC_SAFE_RELEASE(this->_right);
    CC_SAFE_RELEASE(this->_bottomLeft);
    CC_SAFE_RELEASE(this->_bottom);
    CC_SAFE_RELEASE(this->_bottomRight);

    if(this->_scale9Image != batch)
    {
        CC_SAFE_RELEASE(this->_scale9Image);
        _scale9Image = batch;
        CC_SAFE_RETAIN(_scale9Image);
    }
    
    this->removeAllSubviews();
    m_capInsets = capInsets;
    
    // If there is no given rect
    if ( rect.equals(CCRectZero) )
    {
        // Get the Image size as original
        CCSize textureSize = _scale9Image->getImageAtlas()->getImage()->getContentSize();
        
        rect = CCRectMake(0, 0, textureSize.width, textureSize.height);
    }
    
    this->setContentSize(rect.size);
    
    // Set the given rect's size as original size
    m_spriteRect = rect;
    m_originalSize = rect.size;
    m_capInsetsInternal = capInsets;
    
    float w = rect.size.width;
    float h = rect.size.height;

    // If there is no specified center region
    if ( m_capInsetsInternal.equals(CCRectZero) )
    {
        // CCLog("... cap insets not specified : using default cap insets ...");
        m_capInsetsInternal = CCRectMake(w/3, h/3, w/3, h/3);
    }

    float left_w = m_capInsetsInternal.origin.x-0.5;
    float center_w = m_capInsetsInternal.size.width;
    float right_w = rect.size.width - (left_w + center_w);

    float top_h = m_capInsetsInternal.origin.y;
    float center_h = m_capInsetsInternal.size.height;
    float bottom_h = rect.size.height - (top_h + center_h);

    // calculate rects

    // ... top row
    float x = 0.0;
    float y = 0.0;

    // top left
    CCRect leftbottombounds = CCRectMake(x, y,
                                      left_w, top_h);

    // top center
    TRANSLATE_X(x, y, left_w);
    CCRect centerbottombounds = CCRectMake(x, y,
                                        center_w, top_h);
        
    // top right
    TRANSLATE_X(x, y, center_w);
    CCRect rightbottombounds = CCRectMake(x, y,
                                       right_w, top_h);

    // ... center row
    x = 0.0;
    y = 0.0;
    TRANSLATE_Y(x, y, bottom_h);

    // center left
    CCRect leftcenterbounds = CCRectMake(x, y,
                                         left_w, center_h);

    // center center
    TRANSLATE_X(x, y, left_w);
    CCRect centerbounds = CCRectMake(x, y,
                                     center_w, center_h);

    // center right
    TRANSLATE_X(x, y, center_w);
    CCRect rightcenterbounds = CCRectMake(x, y,
                                          right_w, center_h);

    // ... bottom row
    x = 0.0;
    y = 0.0;
    TRANSLATE_Y(x, y, bottom_h);
    TRANSLATE_Y(x, y, center_h);

    // bottom left
    CCRect lefttopbounds = CCRectMake(x, y,
                                         left_w, bottom_h);

    // bottom center
    TRANSLATE_X(x, y, left_w);
    CCRect centertopbounds = CCRectMake(x, y,
                                           center_w, bottom_h);

    // bottom right
    TRANSLATE_X(x, y, center_w);
    CCRect righttopbounds = CCRectMake(x, y,
                                          right_w, bottom_h);

    CCAffineTransform t = CCAffineTransformMakeIdentity();
    t = CCAffineTransformTranslate(t, rect.origin.x, rect.origin.y);
    
    centerbounds = CCRectApplyAffineTransform(centerbounds, t);
    rightbottombounds = CCRectApplyAffineTransform(rightbottombounds, t);
    leftbottombounds = CCRectApplyAffineTransform(leftbottombounds, t);
    righttopbounds = CCRectApplyAffineTransform(righttopbounds, t);
    lefttopbounds = CCRectApplyAffineTransform(lefttopbounds, t);
    rightcenterbounds = CCRectApplyAffineTransform(rightcenterbounds, t);
    leftcenterbounds = CCRectApplyAffineTransform(leftcenterbounds, t);
    centerbottombounds = CCRectApplyAffineTransform(centerbottombounds, t);
    centertopbounds = CCRectApplyAffineTransform(centertopbounds, t);
    
    // Centre
    _centre = new CAImageView();
    _centre->initWithImage(_scale9Image->getImage());
    _centre->setImageRect(centerbounds, false, centerbounds.size);
    _centre->setTag(pCentre);
    _scale9Image->insertSubview(_centre, 0);
    
    // Top
    _top = new CAImageView();
    _top->initWithImage(_scale9Image->getImage());
    _top->setImageRect(centertopbounds, false, centertopbounds.size);
    _top->setTag(pTop);
    _scale9Image->insertSubview(_top, 1);
    
    // Bottom
    _bottom = new CAImageView();
    _bottom->initWithImage(_scale9Image->getImage());
    _bottom->setImageRect(centerbottombounds, false, centerbottombounds.size);
    _bottom->setTag(pBottom);
    _scale9Image->insertSubview(_bottom, 1);
    
    // Left
    _left = new CAImageView();
    _left->initWithImage(_scale9Image->getImage());
    _left->setImageRect(leftcenterbounds, false, leftcenterbounds.size);
    _left->setTag(pLeft);
    _scale9Image->insertSubview(_left, 1);
    
    // Right
    _right = new CAImageView();
    _right->initWithImage(_scale9Image->getImage());
    _right->setImageRect(rightcenterbounds, false, rightcenterbounds.size);
    _right->setTag(pRight);
    _scale9Image->insertSubview(_right, 1);
    
    // Top left
    _topLeft = new CAImageView();
    _topLeft->initWithImage(_scale9Image->getImage());
    _topLeft->setImageRect(lefttopbounds, false, lefttopbounds.size);
    _topLeft->setTag(pTopLeft);
    _scale9Image->insertSubview(_topLeft, 2);
    
    // Top right
    _topRight = new CAImageView();
    _topRight->initWithImage(_scale9Image->getImage());
    _topRight->setImageRect(righttopbounds, false, righttopbounds.size);
    _topRight->setTag(pTopRight);
    _scale9Image->insertSubview(_topRight, 2);
    
    // Bottom left
    _bottomLeft = new CAImageView();
    _bottomLeft->initWithImage(_scale9Image->getImage());
    _bottomLeft->setImageRect(leftbottombounds, false, leftbottombounds.size);
    _bottomLeft->setTag(pBottomLeft);
    _scale9Image->insertSubview(_bottomLeft, 2);
    
    // Bottom right
    _bottomRight = new CAImageView();
    _bottomRight->initWithImage(_scale9Image->getImage());
    _bottomRight->setImageRect(rightbottombounds, false, rightbottombounds.size);
    _bottomRight->setTag(pBottomRight);
    _scale9Image->insertSubview(_bottomRight, 2);
    

    _bottomLeft->setAnchorPoint(CCPoint(0,0));
    _bottomRight->setAnchorPoint(CCPoint(0,0));
    _topLeft->setAnchorPoint(CCPoint(0,0));
    _topRight->setAnchorPoint(CCPoint(0,0));
    _left->setAnchorPoint(CCPoint(0,0));
    _right->setAnchorPoint(CCPoint(0,0));
    _top->setAnchorPoint(CCPoint(0,0));
    _bottom->setAnchorPoint(CCPoint(0,0));
    _centre->setAnchorPoint(CCPoint(0,0));
    
    this->addSubview(_scale9Image);
    
    if (m_bSpritesGenerated)
        {
            // Restore color and opacity
            this->setColor(color);
        }
    m_bSpritesGenerated = true;

    return true;
}

void CAScale9ImageView::setContentSize(const CCSize &size)
{
    CAView::setContentSize(size);
    this->m_positionsAreDirty = true;
    
    this->updatePositions();
    this->m_positionsAreDirty = false;
}

void CAScale9ImageView::updatePositions()
{
    // Check that instances are non-NULL
    if(!((_topLeft) &&
         (_topRight) &&
         (_bottomRight) &&
         (_bottomLeft) &&
         (_centre))) {
        // if any of the above sprites are NULL, return
        return;
    }

    CCSize size = this->m_obContentSize;

    float sizableWidth = size.width - _topLeft->getBounds().size.width - _topRight->getBounds().size.width;
    
    float sizableHeight = size.height - _topLeft->getBounds().size.height - _bottomRight->getBounds().size.height;
    
    float horizontalScale = sizableWidth/_centre->getBounds().size.width;
    
    float verticalScale = sizableHeight/_centre->getBounds().size.height;

    _centre->setScaleX(horizontalScale);
    _centre->setScaleY(verticalScale);

    float rescaledWidth = _centre->getBounds().size.width * horizontalScale;
    float rescaledHeight = _centre->getBounds().size.height * verticalScale;

    float leftWidth = _bottomLeft->getBounds().size.width;
    float bottomHeight = _bottomLeft->getBounds().size.height;

    // Position corners
    _bottomLeft->setFrameOrigin(CCPoint(0,0));
    _bottomRight->setFrameOrigin(CCPoint(leftWidth+rescaledWidth,0));
    _topLeft->setFrameOrigin(CCPoint(0, bottomHeight+rescaledHeight));
    _topRight->setFrameOrigin(CCPoint(leftWidth+rescaledWidth, bottomHeight+rescaledHeight));

    // Scale and position borders
    _left->setFrameOrigin(CCPoint(0, bottomHeight));
    _left->setScaleY(verticalScale);
    _right->setFrameOrigin(CCPoint(leftWidth+rescaledWidth,bottomHeight));
    _right->setScaleY(verticalScale);
    _bottom->setFrameOrigin(CCPoint(leftWidth,0));
    _bottom->setScaleX(horizontalScale);
    _top->setFrameOrigin(CCPoint(leftWidth,bottomHeight+rescaledHeight));
    _top->setScaleX(horizontalScale);

    // Position centre
    _centre->setFrameOrigin(CCPoint(leftWidth, bottomHeight));
}


bool CAScale9ImageView::initWithImage(CAImage* image, CCRect rect, CCRect capInsets)
{
    if (image)
    {
        this->updateWithImage(CABatchView::createWithImage(image), rect, capInsets);
        this->setAnchorPoint(CCPoint(0.5f, 0.5f));
    }
    this->m_positionsAreDirty = true;
    
    if(this->m_positionsAreDirty)
    {
        this->updatePositions();
        this->m_positionsAreDirty = false;
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

bool CAScale9ImageView::initWithImage(CAImage* image)
{
	bool pReturn = this->initWithImage(image, CCRectZero);
	return pReturn;
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

CAScale9ImageView* CAScale9ImageView::resizableSpriteWithCapInsets(CCRect capInsets)
{
    CAScale9ImageView* pReturn = new CAScale9ImageView();
    if ( pReturn && pReturn->initWithImage(_scale9Image->getImage(), m_spriteRect, capInsets) )
    {
        pReturn->autorelease();
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);
    return NULL;
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

/** sets the opacity.
 @warning If the the Image has premultiplied alpha then, the R, G and B channels will be modifed.
 Values goes from 0 to 255, where 255 means fully opaque.
 */

void CAScale9ImageView::setCapInsets(CCRect capInsets)
{
    do
    {
        CC_BREAK_IF(_scale9Image == NULL);
        this->updateWithImage(_scale9Image, this->m_spriteRect, capInsets);
    }
    while (0);
    
}

CCRect CAScale9ImageView::getCapInsets()
{
    return m_capInsets;
}

void CAScale9ImageView::updateCapInset()
{
    CCRect insets;
    if (this->m_insetLeft == 0 && this->m_insetTop == 0 && this->m_insetRight == 0 && this->m_insetBottom == 0)
    {
        insets = CCRectZero;
    }
    else
    {
        insets = CCRectMake(m_insetLeft,
                            m_insetTop,
                            m_spriteRect.size.width-m_insetLeft-m_insetRight,
                            m_spriteRect.size.height-m_insetTop-m_insetBottom);
    }
    this->setCapInsets(insets);
}

void CAScale9ImageView::updateDisplayedColor(const cocos2d::CAColor4B &color)
{
    CAView::updateDisplayedColor(color);
    setColor(color);
}

float CAScale9ImageView::getInsetLeft()
{
    return this->m_insetLeft;
}

float CAScale9ImageView::getInsetTop()
{
    return this->m_insetTop;
}

float CAScale9ImageView::getInsetRight()
{
    return this->m_insetRight;
}

float CAScale9ImageView::getInsetBottom()
{
    return this->m_insetBottom;
}

void CAScale9ImageView::setInsetLeft(float insetLeft)
{
    this->m_insetLeft = insetLeft;
    this->updateCapInset();
}

void CAScale9ImageView::setInsetTop(float insetTop)
{
    this->m_insetTop = insetTop;
    this->updateCapInset();
}

void CAScale9ImageView::setInsetRight(float insetRight)
{
    this->m_insetRight = insetRight;
    this->updateCapInset();
}

void CAScale9ImageView::setInsetBottom(float insetBottom)
{
    this->m_insetBottom = insetBottom;
    this->updateCapInset();
}

void CAScale9ImageView::setColor(const CAColor4B& color)
{
    _color = color;

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
	return _color;
}

NS_CC_END
