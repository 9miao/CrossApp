//
//  CAButton.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-3-23.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CAButton.h"
#include "view/CAScale9ImageView.h"
#include "view/CAView.h"
#include "view/CAScrollView.h"
#include "dispatcher/CATouch.h"
#include "support/CAPointExtension.h"
#include "cocoa/CCSet.h"
#include "view/CALabel.h"
#include "basics/CAApplication.h"
#include "basics/CAScheduler.h"

NS_CC_BEGIN

CAButton::CAButton(const CAButtonType& buttonType)
:m_bAllowsSelected(false)
,m_bSelected(false)
,m_bTouchClick(false)
,m_color(CAColor_white)
,m_eButtonType(buttonType)
,m_sTitleFontName("")
,m_pImageView(NULL)
,m_pLabel(NULL)
,m_fTitleFontSize(0)
,m_pTitleLabelSize(DSizeZero)
,m_pImageSize(DSizeZero)
,m_pTitleOffset(DSizeZero)
,m_pImageOffset(DSizeZero)
{
    for (int i=0; i<CAControlStateAll; i++)
    {
        m_pBackGroundView[i] = NULL;
    }
    
    for (int i=0; i<CAControlStateAll; i++)
    {
        m_pImage[i] = NULL;
        m_sTitle[i] = "";
        m_sImageColor[i] = CAColor_white;
        m_sTitleColor[i] = CAColor_black;
    }
    
    m_pImageView = new CAImageView();
    m_pImageView->init();
    this->insertSubview(m_pImageView, 1);
    
    m_pLabel = new CALabel();
    m_pLabel->init();
    m_pLabel->setTextAlignment(CATextAlignmentCenter);
    m_pLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    m_pLabel->setNumberOfLine(1);
    this->insertSubview(m_pLabel, 1);
}

CAButton::~CAButton(void)
{
    for (int i=0; i<CAControlStateAll; i++)
    {
        CC_SAFE_RELEASE_NULL(m_pBackGroundView[i]);
    }
    CC_SAFE_RELEASE_NULL(m_pImageView);
    CC_SAFE_RELEASE_NULL(m_pLabel);
}

void CAButton::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();
}

void CAButton::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();

    this->setControlState(m_eControlState);
}

