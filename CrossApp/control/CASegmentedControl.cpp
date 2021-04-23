//
//  CASegmentedControl.cpp
//  CrossApp
//
//  Created by qiaoxin on 15-1-20.
//  Copyright (c) 2015年 CrossApp. All rights reserved.
//

#include "CASegmentedControl.h"
#include "view/CAScale9ImageView.h"
#include "dispatcher/CATouch.h"
#include "basics/CAApplication.h"
#include "view/CAClippingView.h"
#include "view/CADrawView.h"
#include "view/CARenderImage.h"
#include "platform/CADensityDpi.h"
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
#include "basics/CAPointExtension.h"

NS_CC_BEGIN

CASegmentedControl::CASegmentedControl(unsigned int itemsCount)
    : m_nItemsCount(itemsCount)
    , m_pBackgroundView(nullptr)
    , m_pNewSegmentItemBackgroundImage(nullptr)
    , m_pSegmentItemBackgroundImage(nullptr)
    , m_fSeparateWidth(LINE_WIDTH)
    , m_iSelectedIndex(0)
    , m_iTouchIndex(-1)
    , m_fSegmentWidth(0)
    , m_cTextColor(CAColor4B(54, 195, 240, 255))
    , m_cTextSelectedColor(CAColor4B::WHITE)
    , m_cImageColor(CAColor4B::WHITE)
    , m_cImageSelectedColor(CAColor4B::WHITE)
    , m_cTintColor(CAColor4B(54, 195, 240, 255))
    , m_pTarget(nullptr)
    , m_obTitleFont(CAFont())
{
    this->setTouchEventScrollHandOverToSuperview(false);
    m_obTitleFont.fontSize = 24;
}

CASegmentedControl::~CASegmentedControl()
{
    CC_SAFE_RELEASE(m_pSegmentItemBackgroundImage);
    CC_SAFE_RELEASE(m_pNewSegmentItemBackgroundImage);
}

void CASegmentedControl::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();
    
    for (size_t i=0; i<(size_t)m_nItemsCount; i++)
    {
        if (i == (size_t)m_iSelectedIndex)
        {
            m_vItemSelectedBackgrounds.at(i)->setVisible(true);
        }
        else
        {
            m_vItemSelectedBackgrounds.at(i)->setVisible(false);
        }
    }
    
    for (size_t i=0; i<(size_t)m_nItemsCount; i++)
    {
        if (CAImageView* imageView = dynamic_cast<CAImageView*>(m_vSegmentItemsTitles.at(i)))
        {
            CAImage* image = m_iSelectedIndex == i ? m_vSelectedImages.at(i) : m_vNormalImages.at(i);
            imageView->setImage(image);
            const CAColor4B& color = m_iSelectedIndex == i ? m_cImageSelectedColor : m_cImageColor;
            imageView->setColor(color);
        }
        else if (CALabel* label = dynamic_cast<CALabel*>(m_vSegmentItemsTitles.at(i)))
        {
            const CAColor4B& color = m_iSelectedIndex == i ? m_cTextSelectedColor : m_cTextColor;
            label->setColor(color);
        }
    }
}

void CASegmentedControl::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();
}

CASegmentedControl* CASegmentedControl::create(unsigned int itemsCount)
{
    CASegmentedControl* segmentedControl = new CASegmentedControl(itemsCount);
    if (segmentedControl && segmentedControl->init())
    {
        segmentedControl->autorelease();
        return segmentedControl;
    }
    CC_SAFE_DELETE(segmentedControl);
    return NULL;
}

CASegmentedControl* CASegmentedControl::createWithFrame(const DRect& rect, unsigned int itemsCount)
{
    CASegmentedControl* segmentedControl = new CASegmentedControl(itemsCount);
    if (segmentedControl && segmentedControl->initWithFrame(rect))
    {
        segmentedControl->autorelease();
        return segmentedControl;
    }
    CC_SAFE_DELETE(segmentedControl);
    return NULL;
}

CASegmentedControl* CASegmentedControl::createWithCenter(const DRect& rect, unsigned int itemsCount)
{
    CASegmentedControl* segmentedControl = new CASegmentedControl(itemsCount);
    if (segmentedControl && segmentedControl->initWithCenter(rect))
    {
        segmentedControl->autorelease();
        return segmentedControl;
    }
    CC_SAFE_DELETE(segmentedControl);
    return NULL;
}

