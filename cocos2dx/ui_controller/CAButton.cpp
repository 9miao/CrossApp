//
//  CAButton.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-3-23.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CAButton.h"
#include "sprite_nodes/CAScale9ImageView.h"
#include "sprite_nodes/CAView.h"
#include "touch_dispatcher/CCTouch.h"
#include "support/CCPointExtension.h"
#include "cocoa/CCSet.h"
#include "label_nodes/CCLabelTTF.h"
#define PLAYSOUND 

NS_CC_BEGIN

CAButton::CAButton(CAButtonType buttonType)
:m_bAllowsSelected(false)
,m_bSelected(false)
,m_textTag("")
,m_closeTapSound(false)
,m_bTouchClick(false)
,m_color(CAColor_white)
,m_IsRehisterTouchDispatcher(false)
,m_eButtonType(buttonType)
,m_sTitleFontName("Helvetica-Bold")
,m_pImageView(NULL)
,m_pLabel(NULL)
{
    for (int i=0; i<CAControlStateAll; i++)
    {
        m_pImage[i] = NULL;
        m_sTitle[i] = "";
        m_sImageColor[i] = CAColor_white;
        m_sTitleColor[i] = CAColor_black;
    }
    
    m_pImageView = new CAImageView();
    m_pImageView->init();
    m_pLabel = new CCLabelTTF();
    m_pLabel->initWithString("", m_sTitleFontName.c_str(), 20);
}

CAButton::~CAButton(void)
{
    CC_SAFE_RELEASE_NULL(m_pImageView);
    CC_SAFE_RELEASE_NULL(m_pLabel);
}

void CAButton::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
}

void CAButton::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();

    this->setControlState(m_eControlState);
}

CAButton* CAButton::create(CAButtonType buttonType)
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

CAButton* CAButton::createWithFrame(const CCRect& rect, CAButtonType buttonType)
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

CAButton* CAButton::createWithCenter(const CCRect& rect, CAButtonType buttonType)
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

bool CAButton::initWithFrame(const CCRect& rect)
{
    if (!CAButton::init())
    {
        return false;
    }
    this->setColor(CAColor_clear);
    this->setFrame(rect);
    return true;
}

bool CAButton::initWithCenter(const CCRect& rect)
{
    if (!CAButton::init())
    {
        return false;
    }
    
    this->setCenter(rect);
    return true;
}

bool CAButton::init()
{
    if (!CAView::init())
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
        case CAButtonTypeRounded3DRect:
            this->setBackGroundViewRounded3DRect();
            break;
        default:
            break;
    }
    
    this->insertSubview(m_pImageView, 1);
    this->insertSubview(m_pLabel, 1);
    
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

void CAButton::setBackGroundViewRounded3DRect()
{
    const char* fileName[CAControlStateAll] =
    {
        "source_material/btn_rounded3D_normal.png",
        "source_material/btn_rounded3D_highlighted.png",
        "source_material/btn_rounded3D_disabled.png",
        "source_material/btn_rounded3D_selected.png"
    };
    
    CAColor4B color[CAControlStateAll] =
    {
        ccc4(255, 255, 255, 255),
        ccc4(0, 41, 57, 255),
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

void CAButton::setBackGroundViewForState(CAControlState controlState, CAView *var)
{
    CAControl::setBackGroundViewForState(controlState, var);
    
    CC_RETURN_IF(var == NULL);
    
    if (this->getBounds().equals(CCRectZero))
    {
        this->setBounds(CCRect(0, 0, var->getFrame().size.width, var->getFrame().size.height));
    }
    
    this->updateWithPreferredSize();
}

void CAButton::setImageForState(CAControlState controlState, CAImage* var)
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

void CAButton::setTitleForState(CAControlState controlState, std::string var)
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

void CAButton::setImageColorForState(CAControlState controlState, CAColor4B var)
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

void CAButton::setTitleColorForState(CAControlState controlState, CAColor4B var)
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

void CAButton::setTitleFontName(std::string var)
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

bool CAButton::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);

    do
    {
        CC_BREAK_IF(!this->isVisible());
        CC_BREAK_IF(!m_bTouchEnabled);
        CC_BREAK_IF(m_eControlState != CAControlStateNormal && m_eControlState != CAControlStateSelected);
        
        return this->setTouchBegin(point);
    }
    while (0);
    
    return false;
}

void CAButton::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
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

void CAButton::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);

    if (!this->isTouchClick())
        return;

    this->setTouchUpSide(point);
    
    if (getBounds().containsPoint(point))
    {
        this->setTouchUpInSide(point);
    }

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
}

void CAButton::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
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

