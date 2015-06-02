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
#include "support/CCPointExtension.h"
#include "cocoa/CCSet.h"
#include "view/CALabel.h"
#include "basics/CAApplication.h"

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

CAButton* CAButton::createWithFrame(const CCRect& rect, const CAButtonType& buttonType)
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

CAButton* CAButton::createWithCenter(const CCRect& rect, const CAButtonType& buttonType)
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
    CCAssert(dynamic_cast<CAControl*>(var) == NULL, "Not allowed to inherit from the CAControl");
    CCAssert(dynamic_cast<CAScrollView*>(var) == NULL, "Not allowed to inherit from the CAScrollView");
    
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
    
    if (this->getBounds().equals(CCRectZero))
    {
        this->setBounds(CCRect(0, 0, var->getFrame().size.width, var->getFrame().size.height));
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
        
        if (CAScale9ImageView* _var = dynamic_cast<CAScale9ImageView*>(m_pBackGroundView[i]))
        {
            _var->setFrame(this->getBounds());
        }
        else
        {
            m_pBackGroundView[i]->setFrame(this->getBounds());
        }
    }
    
    m_pLabel->setFontSize(this->getBounds().size.height * 0.667f);
}

bool CAButton::ccTouchBegan(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);

    do
    {
        CC_BREAK_IF(m_eControlState != CAControlStateNormal && m_eControlState != CAControlStateSelected);

        return this->setTouchBegin(point);
    }
    while (0);
    
    return false;
}

void CAButton::ccTouchMoved(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);

    if (!this->isTouchClick()) return;
    
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
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    if (!this->isTouchClick())
        return;
    do
    {
        CC_BREAK_IF(this->getControlState() != CAControlStateHighlighted);
        
        if (m_bAllowsSelected)
        {
            if (m_bSelected)
            {
                m_bSelected = false;
                this->setControlState(CAControlStateNormal);
            }
            else
            {
                m_bSelected = true;
                this->setControlState(CAControlStateSelected);
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
    if (m_bAllowsSelected && m_bSelected)
    {
        this->setControlState(CAControlStateSelected);
    }
    else
    {
        this->setControlState(CAControlStateNormal);
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
    CCRect imageViewCenter = CCRectZero;
    CCRect rect = CCRectZero;
    CCRect labelCenter = this->getBounds();
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
        CCSize size = this->getBounds().size;
        CCSize iSize = image->getContentSize();
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
        CCSize size = this->getBounds().size;
        CCSize iSize = image->getContentSize();
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
    m_pImageView->setCenter(imageViewCenter);
    
    if (image != m_pImageView->getImage())
    {
        m_pImageView->setImage(image);
    }
    
    m_pLabel->setColor(m_sTitleColor[m_eControlState]);
    m_pLabel->setCenter(labelCenter);
    
    if (!title.empty())
    {
        m_pLabel->setFontSize(labelSize);
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
    if (m_bAllowsSelected && m_bSelected)
    {
        this->setControlState(CAControlStateSelected);
    }
    else
    {
        this->setControlState(CAControlStateNormal);
    }
}

bool CAButton::setTouchBegin(const CCPoint& point)
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

void CAButton::setTouchUpInSide(const CCPoint& point)
{
    if (m_pTarget[CAControlEventTouchUpInSide] && m_selTouch[CAControlEventTouchUpInSide])
    {
        ((CAObject *)m_pTarget[CAControlEventTouchUpInSide]->*m_selTouch[CAControlEventTouchUpInSide])(this,point);
    }
}

void CAButton::setTouchUpOutSide(const CCPoint& point)
{
    if (m_pTarget[CAControlEventTouchUpOutSide] && m_selTouch[CAControlEventTouchUpOutSide])
    {
        ((CAObject *)m_pTarget[CAControlEventTouchUpOutSide]->*m_selTouch[CAControlEventTouchUpOutSide])(this,point);
    }
}

void CAButton::setTouchMoved(const CCPoint& point)
{
    if (m_pTarget[CAControlEventTouchMoved] && m_selTouch[CAControlEventTouchMoved])
    {
        ((CAObject *)m_pTarget[CAControlEventTouchMoved]->*m_selTouch[CAControlEventTouchMoved])(this,point);
    }
}

void CAButton::setTouchMovedOutSide(const CCPoint& point)
{
    if (m_pTarget[CAControlEventTouchMovedOutSide] && m_selTouch[CAControlEventTouchMovedOutSide])
    {
        ((CAObject *)m_pTarget[CAControlEventTouchMovedOutSide]->*m_selTouch[CAControlEventTouchMovedOutSide])(this,point);
    }
}

void CAButton::setContentSize(const CCSize & var)
{
    CCSize size = var;
    if (m_eButtonType != CAButtonTypeCustom)
    {
        size.height = MAX(size.height, _px(60));
        size.width = MAX(size.width, _px(60));
    }
    CAView::setContentSize(size);
    for(int i=0; i<CAControlStateAll; i++)
    {
        CC_CONTINUE_IF(m_pBackGroundView[i] == NULL);
        m_pBackGroundView[i]->setFrame(this->getBounds());
    }
    
    this->updateWithPreferredSize();
    this->setControlState(m_eControlState);
}

NS_CC_END