CASegmentedControl* CASegmentedControl::createWithLayout(const CrossApp::DLayout &layout, unsigned int itemsCount)
{
    CASegmentedControl* segmentedControl = new CASegmentedControl(itemsCount);
    if (segmentedControl && segmentedControl->initWithLayout(layout))
    {
        segmentedControl->autorelease();
        return segmentedControl;
    }
    CC_SAFE_DELETE(segmentedControl);
    return NULL;
}

bool CASegmentedControl::init()
{
    if (!CAControl::init())
    {
        return false;
    }
    
    this->initWithData();
    this->initWithView();

    return true;
}


void CASegmentedControl::initWithData()
{
    for (size_t i=0; i<m_nItemsCount; i++)
    {
        m_vTitles.push_back("");
        m_vNormalImages.pushBack(nullptr);
        m_vSelectedImages.pushBack(nullptr);
        m_vItemContentOffsets.push_back(DSizeZero);
        m_vItemImageSizes.push_back(DSizeZero);
        m_vItemTouchEnableds.push_back(true);
        m_vSegmentItemsTitles.pushBack(nullptr);
    }
}

void CASegmentedControl::initWithView()
{
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CASegmentedControl");
    m_pBackgroundView = CAScale9ImageView::createWithFrame(this->getBounds());
    m_pBackgroundView->setVisible(true);
    this->insertSubview(m_pBackgroundView, -2);
    m_pBackgroundView->setImage(CAImage::create(map.at("backgroundView_normal")));
    
    for (size_t i=0; i<(size_t)m_nItemsCount; i++)
    {
        CAImageView* imageView = CAImageView::create();
        this->insertSubview(imageView, -1);
        m_vItemSelectedBackgrounds.pushBack(imageView);
    }
    
    this->setSegmentItemBackgroundImage(CAImage::create(map.at("backgroundView_selected")));
    m_cTintColor = ccc4Int(CrossApp::hex2Int(map.at("tintColor")));
    m_cTextColor = ccc4Int(CrossApp::hex2Int(map.at("titleColor_normal")));
    m_cTextSelectedColor = ccc4Int(CrossApp::hex2Int(map.at("titleColor_selected")));
    for (size_t i=0; i<(size_t)m_nItemsCount; i++)
    {
        CAView* view = new CAView();
        this->addSubview(view);
        m_vSegmentItems.pushBack(view);
        view->release();
    }
    
    for (size_t i=0; i<(size_t)m_nItemsCount-1; i++)
    {
        CAView* view = CAView::createWithColor(m_cTintColor);
        this->insertSubview(view, 1);
        m_vSeparateViews.pushBack(view);
    }
}

