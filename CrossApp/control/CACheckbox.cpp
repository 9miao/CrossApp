//
//  CACheckbox.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-3-23.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CACheckbox.h"
#include "view/CAScale9ImageView.h"
#include "view/CAView.h"
#include "view/CAScrollView.h"
#include "dispatcher/CATouch.h"
#include "basics/CAPointExtension.h"
#include "cocoa/CCSet.h"
#include "view/CALabel.h"
#include "basics/CAApplication.h"
#include "basics/CAScheduler.h"
#include "animation/CAViewAnimation.h"
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
NS_CC_BEGIN

CACheckbox::CACheckbox(const CACheckbox::Type& type)
:m_pImageView(nullptr)
,m_pLabel(nullptr)
,m_pTitleLabelSize(DSizeZero)
,m_bDefineTitleLabelSize(false)
,m_pImageSize(DSizeZero)
,m_bDefineImageSize(false)
,m_pTitleOffset(DSizeZero)
,m_bDefineTitleOffset(false)
,m_pImageOffset(DSizeZero)
,m_bDefineImageOffset(false)
,m_pImageNormal(nullptr)
,m_pImageSelected(nullptr)
,m_cImageColorNormal(CAColor4B::WHITE)
,m_cImageColorSelected(CAColor4B::WHITE)
,m_cTitleColorNormal(CAColor4B::BLACK)
,m_cTitleColorSelected(CAColor4B::BLACK)
,m_bIsOn(false)
,m_eType(type)
{
    m_pImageView = new CAImageView();
    m_pImageView->init();
    this->insertSubview(m_pImageView, 1);
    
    m_pLabel = new CALabel();
    m_pLabel->init();
    m_pLabel->setTextAlignment(CATextAlignment::Left);
    m_pLabel->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
    m_pLabel->setNumberOfLine(1);
    this->insertSubview(m_pLabel, 1);
    
    m_obTitleFont = m_pLabel->getFont();
    m_obTitleFont.fontSize = 0;
}

CACheckbox::~CACheckbox(void)
{
    CC_SAFE_RELEASE_NULL(m_pImageView);
    CC_SAFE_RELEASE_NULL(m_pLabel);
    CC_SAFE_RELEASE_NULL(m_pImageNormal);
    CC_SAFE_RELEASE_NULL(m_pImageSelected);
}