CAButton* CAButton::create(const CAButtonType& buttonType)
{
    
    CAButton* btn = new CAButton(buttonType);
    
    if (btn && btn->init())
    {
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return NULL;
}

CAButton* CAButton::createWithFrame(const DRect& rect, const CAButtonType& buttonType)
{

    CAButton* btn = new CAButton(buttonType);
    
    if (btn && btn->initWithFrame(rect))
    {
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return NULL;
}

CAButton* CAButton::createWithCenter(const DRect& rect, const CAButtonType& buttonType)
{
    
    CAButton* btn = new CAButton(buttonType);
    
    if (btn && btn->initWithCenter(rect))
    {
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return NULL;
}

bool CAButton::init()
{
    if (!CAControl::init())
    {
        return false;
    }
    
    this->setColor(CAColor_clear);
    
    switch (m_eButtonType)
    {
        case CAButtonTypeSquareRect:
            this->setBackGroundViewSquareRect();
            break;
        case CAButtonTypeRoundedRect:
            this->setBackGroundViewRoundedRect();
            break;
        default:
            break;
    }
    
    return true;
}

void CAButton::setBackGroundViewSquareRect()
{
    const char* fileName[CAControlStateAll] =
    {
        "source_material/btn_square_normal.png",
        "source_material/btn_square_highlighted.png",
        "source_material/btn_square_disabled.png",
        "source_material/btn_square_selected.png"
    };
    
    CAColor4B color[CAControlStateAll] =
    {
        ccc4( 46, 192, 255, 255),
        ccc4(255, 255, 255, 255),
        ccc4(255, 255, 255, 255),
        ccc4(255, 255, 255, 255)
        
    };
    
    for (int i=0; i<CAControlStateAll; i++)
    {
        CAImage* image = CAImage::create(fileName[i]);
        CAScale9ImageView* bg = CAScale9ImageView::createWithImage(image);
        this->setBackGroundViewForState((CAControlState)i, bg);
        m_sTitleColor[i] = color[i];
    }
}

void CAButton::setBackGroundViewRoundedRect()
{
    const char* fileName[CAControlStateAll] =
    {
        "source_material/btn_rounded_normal.png",
        "source_material/btn_rounded_highlighted.png",
        "source_material/btn_rounded_disabled.png",
        "source_material/btn_rounded_selected.png"
    };
    
    CAColor4B color[CAControlStateAll] =
    {
        ccc4( 46, 192, 255, 255),
        ccc4(255, 255, 255, 255),
        ccc4(255, 255, 255, 255),
        ccc4(255, 255, 255, 255)
        
    };
    
    for (int i=0; i<CAControlStateAll; i++)
    {
        CAImage* image = CAImage::create(fileName[i]);
        CAScale9ImageView* bg = CAScale9ImageView::createWithImage(image);
        this->setBackGroundViewForState((CAControlState)i, bg);
        m_sTitleColor[i] = color[i];
    }
}

void CAButton::setBackGroundViewForState(const CAControlState& controlState, CAView *var)
{
    if (controlState == CAControlStateAll)
    {
        for (int i=0; i<CAControlStateAll; i++)
        {
            this->setBackGroundViewForState((CAControlState)i, var);
        }
        return;
    }
    
    if (m_pBackGroundView[controlState] != var)
    {
        CC_SAFE_RETAIN(var);
        this->removeSubview(m_pBackGroundView[controlState]);
        CC_SAFE_RELEASE(m_pBackGroundView[controlState]);
        m_pBackGroundView[controlState] = var;
        this->setControlState(m_eControlState);
    }
    
    CC_RETURN_IF(var == NULL);
    
    if (this->getBounds().equals(DRectZero))
    {
        this->setBounds(DRect(0, 0, var->getFrame().size.width, var->getFrame().size.height));
    }
    
    this->updateWithPreferredSize();
}

CAView* CAButton::getBackGroundViewForState(const CAControlState& controlState)
{
    if (controlState == CAControlStateAll)
    {
        return NULL;
    }
    return m_pBackGroundView[controlState];
}

void CAButton::setImageForState(const CAControlState& controlState, CAImage* var)
{
    if (controlState == CAControlStateAll)
    {
        for (int i=0; i<CAControlStateAll; i++)
            this->setImageForState((CAControlState)i, var);

        return;
    }
    
    if (m_pImage[controlState] != var)
    {
        CC_SAFE_RETAIN(var);
        CC_SAFE_RELEASE(m_pImage[controlState]);
        m_pImage[controlState] = var;
    }
    
    if (m_bRunning)
    {
        this->setControlState(m_eControlState);
    }
}

CAImage* CAButton::getImageForState(const CAControlState& controlState)
{
    return m_pImage[controlState];
}

void CAButton::setTitleForState(const CAControlState& controlState, const std::string& var)
{
    if (controlState == CAControlStateAll)
    {
        for (int i=0; i<CAControlStateAll; i++)
            this->setTitleForState((CAControlState)i, var);
        
        return;
    }
    
    if (m_sTitle[controlState] != var)
    {
        m_sTitle[controlState] = var;
    }
    
    if (m_bRunning)
    {
        this->setControlState(m_eControlState);
    }
}

const std::string& CAButton::getTitleForState(const CAControlState& controlState)
{
    return m_sTitle[controlState];
}

void CAButton::setImageColorForState(const CAControlState& controlState, const CAColor4B& var)
{
    if (controlState == CAControlStateAll)
    {
        for (int i=0; i<CAControlStateAll; i++)
            this->setImageColorForState((CAControlState)i, var);
    }
    else
    {
        m_sImageColor[controlState] = var;
    }
    
    if (m_bRunning)
    {
        this->setControlState(m_eControlState);
    }
}

void CAButton::setTitleColorForState(const CAControlState& controlState, const CAColor4B& var)
{
    if (controlState == CAControlStateAll)
    {
        for (int i=0; i<CAControlStateAll; i++)
            this->setTitleColorForState((CAControlState)i, var);
    }
    else
    {
        m_sTitleColor[controlState] = var;
    }
    
    if (m_bRunning)
    {
        this->setControlState(m_eControlState);
    }
}

void CAButton::setTitleFontName(const std::string& var)
{
    if (m_sTitleFontName.compare(var))
    {
        m_sTitleFontName = var;
        m_pLabel->setFontName(m_sTitleFontName.c_str());
    }
    
    if (m_bRunning)
    {
        this->setControlState(m_eControlState);
    }
}

void CAButton::updateWithPreferredSize()
{
    for (int i=0; i<CAControlStateAll; i++)
    {
        CC_CONTINUE_IF(m_pBackGroundView[i] == NULL);
        CC_CONTINUE_IF(this->getBounds().equals(m_pBackGroundView[i]->getBounds()));
        
        m_pBackGroundView[i]->setFrame(this->getBounds());
    }
    
    if (m_fTitleFontSize < FLT_EPSILON)
    {
        m_pLabel->setFontSize(this->getBounds().size.height * 0.667f);
    }
}

bool CAButton::ccTouchBegan(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);

    do
    {
        CC_BREAK_IF(m_eControlState != CAControlStateNormal && m_eControlState != CAControlStateSelected);

        CAScheduler::schedule(schedule_selector(CAButton::setTouchLongPress), this, 0, 0, 0.5f);
        
        return this->setTouchBegin(point);
    }
    while (0);
    
    return false;
}

void CAButton::ccTouchMoved(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    CC_RETURN_IF(!this->isTouchClick());
    
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);

    CAScheduler::unschedule(schedule_selector(CAButton::setTouchLongPress), this);
    
    if (getBounds().containsPoint(point))
    {
        this->setTouchMoved(point);
        this->setControlState(CAControlStateHighlighted);
    }
    else
    {
        this->setTouchMovedOutSide(point);
        if (m_bAllowsSelected && m_bSelected)
        {
            this->setControlState(CAControlStateSelected);
        }
        else
        {
            this->setControlState(CAControlStateNormal);
        }
    }
}

void CAButton::ccTouchEnded(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    CC_RETURN_IF(!this->isTouchClick());
    
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    CAScheduler::unschedule(schedule_selector(CAButton::setTouchLongPress), this);
    
    do
    {
        CC_BREAK_IF(this->getControlState() != CAControlStateHighlighted);
        
        if (m_bAllowsSelected)
        {
            if (getBounds().containsPoint(point))
            {
                m_bSelected = !m_bSelected;
            }
            
            if (m_bSelected)
            {
                this->setControlState(CAControlStateSelected);
            }
            else
            {
                this->setControlState(CAControlStateNormal);
            }
        }
        else
        {
            this->setControlState(CAControlStateNormal);
        }
    }
    while (0);
    
    if (getBounds().containsPoint(point))
    {
        this->setTouchUpInSide(point);
    }
    else
    {
        this->setTouchUpOutSide(point);
    }
}

void CAButton::ccTouchCancelled(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    CAScheduler::unschedule(schedule_selector(CAButton::setTouchLongPress), this);
    
    if (m_bAllowsSelected && m_bSelected)
    {
        this->setControlState(CAControlStateSelected);
    }
    else
    {
        this->setControlState(CAControlStateNormal);
    }
    
    if (m_pTarget[CAControlEventTouchCancelled] && m_selTouch[CAControlEventTouchCancelled])
    {
        ((CAObject *)m_pTarget[CAControlEventTouchCancelled]->*m_selTouch[CAControlEventTouchCancelled])(this, point);
    }
}

void CAButton::setControlState(const CAControlState& var)
{
    CAControl::setControlState(var);

    CC_RETURN_IF(var == CAControlStateAll);
    
    for (int i=0; i<CAControlStateAll; i++)
    {
        this->removeSubview(m_pBackGroundView[i]);
    }
    
    m_eControlState = var;
    
    if (m_bControlStateLocked)
    {
        m_eControlState = CAControlStateNormal;
    }
    
    if (m_pBackGroundView[m_eControlState] && m_eControlState != CAControlStateNormal)
    {
        m_pBackGroundView[m_eControlState]->setFrame(this->getBounds());
        this->insertSubview(m_pBackGroundView[m_eControlState], -1);
    }
    else if (m_pBackGroundView[CAControlStateNormal])
    {
        m_pBackGroundView[CAControlStateNormal]->setFrame(this->getBounds());
        this->insertSubview(m_pBackGroundView[CAControlStateNormal], -1);
    }
    
    if (m_eControlState != CAControlStateHighlighted)
    {
        m_bSelected = m_eControlState == CAControlStateSelected ? true : false;
    }
    
    CAImage* image = NULL;
    std::string title = "";
    DRect imageViewCenter = CCRectZero;
    DRect rect = CCRectZero;
    DRect labelCenter = this->getBounds();
    float labelSize = 0;
    
    image = m_pImage[m_eControlState];
    title = m_sTitle[m_eControlState];
    
    if (image == NULL)
    {
        image = this->isSelected() ? m_pImage[CAControlStateSelected] : m_pImage[CAControlStateNormal];
    }
    
    if (strcmp(title.c_str(), "") == 0)
    {
        title = this->isSelected() ? m_sTitle[CAControlStateSelected] : m_sTitle[CAControlStateNormal];
    }
    
    if (image && title.compare("") == 0)
    {
        DSize size = this->getBounds().size;
        DSize iSize = image->getContentSize();
        float scaleX = size.width / iSize.width * 0.75f;
        float scaleY = size.height / iSize.height * 0.75f;
        float scale = MIN(scaleX, scaleY);
        iSize = ccpMult(iSize, scale);
        
        imageViewCenter.origin = size / 2;
        imageViewCenter.size = iSize;
    }
    else if (image == NULL && title.compare("") != 0)
    {
        labelSize = this->getBounds().size.height * 0.4f;
        labelCenter.origin = this->getBounds().size / 2 ;
    }
    else if (image && title.compare("") != 0)
    {
        DSize size = this->getBounds().size;
        DSize iSize = image->getContentSize();
        float scaleX = size.width / iSize.width * 0.5f;
        float scaleY = size.height / iSize.height * 0.45f;
        float scale = MIN(scaleX, scaleY);
        iSize = ccpMult(iSize, scale);
        
        imageViewCenter.size = iSize;
        imageViewCenter.origin.x = size.width / 2;
        imageViewCenter.origin.y = size.height * 0.35f;
        
        labelSize = size.height * 0.2f;
        labelCenter.origin.x = size.width / 2;
        labelCenter.origin.y = size.height * 0.75f;
    }

    m_pImageView->setColor(m_sImageColor[m_eControlState]);

    if (!m_pImageSize.equals(DSizeZero))
    {
        imageViewCenter.size = m_pImageSize;
    }
    if (!m_pImageOffset.equals(DSizeZero))
    {
        imageViewCenter.origin = ccpMult(this->getBounds().size, 0.5f);
        imageViewCenter.origin = ccpAdd(imageViewCenter.origin, m_pImageOffset);
    }
    m_pImageView->setCenter(imageViewCenter);
    
    
    if (image != m_pImageView->getImage())
    {
        m_pImageView->setImage(image);
    }
    m_pLabel->setColor(m_sTitleColor[m_eControlState]);

    
    if (!m_pTitleLabelSize.equals(DSizeZero))
    {
        labelCenter.size = m_pTitleLabelSize;
    }
    if(!m_pTitleOffset.equals(DSizeZero))
    {
        labelCenter.origin = ccpMult(this->getBounds().size, 0.5f);
        labelCenter.origin = ccpAdd(labelCenter.origin, m_pTitleOffset);
    }
    m_pLabel->setCenter(labelCenter);
    
    if (!title.empty())
    {
        if(m_fTitleFontSize==0)
            m_fTitleFontSize = labelSize;
        m_pLabel->setFontSize(m_fTitleFontSize);
    }
    
    if (strcmp(title.c_str(), m_pLabel->getText().c_str()))
    {
        m_pLabel->setText(title.c_str());
    }
}

void CAButton::interruptTouchState()
{
    CC_RETURN_IF(m_bTouchClick == false);
    m_bTouchClick = false;
    CC_RETURN_IF(m_eControlState != CAControlStateHighlighted);

    this->ccTouchCancelled(NULL, NULL);
}

bool CAButton::setTouchBegin(const DPoint& point)
{
	m_bTouchClick = true;

    if (m_pTarget[CAControlEventTouchDown] && m_selTouch[CAControlEventTouchDown])
    {
		((CAObject *)m_pTarget[CAControlEventTouchDown]->*m_selTouch[CAControlEventTouchDown])(this, point);
    }
    
	if (m_bTouchClick)
	{
		this->setControlState(CAControlStateHighlighted);
	}

	return m_bTouchClick;
}

void CAButton::setTouchUpInSide(const DPoint& point)
{
    if (m_pTarget[CAControlEventTouchUpInSide] && m_selTouch[CAControlEventTouchUpInSide])
    {
        ((CAObject *)m_pTarget[CAControlEventTouchUpInSide]->*m_selTouch[CAControlEventTouchUpInSide])(this, point);
    }
}

void CAButton::setTouchUpOutSide(const DPoint& point)
{
    if (m_pTarget[CAControlEventTouchUpOutSide] && m_selTouch[CAControlEventTouchUpOutSide])
    {
        ((CAObject *)m_pTarget[CAControlEventTouchUpOutSide]->*m_selTouch[CAControlEventTouchUpOutSide])(this, point);
    }
}

void CAButton::setTouchMoved(const DPoint& point)
{
    if (m_pTarget[CAControlEventTouchMoved] && m_selTouch[CAControlEventTouchMoved])
    {
        ((CAObject *)m_pTarget[CAControlEventTouchMoved]->*m_selTouch[CAControlEventTouchMoved])(this, point);
    }
}

void CAButton::setTouchMovedOutSide(const DPoint& point)
{
    if (m_pTarget[CAControlEventTouchMovedOutSide] && m_selTouch[CAControlEventTouchMovedOutSide])
    {
        ((CAObject *)m_pTarget[CAControlEventTouchMovedOutSide]->*m_selTouch[CAControlEventTouchMovedOutSide])(this, point);
    }
}

void CAButton::setTouchLongPress(float dt)
{
    if (m_pTarget[CAControlEventTouchLongPress] && m_selTouch[CAControlEventTouchLongPress])
    {
        ((CAObject *)m_pTarget[CAControlEventTouchLongPress]->*m_selTouch[CAControlEventTouchLongPress])(this, DPointZero);
    }
}

void CAButton::setContentSize(const DSize & var)
{
    DSize size = var;
//    if (m_eButtonType != CAButtonTypeCustom)
//    {
//        size.height = MAX(size.height, _px(60));
//        size.width = MAX(size.width, _px(60));
//    }
    CAView::setContentSize(size);
    for(int i=0; i<CAControlStateAll; i++)
    {
        CC_CONTINUE_IF(m_pBackGroundView[i] == NULL);
        m_pBackGroundView[i]->setFrame(this->getBounds());
    }
    
    this->updateWithPreferredSize();
    this->setControlState(m_eControlState);
}

void CAButton::setImageOffset(const DSize& offset)
{
    m_pImageOffset = offset;
    DRect rect = m_pImageView->getCenter();
    rect.origin = m_obContentSize/2;
    rect.origin.x += offset.width;
    rect.origin.y += offset.height;
    m_pImageView->setCenter(rect);
}

void CAButton::setImageSize(const DSize& size)
{
    m_pImageSize = size;
    DRect rect = m_pImageView->getCenter();
    rect.size = m_pImageSize;
    m_pImageView->setCenter(rect);
}

void CAButton::setTitleOffset(const DSize& offset)
{
    m_pTitleOffset = offset;
    DRect rect = m_pLabel->getCenter();
    rect.origin = m_obContentSize/2;
    rect.origin.x += offset.width;
    rect.origin.y += offset.height;
    m_pLabel->setCenter(rect);
}

void CAButton::setTitleLabelSize(const DSize& size)
{
    m_pTitleLabelSize = size;
    DRect rect = m_pLabel->getCenter();
    rect.size = m_pTitleLabelSize;
    m_pLabel->setCenter(rect);
}

void CAButton::setTitleFontSize(float fontSize)
{
    m_fTitleFontSize = fontSize;
    m_pLabel->setFontSize(m_fTitleFontSize);
}

NS_CC_END