void CASegmentedControl::setContentSize(const CrossApp::DSize &var)
{
    DSize size = var;
    if (m_bRecSpe)
    {
        size.width = MAX(size.width, size.height);
        const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CASegmentedControl");
        int h = atoi(map.at("height").c_str());
        size.height = (h == 0) ? size.height : h;
    }
    CAControl::setContentSize(size);
    
    m_fSegmentWidth = size.width / m_nItemsCount;
    
    for (size_t i=0; i<(size_t)m_nItemsCount; i++)
    {
        DRect rect;
        rect.origin.x = i * m_fSegmentWidth;
        rect.origin.y = 0;
        rect.size.width = m_fSegmentWidth;
        rect.size.height = size.height;
        m_vSegmentItems.at(i)->setFrame(rect);
        
        DRect center = m_vSegmentItems.at(i)->getBounds();
        center.origin = center.size/2 + m_vItemContentOffsets.at(i);

		CAImageView* imageView = dynamic_cast<CAImageView*>(m_vSegmentItemsTitles.at(i));
		if (imageView && imageView->getImage())
        {
            center.size = m_vItemImageSizes.at(i).equals(DSizeZero) ? imageView->getImage()->getContentSize() : m_vItemImageSizes.at(i);
        }
        else
        {
            center.size = rect.size;
        }
        if (m_vSegmentItemsTitles.at(i))
        {
            m_vSegmentItemsTitles.at(i)->setCenter(center);
        }
    }
    
    for (size_t i=0; i<(size_t)m_nItemsCount-1; i++)
    {
        DRect rect;
        rect.origin.x = (i + 1) * m_fSegmentWidth;
        rect.origin.y = size.height / 2;
        rect.size.width = m_fSeparateWidth;
        rect.size.height = size.height;
        m_vSeparateViews.at(i)->setCenter(rect);
    }
    
    for (int index = 0; index < m_vSegmentItemsTitles.size(); index++)
    {
        CC_CONTINUE_IF(!m_vNormalImages.at(index) && !m_vSelectedImages.at(index));
        DRect center = m_vSegmentItemsTitles.at(index)->getCenter();
        
        if (!m_obContentSize.equals(DSizeZero) && m_vItemImageSizes.at(index).equals(DSizeZero))
        {
            DSize size = m_obContentSize;
            DSize iSize = m_vNormalImages.at(index) ? m_vNormalImages.at(index)->getContentSize() : m_vSelectedImages.at(index)->getContentSize();
            float scaleX = size.width / iSize.width * 0.75f;
            float scaleY = size.height / iSize.height * 0.75f;
            float scale = MIN(scaleX, scaleY);
            iSize = ccpMult(iSize, scale);
            
            center.size = iSize;
            
            m_vSegmentItemsTitles.at(index)->setCenter(center);
        }
        else if (!m_vItemImageSizes.at(index).equals(DSizeZero))
        {
            center.size = m_vItemImageSizes.at(index);
            m_vSegmentItemsTitles.at(index)->setCenter(center);
        }
    }

    for (size_t i=0; i<(size_t)m_nItemsCount; i++)
    {
        m_vItemSelectedBackgrounds.at(i)->setFrame(m_vSegmentItems.at(i)->getFrame());
        if (i == (size_t)m_iSelectedIndex)
        {
            m_vItemSelectedBackgrounds.at(i)->setVisible(true);
        }
        else
        {
            m_vItemSelectedBackgrounds.at(i)->setVisible(false);
        }
    }
    this->printWithSegmentItemBackground();
    
    if (m_pBackgroundView)
    {
        m_pBackgroundView->setFrame(this->getBounds());
    }
}

unsigned int CASegmentedControl::getNumberOfSegments()
{
    return m_nItemsCount;
}

void CASegmentedControl::setSelectedAtIndex(int index)
{
    m_iSelectedIndex = index;
    
    for (size_t i=0; i<(size_t)m_nItemsCount; i++)
    {
        if (i == (size_t)m_iSelectedIndex)
        {
            m_vItemSelectedBackgrounds.at(i)->setVisible(true);
        }
        else
        {
            m_vItemSelectedBackgrounds.at(i)->setVisible(false);
        }
    }
    
    for (size_t i=0; i<(size_t)m_nItemsCount; i++)
    {
        if (CAImageView* imageView = dynamic_cast<CAImageView*>(m_vSegmentItemsTitles.at(i)))
        {
            CAImage* image = m_iSelectedIndex == i ? m_vSelectedImages.at(i) : m_vNormalImages.at(i);
            imageView->setImage(image);
            const CAColor4B& color = m_iSelectedIndex == i ? m_cImageSelectedColor : m_cImageColor;
            imageView->setColor(color);
        }
        else if (CALabel* label = dynamic_cast<CALabel*>(m_vSegmentItemsTitles.at(i)))
        {
            const CAColor4B& color = m_iSelectedIndex == i ? m_cTextSelectedColor : m_cTextColor;
            label->setColor(color);
        }
    }
}

int CASegmentedControl::getSelectedAtIndex()
{
    return m_iSelectedIndex;
}

void CASegmentedControl::setBackgroundImage(CAImage* image)
{
    m_pBackgroundView->setImage(image);
}

void CASegmentedControl::setSegmentItemBackgroundImage(CAImage* image)
{
    CC_SAFE_RELEASE(m_pSegmentItemBackgroundImage);
    CC_SAFE_RETAIN(image);
    m_pSegmentItemBackgroundImage = image;
    this->printWithSegmentItemBackground();
}