CACheckbox* CACheckbox::create(const CACheckbox::Type& type)
{
    
    CACheckbox* btn = new CACheckbox(type);
    
    if (btn && btn->init())
    {
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return NULL;
}

CACheckbox* CACheckbox::createWithFrame(const DRect& rect, const CACheckbox::Type& type)
{
    CACheckbox* btn = new CACheckbox(type);
    
    if (btn && btn->initWithFrame(rect))
    {
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return NULL;
}

CACheckbox* CACheckbox::createWithCenter(const DRect& rect, const CACheckbox::Type& type)
{
    CACheckbox* btn = new CACheckbox(type);
    
    if (btn && btn->initWithCenter(rect))
    {
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return NULL;
}

CACheckbox* CACheckbox::createWithLayout(const CrossApp::DLayout &layout, const CACheckbox::Type& type)
{
    CACheckbox* btn = new CACheckbox(type);
    
    if (btn && btn->initWithLayout(layout))
    {
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return NULL;
}

bool CACheckbox::init()
{
    if (!CAControl::init())
    {
        return false;
    }

    switch (m_eType) {
        case CACheckbox::Type::SquareRect:
        {
            const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CACheckbox_SquareRect");
            this->setImageStateNormal(CAImage::create(map.at("checkbox_image_normal")));
            this->setImageStateSelected(CAImage::create(map.at("checkbox_image_selected")));
            this->setTitleColorStateNormal(ccc4Int(CrossApp::hex2Int(map.at("titleColor_normal"))));
            this->setTitleColorStateSelected(ccc4Int(CrossApp::hex2Int(map.at("titleColor_selected"))));
        }
            break;
        case CACheckbox::Type::RoundedRect:
        {
            const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CACheckbox_RoundedRect");
            this->setImageStateNormal(CAImage::create(map.at("checkbox_image_normal")));
            this->setImageStateSelected(CAImage::create(map.at("checkbox_image_selected")));
            this->setTitleColorStateNormal(ccc4Int(CrossApp::hex2Int(map.at("titleColor_normal"))));
            this->setTitleColorStateSelected(ccc4Int(CrossApp::hex2Int(map.at("titleColor_selected"))));
        }
            break;
        default:
            break;
    }
    
    return true;
}

void CACheckbox::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();
}

void CACheckbox::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();
    
    this->updateCheckboxState();
}

void CACheckbox::setImageStateNormal(CAImage* var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE(m_pImageNormal);
    m_pImageNormal = var;
    
    this->updateCheckboxState();
}

CAImage* CACheckbox::getImageStateNormal()
{
    return m_pImageNormal;
}

void CACheckbox::setImageStateSelected(CAImage* var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE(m_pImageSelected);
    m_pImageSelected = var;
    
    this->updateCheckboxState();
}

CAImage* CACheckbox::getImageStateSelected()
{
    return m_pImageSelected;
}

void CACheckbox::setTitleStateNormal(const std::string& var)
{
    m_sTitleNormal = var;
    
    this->updateCheckboxState();
}

const std::string& CACheckbox::getTitleStateNormal()
{
    return m_sTitleNormal;
}

void CACheckbox::setTitleStateSelected(const std::string& var)
{
    m_sTitleSelected = var;
    
    this->updateCheckboxState();
}

const std::string& CACheckbox::getTitleStateSelected()
{
    return m_sTitleSelected;
}

void CACheckbox::setImageColorStateNormal(const CAColor4B& var)
{
    m_cImageColorNormal = var;
    
    this->updateCheckboxState();
}

void CACheckbox::setImageColorStateSelected(const CAColor4B& var)
{
    m_cImageColorSelected = var;
    
    this->updateCheckboxState();
}

void CACheckbox::setTitleColorStateNormal(const CAColor4B& var)
{
    m_cTitleColorNormal = var;
    
    this->updateCheckboxState();
}

void CACheckbox::setTitleColorStateSelected(const CAColor4B& var)
{
    m_cTitleColorSelected = var;
    
    this->updateCheckboxState();
}

void CACheckbox::setTitleFontName(const std::string& var)
{
    if (m_obTitleFont.fontName.compare(var))
    {
        m_obTitleFont.fontName = var;
        m_pLabel->setFont(m_obTitleFont);
        
        this->updateCheckboxState();
    }
}

void CACheckbox::setImageOffset(const DSize& offset)
{
    m_bDefineImageOffset = true;
    m_pImageOffset = offset;
    DRect rect = m_pImageView->getCenter();
    rect.origin = m_obContentSize/2;
    rect.origin.x += offset.width;
    rect.origin.y += offset.height;
    m_pImageView->setCenter(rect);
}

void CACheckbox::setImageSize(const DSize& size)
{
    m_bDefineImageSize = true;
    m_pImageSize = size;
    DRect rect = m_pImageView->getCenter();
    rect.size = m_pImageSize;
    m_pImageView->setCenter(rect);
}

void CACheckbox::setTitleOffset(const DSize& offset)
{
    m_bDefineTitleOffset = true;
    m_pTitleOffset = offset;
    DRect rect = m_pLabel->getCenter();
    rect.origin = m_obContentSize/2;
    rect.origin.x += offset.width;
    rect.origin.y += offset.height;
    m_pLabel->setCenter(rect);
}

void CACheckbox::setTitleLabelSize(const DSize& size)
{
    m_bDefineTitleLabelSize= true;
    m_pTitleLabelSize = size;
    DRect rect = m_pLabel->getCenter();
    rect.size = m_pTitleLabelSize;
    m_pLabel->setCenter(rect);
}

void CACheckbox::setTitleFontSize(float fontSize)
{
    m_obTitleFont.fontSize = fontSize;
    this->updateCheckboxState();
}

void CACheckbox::setTitleBold(bool bold)
{
    m_obTitleFont.bold = bold;
    m_pLabel->setBold(bold);
}

void CACheckbox::setTitleTextAlignment(const CATextAlignment& var)
{
    m_pLabel->setTextAlignment(var);
}

void CACheckbox::setTitleFont(const CAFont& font)
{
    m_obTitleFont = font;
    m_pLabel->setFont(font);
}

const CAFont& CACheckbox::getTitleFont()
{
    return m_pLabel->getFont();
}

void CACheckbox::setTarget(const std::function<void(bool on)>& function)
{
    if (function) m_function = function;
}

void CACheckbox::setIsOn(bool on)
{
    if (m_bIsOn != on)
    {
        m_bIsOn = on;
        this->updateCheckboxState();
    }
}

bool CACheckbox::isOn()
{
    return m_bIsOn;
}

void CACheckbox::updateWithPreferredSize()
{
    if (m_obTitleFont.fontSize < FLT_EPSILON)
    {
        m_pLabel->setFontSize(this->getBounds().size.height * 0.667f);
    }
}

bool CACheckbox::ccTouchBegan(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);

    this->setHighlightedState(true);
    
    return true;
}

void CACheckbox::ccTouchMoved(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    if (getBounds().containsPoint(point))
    {
        this->setHighlightedState(true);
    }
    else
    {
        this->setHighlightedState(false);
    }
}

void CACheckbox::ccTouchEnded(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);

    this->setHighlightedState(false);
    
    bool on = !m_bIsOn;
    
    if (getBounds().containsPoint(point))
    {
        on = !m_bIsOn;
    }
    else
    {
        on = m_bIsOn;
    }
    
    if (on != m_bIsOn)
    {
        this->setIsOn(on);
        if (m_function)
        {
            m_function(m_bIsOn);
        }
    }
}