void CAButton::setControlState(CAControlState var)
{
    CAControl::setControlState(var);

    if (m_eControlState == CAControlStateSelected)
    {
        m_bSelected = true;
    }
    else if(m_eControlState != CAControlStateHighlighted)
    {
        m_bSelected = false;
    }
    
    CAImage* image = NULL;
    std::string title = "";
    CCRect imageViewCenter = CCRectZero;
    CCRect rect = CCRectZero;
    CCPoint labelCenterOrigin = CCPointZero;
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
        scale = MIN(scale, 1.0f);
        iSize = ccpMult(iSize, scale);
        
        imageViewCenter.origin = size / 2;
        imageViewCenter.size = iSize;
    }
    else if (image == NULL && title.compare("") != 0)
    {
        labelSize = this->getBounds().size.height * 0.4f;
        labelCenterOrigin = this->getBounds().size / 2 ;
    }
    else if (image && title.compare("") != 0)
    {
        CCSize size = this->getBounds().size;
        CCSize iSize = image->getContentSize();
        float scaleX = size.width / iSize.width * 0.5f;
        float scaleY = size.height / iSize.height * 0.5f;
        float scale = MIN(scaleX, scaleY);
        scale = MIN(scale, 1.0f);
        iSize = ccpMult(iSize, scale);
        
        imageViewCenter.size = iSize;
        imageViewCenter.origin.x = size.width / 2;
        imageViewCenter.origin.y = size.height * 0.35f;
        
        labelSize = size.height * 0.25f;
        labelCenterOrigin.x = size.width / 2;
        labelCenterOrigin.y = size.height * 0.75f;
    }

    if (image != m_pImageView->getImage())
    {
        m_pImageView->setImage(image);
    }
    m_pImageView->setCenter(imageViewCenter);
    
    m_pImageView->setColor(m_sImageColor[m_eControlState]);
    m_pLabel->setColor(m_sTitleColor[m_eControlState]);
    
    
    if (strcmp(title.c_str(), m_pLabel->getString().c_str()))
    {
        m_pLabel->setString(title.c_str());
    }
    
    if (!title.empty())
    {
        m_pLabel->setFontSize(labelSize);
    }
    m_pLabel->setCenterOrigin(labelCenterOrigin);
    
    float scale = this->getBounds().size.width / m_pLabel->getImage()->getContentSize().width * 0.8f;
    scale = MIN(scale, 1.0f);
    m_pLabel->setScale(scale);
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

bool CAButton::setTouchBegin(CCPoint point)
{
	m_bTouchClick = true;

    if (m_pTarget[CAControlEventTouchDown] && m_selTouch[CAControlEventTouchDown])
    {
		((CCObject *)m_pTarget[CAControlEventTouchDown]->*m_selTouch[CAControlEventTouchDown])(this, point);
    }
    
	if (m_bTouchClick)
	{
		this->setControlState(CAControlStateHighlighted);
	}

	return m_bTouchClick;
}

void CAButton::setTouchUpInSide(CCPoint point)
{
    if (m_pTarget[CAControlEventTouchUpInSide] && m_selTouch[CAControlEventTouchUpInSide])
    {
        ((CCObject *)m_pTarget[CAControlEventTouchUpInSide]->*m_selTouch[CAControlEventTouchUpInSide])(this,point);
    }
}

void CAButton::setTouchUpSide(CCPoint point)
{
    if (m_pTarget[CAControlEventTouchUpSide] && m_selTouch[CAControlEventTouchUpSide])
    {
        ((CCObject *)m_pTarget[CAControlEventTouchUpSide]->*m_selTouch[CAControlEventTouchUpSide])(this,point);
    }
}

void CAButton::setTouchMoved(cocos2d::CCPoint point)
{
    if (m_pTarget[CAControlEventTouchMoved] && m_selTouch[CAControlEventTouchMoved])
    {
        ((CCObject *)m_pTarget[CAControlEventTouchMoved]->*m_selTouch[CAControlEventTouchMoved])(this,point);
    }
}

void CAButton::setTouchMovedOutSide(cocos2d::CCPoint point)
{
    if (m_pTarget[CAControlEventTouchMovedOutSide] && m_selTouch[CAControlEventTouchMovedOutSide])
    {
        ((CCObject *)m_pTarget[CAControlEventTouchMovedOutSide]->*m_selTouch[CAControlEventTouchMovedOutSide])(this,point);
    }
}

bool CAButton::isTextTagEqual(const char *text)
{
    return (m_textTag.compare(text)==0);
}

void CAButton::setContentSize(const CCSize & var)
{
    CCSize size = var;
    size.height = MAX(size.height, 60);
    size.width = MAX(size.width, size.height);
    CAControl::setContentSize(size);
    
    this->updateWithPreferredSize();
    this->setControlState(m_eControlState);
}

NS_CC_END