void CASegmentedControl::setTitleColor(const CAColor4B& color)
{
    m_cTextColor = color;
    for (size_t i=0; i<(size_t)m_nItemsCount; i++)
    {
        if (CALabel* label = dynamic_cast<CALabel*>(m_vSegmentItemsTitles.at(i)))
        {
            CC_CONTINUE_IF(m_iSelectedIndex == i);
            label->setColor(m_cTextColor);
        }
    }
}

const CAColor4B& CASegmentedControl::getTitleColor()
{
    return m_cTextColor;
}

void CASegmentedControl::setTitleSelectedColor(const CAColor4B& color)
{
    m_cTextSelectedColor = color;
    if (CALabel* label = dynamic_cast<CALabel*>(m_vSegmentItemsTitles.at(m_iSelectedIndex)))
    {
        label->setColor(m_cTextSelectedColor);
    }
}

const CAColor4B& CASegmentedControl::getTitleSelectedColor()
{
    return m_cTextSelectedColor;
}

void CASegmentedControl::setImageColor(const CAColor4B& color)
{
    m_cImageColor = color;
    for (size_t i=0; i<(size_t)m_nItemsCount; i++)
    {
        if (CAImageView* imageView = dynamic_cast<CAImageView*>(m_vSegmentItemsTitles.at(i)))
        {
            CC_CONTINUE_IF(m_iSelectedIndex == i);
            imageView->setColor(m_cImageColor);
        }
    }
}
const CAColor4B& CASegmentedControl::getImageColor()
{
    return m_cImageColor;
}

void CASegmentedControl::setImageSelectedColor(const CAColor4B& color)
{
    m_cImageSelectedColor = color;
    if (CAImageView* imageView = dynamic_cast<CAImageView*>(m_vSegmentItemsTitles.at(m_iSelectedIndex)))
    {
        imageView->setColor(m_cImageSelectedColor);
    }
}

const CAColor4B& CASegmentedControl::getImageSelectedColor()
{
    return m_cImageSelectedColor;
}

void CASegmentedControl::setTintColor(const CAColor4B& color)
{
    m_cTintColor = color;
    for (int i=0; i<m_nItemsCount-1; i++)
    {
        m_vSeparateViews.at(i)->setColor(m_cTintColor);
    }
}

void CASegmentedControl::setTitleForSegmentAtIndex(const std::string& title, int index)
{
    CC_RETURN_IF(index >= (int)m_nItemsCount);
    CC_RETURN_IF(index < 0);
    
    m_vSegmentItems.at(index)->removeAllSubviews();
    DRect center = m_vSegmentItems.at(index)->getBounds();
    center.origin = center.size/2 + m_vItemContentOffsets.at(index);
    CALabel* label = CALabel::createWithCenter(center);
    label->setFont(m_obTitleFont);
    label->setColor(m_iSelectedIndex == index ? m_cTextSelectedColor : m_cTextColor);
    label->setTextAlignment(CATextAlignment::Center);
    label->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
    label->setText(title);
    m_vSegmentItems.at(index)->addSubview(label);
    m_vSegmentItemsTitles.replace(index, label);
}

std::string CASegmentedControl::getTitleForSegmentAtIndex(int index)
{
    std::string text;
    if (CALabel* label = dynamic_cast<CALabel*>(m_vSegmentItemsTitles.at(index)))
    {
        text = label->getText();
    }
    return text;
}

void CASegmentedControl::setImageForSegmentAtIndex(CAImage* image, int index, CAControl::State state)
{
    CC_RETURN_IF(index >= (int)m_nItemsCount);
    CC_RETURN_IF(index < 0);
    
    m_vSegmentItems.at(index)->removeAllSubviews();
    DRect center = m_vSegmentItems.at(index)->getBounds();
    center.origin = center.size/2 + m_vItemContentOffsets.at(index);
    
    if (!m_obContentSize.equals(DSizeZero) && m_vItemImageSizes.at(index).equals(DSizeZero))
    {
        DSize size = m_obContentSize;
        DSize iSize = image->getContentSize();
        float scaleX = size.width / iSize.width * 0.75f;
        float scaleY = size.height / iSize.height * 0.75f;
        float scale = MIN(scaleX, scaleY);
        iSize = ccpMult(iSize, scale);
        
        center.size = iSize;
    }
    
    center.size = m_vItemImageSizes.at(index);
    
    CAImageView* imageView = CAImageView::create();
    imageView->setCenter(center);
    imageView->setColor(m_iSelectedIndex == index ? m_cImageSelectedColor : m_cImageColor);
    m_vSegmentItems.at(index)->addSubview(imageView);
    m_vSegmentItemsTitles.replace(index, imageView);

    switch (state)
    {
        case CAControl::State::Normal:
        {
            m_vNormalImages.replace(index, image);
            if (m_iSelectedIndex != index)
            {
                imageView->setImage(image);
            }
        }
            break;
        case CAControl::State::Selected:
        {
            m_vSelectedImages.replace(index, image);
            if (m_iSelectedIndex == index)
            {
                imageView->setImage(image);
            }
        }
            break;
        default:
            break;
    }
    
}