void CACheckbox::ccTouchCancelled(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    this->setHighlightedState(false);
}

void CACheckbox::setHighlightedState(bool var)
{
    CAColor4B imageColor = m_bIsOn ? m_cImageColorSelected : m_cImageColorNormal;
    CAColor4B titleColor = m_bIsOn ? m_cTitleColorSelected : m_cTitleColorNormal;
    
    if (var)
    {
        imageColor = ccc4Mult(imageColor, 0.9f);
        titleColor = ccc4Mult(titleColor, 0.9f);
    }
    
    m_pImageView->setColor(imageColor);
    m_pLabel->setColor(titleColor);
}

void CACheckbox::updateCheckboxState()
{
    CC_RETURN_IF(!m_bRunning);

    DRect imageViewCenter = DRectZero;
    DRect rect = DRectZero;
    DRect labelCenter = this->getBounds();
    float labelSize = 0;
    
    CAImage* image = (m_bIsOn && m_pImageSelected) ? m_pImageSelected : m_pImageNormal;
    const std::string& title = (m_bIsOn && !m_sTitleSelected.empty()) ? m_sTitleSelected : m_sTitleNormal;

    if (image && title.length() == 0)
    {
        DSize size = this->getBounds().size;
        DSize iSize = image->getContentSize();
        float scaleX = size.width / iSize.width * 0.8f;
        float scaleY = size.height / iSize.height * 0.8f;
        float scale = MIN(scaleX, scaleY);
        iSize = ccpMult(iSize, scale);
        
        imageViewCenter.origin = size / 2;
        imageViewCenter.size = iSize;
    }
    else if (!image && title.length() > 0)
    {
        labelSize = m_obContentSize.height * 0.8f;
        labelCenter.size.height = MAX(m_obContentSize.height, CAImage::getFontHeight(m_obTitleFont.fontName.c_str(), m_obTitleFont.fontSize));
        labelCenter.origin.x = m_obContentSize.width * 0.5f;
        labelCenter.origin.y = m_obContentSize.height * 0.425f;
    }
    else if (image && title.length() > 0)
    {
        DSize size = m_obContentSize;
        DSize iSize = image->getContentSize();
        float scaleX = size.width / iSize.width * 0.8f;
        float scaleY = size.height / iSize.height * 0.8f;
        float scale = MIN(scaleX, scaleY);
        iSize = ccpMult(iSize, scale);
        
        imageViewCenter.size = iSize;
        imageViewCenter.origin.x = iSize.width * 0.5f + 10;
        imageViewCenter.origin.y = size.height * 0.5f;
        
        labelSize = size.height * 0.8f;
        labelCenter.size.width = m_obContentSize.width - iSize.width - 30;
        labelCenter.size.height = MAX(m_obContentSize.height, CAImage::getFontHeight(m_obTitleFont.fontName.c_str(), m_obTitleFont.fontSize));
        labelCenter.origin.x = iSize.width + 20 + labelCenter.size.width /2;
        labelCenter.origin.y = size.height * 0.5f;
    }

    if (!title.empty())
    {
        if (m_bDefineTitleLabelSize)
        {
            labelCenter.size = m_pTitleLabelSize;
        }
        
        if(m_bDefineTitleOffset)
        {
            labelCenter.origin = ccpMult(m_obContentSize, 0.5f);
            labelCenter.origin = ccpAdd(labelCenter.origin, m_pTitleOffset);
        }
        m_pLabel->setCenter(labelCenter);
        
        if(m_obTitleFont.fontSize == 0)
        {
            m_obTitleFont.fontSize = labelSize;
        }
        m_pLabel->setFont(m_obTitleFont);

        m_pLabel->setColor(m_bIsOn ? m_cTitleColorSelected : m_cTitleColorNormal);
    }
    
    if (strcmp(title.c_str(), m_pLabel->getText().c_str()))
    {
        m_pLabel->setText(title.c_str());
    }
    
    if (image)
    {
        m_pImageView->setColor(m_bIsOn ? m_cImageColorSelected : m_cImageColorNormal);
        
        if (m_bDefineImageSize)
        {
            imageViewCenter.size = m_pImageSize;
        }
        if (m_bDefineImageOffset)
        {
            imageViewCenter.origin = ccpMult(m_obContentSize, 0.5f);
            imageViewCenter.origin = ccpAdd(imageViewCenter.origin, m_pImageOffset);
        }
        m_pImageView->setCenter(imageViewCenter);
    }
    
    if (image != m_pImageView->getImage())
    {
        m_pImageView->setImage(image);
    }
}

void CACheckbox::setContentSize(const DSize & var)
{
    DSize size = var;
//    if (m_bRecSpe)
//    {
//        const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("");
//        int h = atoi(map.at("height").c_str());
//        size.height = (h == 0) ? size.height : h;
//    }
    CAView::setContentSize(size);
    
    this->updateWithPreferredSize();
    this->updateCheckboxState();
}


NS_CC_END