CAImage* CASegmentedControl::getImageForSegmentAtIndex(int index)
{
    CAImage* image = NULL;
    if (index >=0 && index < (int)m_nItemsCount)
    {
        if (CAImageView* imageView = dynamic_cast<CAImageView*>(m_vSegmentItemsTitles.at(index)))
        {
            image = imageView->getImage();
        }
    }
    return image;
}

void CASegmentedControl::setContentOffsetForSegmentAtIndex(DSize offset, int index)
{
    CC_RETURN_IF(index >= (int)m_nItemsCount);
    CC_RETURN_IF(index < 0);
    m_vItemContentOffsets.at(index) = offset;
	CAView* item = m_vSegmentItemsTitles.at(index);
	if (item)
	{
		DSize size = item->getCenterOrigin();
		size.width += offset.width;
		size.height += offset.height;
		item->setCenterOrigin(size);
	}
}

DSize CASegmentedControl::getContentOffsetForSegmentAtIndex(int index)
{
    DSize offset = DSizeZero;
    if (index >=0 && index < (int)m_nItemsCount)
    {
        offset = m_vItemContentOffsets.at(index);
    }
    return offset;
}

void CASegmentedControl::setImageSizeAtIndex(DSize size, int index)
{
    CC_RETURN_IF(index >= (int)m_nItemsCount);
    CC_RETURN_IF(index < 0);
    m_vItemImageSizes.at(index) = size;
	CAView* item = m_vSegmentItemsTitles.at(index);
	if (item)
	{
		DRect rect = item->getCenter();
		rect.size.width = size.width;
		rect.size.height = size.height;
		item->setCenter(rect);
	}
}

void CASegmentedControl::setTitleFontName(std::string titleName)
{
    m_obTitleFont.fontName = titleName;
    for (size_t i=0; i<(size_t)m_nItemsCount; i++)
    {
        if (CALabel* label = dynamic_cast<CALabel*>(m_vSegmentItemsTitles.at(i)))
        {
            label->setFont(m_obTitleFont);
        }
    }
}

void CASegmentedControl::setTitleFontSize(float titleSize)
{
    m_obTitleFont.fontSize = titleSize;
    for (size_t i=0; i<(size_t)m_nItemsCount; i++)
    {
        if (CALabel* label = dynamic_cast<CALabel*>(m_vSegmentItemsTitles.at(i)))
        {
            label->setFont(m_obTitleFont);
        }
    }
}

void CASegmentedControl::setTitleFont(const CAFont& font)
{
    m_obTitleFont = font;
    for (size_t i=0; i<(size_t)m_nItemsCount; i++)
    {
        if (CALabel* label = dynamic_cast<CALabel*>(m_vSegmentItemsTitles.at(i)))
        {
            label->setFont(m_obTitleFont);
        }
    }
}

const CAFont& CASegmentedControl::getTitleFont()
{
    return m_obTitleFont;
}

void CASegmentedControl::setEnabledForSegmentAtIndex(bool isEnable, int index)
{
    CC_RETURN_IF(index >= (int)m_nItemsCount);
    CC_RETURN_IF(index < 0);
    m_vItemTouchEnableds.at(index) = isEnable;
}

bool CASegmentedControl::isEnabledForSegmentAtIndex(int index)
{
    bool isEnable = true;
    if (index >=0 && index < (int)m_nItemsCount)
    {
        isEnable = m_vItemTouchEnableds.at(index);
    }
    return isEnable;
}

void CASegmentedControl::printWithSegmentItemBackground()
{
    CC_RETURN_IF(m_obContentSize.equals(DSizeZero));
    CC_RETURN_IF(!m_pSegmentItemBackgroundImage);
    
    DRect rect = DRectZero;
    rect.size = m_obContentSize;

    CAScale9ImageView* scale9ImageView = CAScale9ImageView::createWithFrame(rect);
    scale9ImageView->setImage(m_pSegmentItemBackgroundImage);

    CARenderImage* render = CARenderImage::create(rect.size.width, rect.size.height);
    render->printscreenWithView(scale9ImageView);
    
    CC_SAFE_RELEASE(m_pNewSegmentItemBackgroundImage);
    m_pNewSegmentItemBackgroundImage = render->getImageView()->getImage();
    CC_SAFE_RETAIN(m_pNewSegmentItemBackgroundImage);
    
    for (size_t i=0; i<(size_t)m_nItemsCount; i++)
    {
        m_vItemSelectedBackgrounds.at(i)->setImage(m_pNewSegmentItemBackgroundImage);
        DRect rect = m_vSegmentItems.at(i)->getFrame();
        m_vItemSelectedBackgrounds.at(i)->setImageRect(rect);
    }
}


int CASegmentedControl::pickWithPoint(const DPoint& point)
{
    int res = m_iSelectedIndex;
    for (size_t i=0; i<(size_t)m_nItemsCount; i++)
    {
        CC_CONTINUE_IF(m_vItemTouchEnableds.at(i) == false);
        if (m_vSegmentItems.at(i)->getFrame().containsPoint(point))
        {
            res = (int)i;
            continue;
        }
    }
    return res;
}

void CASegmentedControl::touchVithIndex(int index)
{
    m_iTouchIndex = index;
    for (size_t i=0; i<(size_t)m_nItemsCount; i++)
    {
        CC_CONTINUE_IF(i == (size_t)m_iSelectedIndex);
        CC_CONTINUE_IF(m_vItemTouchEnableds.at(i) == false);
        if (i == (size_t)m_iTouchIndex)
        {
            m_vItemSelectedBackgrounds.at(i)->setVisible(true);
        }
        else
        {
            m_vItemSelectedBackgrounds.at(i)->setVisible(false);
        }
    }
    
    
    for (size_t i=0; i<(size_t)m_nItemsCount; i++)
    {
        CC_CONTINUE_IF(i == (size_t)m_iSelectedIndex);
        CC_CONTINUE_IF(m_vItemTouchEnableds.at(i) == false);
        if (CAImageView* imageView = dynamic_cast<CAImageView*>(m_vSegmentItemsTitles.at(i)))
        {
            CAImage* image = (size_t)m_iTouchIndex == i ? m_vSelectedImages.at(i) : m_vNormalImages.at(i);
            imageView->setImage(image);
            const CAColor4B& color = (size_t)m_iTouchIndex == i ? m_cImageSelectedColor : m_cImageColor;
            imageView->setColor(color);
        }
        else if (CALabel* label = dynamic_cast<CALabel*>(m_vSegmentItemsTitles.at(i)))
        {
            const CAColor4B& color = (size_t)m_iTouchIndex == i ? m_cTextSelectedColor : m_cTextColor;
            label->setColor(color);
        }
    }
}

bool CASegmentedControl::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    int index = this->pickWithPoint(point);
    this->touchVithIndex(index);
    return true;
}

void CASegmentedControl::ccTouchMoved(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);

    int index = 0;
    if (this->getBounds().containsPoint(point))
    {
        index = this->pickWithPoint(point);
    }
    else
    {
        index = m_iSelectedIndex;
    }
    this->touchVithIndex(index);
}

void CASegmentedControl::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    bool changed = (m_iTouchIndex != m_iSelectedIndex);
    
    this->setSelectedAtIndex(m_iTouchIndex);
    
    if (changed && m_function)
    {
        m_function(m_iSelectedIndex);
    }
}

void CASegmentedControl::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    this->setSelectedAtIndex(m_iSelectedIndex);
}

void CASegmentedControl::setTarget(const std::function<void(int)>& function)
{
    if (function) m_function = function;
}

NS_CC_END
