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

using namespace std;
NS_CC_BEGIN

CASegmentedControl::CASegmentedControl(unsigned int itemsCount)
    : CAControl()
    , m_nItemsCount(itemsCount)
    , m_pBackgroundView(NULL)
    , m_fSeparateWidth(s_px_to_dip(2))
    , m_iSelectedIndex(-1)
    , m_iTouchIndex(0)
    , m_cTextColor(ccc4(54, 195, 240, 255))
    , m_cTextSelectedColor(CAColor_white)
    , m_cImageColor(CAColor_white)
    , m_cImageSelectedColor(CAColor_white)
    , m_cTintColor(ccc4(54, 195, 240, 255))
    , m_pTarget(NULL)
    , m_pCallFunc(NULL)
    , m_pSegmentItemBackgroundImage(NULL)
    , m_sTitleFontName("")
    , m_fTitleFontSize(24)
{

}

CASegmentedControl::~CASegmentedControl()
{
    m_vTitles.clear();
    m_vNormalImages.clear();
    m_vSelectedImages.clear();
    m_vIsEnabled.clear();
    m_vContentOffset.clear();
    m_vSegments.clear();
    m_vSeparateView.clear();
    m_vSegmentWidth.clear();
    m_vImageSize.clear();
	m_vSegmentItemBackground.clear();
 	m_pCallFunc = NULL;
}

void CASegmentedControl::removeAllSeparateImage()
{
    for (std::vector<CAView*>::iterator itr = m_vSeparateView.begin(); itr != m_vSeparateView.end(); ++itr)
    {
        (*itr)->removeFromSuperview();
    }
    m_vSeparateView.clear();
}

void CASegmentedControl::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();
}

void CASegmentedControl::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();
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

bool CASegmentedControl::initWithFrame(const DRect& rect)
{
    if (!CAControl::initWithFrame(rect))
    {
        return false;
    }
    this->setBackgroundImage( CAImage::create("source_material/btn_rounded_normal.png") );

    const float elemWidth = this->getBounds().size.width / m_nItemsCount;
    DRect elemFrame = DRect(0, 0, this->getBounds().size.width/m_nItemsCount, this->getBounds().size.height);
    for (int i = 0; i < m_nItemsCount; ++i)
    {
        CAView* segment = this->createDefaultSegment(i);
        if (segment)
        {
            segment->setFrame(elemFrame);
            m_vSegments.pushBack(segment);
            this->insertSubview(segment, 1);
        }
        elemFrame.origin.x += elemWidth;
    }
    setSegmentItemBackgroundImage(CAImage::create("source_material/btn_rounded_highlighted.png"));
    createSeparate();
    return true;
}

bool CASegmentedControl::initWithCenter(const DRect& rect)
{
    if (!CAControl::initWithCenter(rect))
    {
        return false;
    }
    this->setBackgroundImage( CAImage::create("source_material/btn_rounded_normal.png") );

    const float elemWidth = this->getBounds().size.width / m_nItemsCount;
    DRect elemFrame = DRect(0, 0, this->getBounds().size.width/m_nItemsCount, this->getBounds().size.height);
    for (int i = 0; i < m_nItemsCount; ++i)
    {
        CAView *segment = this->createDefaultSegment(i);
        if (segment)
        {
            segment->setFrame(elemFrame);
            m_vSegments.pushBack(segment);
            this->insertSubview(segment, 1);
        }
        elemFrame.origin.x += elemWidth;
    }
    setSegmentItemBackgroundImage(CAImage::create("source_material/btn_rounded_highlighted.png"));
    createSeparate();
    return true;
}

#pragma mark --
void CASegmentedControl::setTitleColor(const CAColor4B& color)
{
    m_cTextColor = color;
    for(int index=0; index<m_vSegments.size(); ++index)
    {
        CC_CONTINUE_IF(m_iSelectedIndex == index);
        CALabel* label = m_vTitles.at(index);
        if(label != NULL)
			label->setColor(m_cTextColor);
    }
}

const CAColor4B& CASegmentedControl::getTitleColor()
{
    return m_cTextColor;
}

void CASegmentedControl::setTitleSelectedColor(const CAColor4B& color)
{
    m_cTextSelectedColor = color;
    CC_RETURN_IF(m_iSelectedIndex == -1);
    CALabel* label = m_vTitles.at(m_iSelectedIndex);
	if (label != NULL)
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
    for(int index=0; index<m_vSegments.size(); ++index)
    {
        CC_CONTINUE_IF(m_iSelectedIndex == index);
        CAImageView* imageView = dynamic_cast<CAImageView*>(m_vSegments.at(index)->getSubviewByTextTag("image"));
        if(imageView != NULL)
            imageView->setColor(m_cImageColor);
    }
}
const CAColor4B& CASegmentedControl::getImageColor()
{
    return m_cImageColor;
}

void CASegmentedControl::setImageSelectedColor(const CAColor4B& color)
{
    m_cImageSelectedColor = color;
    CC_RETURN_IF(m_iSelectedIndex == -1);
    CAImageView* imageView = dynamic_cast<CAImageView*>(m_vSegments.at(m_iSelectedIndex)->getSubviewByTextTag("image"));
    if(imageView != NULL)
        imageView->setColor(m_cImageSelectedColor);
}
const CAColor4B& CASegmentedControl::getImageSelectedColor()
{
    return m_cImageSelectedColor;
}

void CASegmentedControl::setTintColor(const CAColor4B& color)
{
    m_cTintColor = color;
    cleanAllSeparate();
    createSeparate();
}

void CASegmentedControl::setSelectedAtIndex(int index)
{
    m_iSelectedIndex = index;
    for(int i=0; i<m_vSegments.size(); ++i)
    {
        refreshSegmentItemByIndex(i, CAControlStateNormal);
        setSegmentItemBackgroundVisibleWithIndex(false, i);
    }
    refreshSegmentItemByIndex(index, CAControlStateSelected);
    setSegmentItemBackgroundVisibleWithIndex(true, index);
}

int CASegmentedControl::getSelectedAtIndex()
{
    return m_iSelectedIndex;
}

int CASegmentedControl::getNumberOfSegments()
{
    return m_nItemsCount;
}

void CASegmentedControl::insertSegmentWithTitleAtIndex(const std::string& title, int index)
{
    if(index >= 0 && index <= m_vSegments.size())
    {
        if(index <= m_iSelectedIndex)
        {
            m_iSelectedIndex++;
        }
        CAView *segment = this->createDefaultSegment(index);
        if (segment)
        {
            segment->setFrame(DRect(0,0,this->getBounds().size.width/m_vSegments.size(), this->getBounds().size.height));
            m_vSegments.insert(index, segment);
			m_nItemsCount++;
            this->insertSubview(segment, 1);
        }
        refreshAllSegmentItemBounds();
        setTitleForSegmentAtIndex(title, index);
        refreshAllSegmentItemPosition();
        setSegmentItemBackgroundImage(m_pSegmentItemBackgroundImage);
        
        cleanAllSeparate();
        createSeparate();
    }
}

void CASegmentedControl::insertSegmentWithImageAtIndex(CAImage* image, int index, CAControlState controlState)
{
    if(index >= 0 && index <= m_vSegments.size())
    {
        if(index <= m_iSelectedIndex)
        {
            m_iSelectedIndex++;
        }
        CAView *segment = this->createDefaultSegment(index);
        if (segment)
        {
            segment->setFrame(DRect(0,0,this->getBounds().size.width/m_vSegments.size(), this->getBounds().size.height));
            m_vSegments.insert(index, segment);
			m_nItemsCount++;
            this->insertSubview(segment, 1);
        }
        refreshAllSegmentItemBounds();
        setImageForSegmentAtIndex(image, index, controlState);
        refreshAllSegmentItemPosition();
        setSegmentItemBackgroundImage(m_pSegmentItemBackgroundImage);
        
        cleanAllSeparate();
        createSeparate();
    }
    refreshSegmentItemByIndex(m_iSelectedIndex, CAControlStateSelected);
}

void CASegmentedControl::refreshAllLable()
{
    for(int i=0; i<m_vTitles.size(); i++)
    {
        DPoint point = m_vSegments.at(i)->getBounds().origin;
        DSize size = m_vSegments.at(i)->getBounds().size;
        DSize contentOffset = m_vContentOffset.at(i);
        CALabel* label = m_vTitles.at(i);
        if(label != NULL)
        {
            label->setCenter(DRect(size.width*0.5f + contentOffset.width,
                                    size.height*0.5f + contentOffset.height,
                                    size.width,
                                    size.height));
        }
    }
}

void CASegmentedControl::setTitleForSegmentAtIndex(const std::string& title, int index)
{
    if(index < m_vSegments.size())
    {
        DPoint point = m_vSegments.at(index)->getBounds().origin;
        DSize size = m_vSegments.at(index)->getBounds().size;
        CALabel* label = CALabel::createWithCenter(DRect(m_vContentOffset.at(index).width+ size.width, m_vContentOffset.at(index).height + size.height, size.width, size.height));
        label->setTextAlignment(CATextAlignmentCenter);
        label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        label->setText(title);
        label->setFontSize(m_fTitleFontSize);
        label->setFontName(m_sTitleFontName);
		label->setColor(m_cTextColor);
        label->retain();
        CAVector<CALabel*>::iterator itr = m_vTitles.begin()+index;
        if(*itr != NULL)
        {
            (*itr)->removeFromSuperview();
        }
        (*itr) = label;
        m_vSegments.at(index)->addSubview(*itr);
        refreshSegmentItemByIndex(index, CAControlStateNormal);
    }
    if(m_iSelectedIndex>=0 && m_iSelectedIndex<m_vSegments.size())
        refreshSegmentItemByIndex(m_iSelectedIndex, CAControlStateSelected);
}

std::string CASegmentedControl::getTitleForSegmentAtIndex(int index)
{
    if (index >= 0 && index < (int)m_vTitles.size())
    {
        CALabel* label = m_vTitles.at(index);
        if (label)
        {
            return label->getText();
        }
    }
    return "";
}

void CASegmentedControl::setImageForSegmentAtIndex(CAImage* image, int index, CAControlState controlState)
{
    if(index >= 0 && index < (int)m_vSegments.size())
    {
        CALabel* label = m_vTitles.at(index);
        if(label)
        {
            label->removeFromSuperview();
            CC_SAFE_RELEASE(label);
            m_vTitles.replace(index, NULL);
        }
        if( CAControlStateNormal == controlState )
        {
            m_vNormalImages.replace(index, image);
            if (m_vSelectedImages.at(index) == NULL)
            {
                m_vSelectedImages.replace(index, image);
            }
        }
        else if( CAControlStateSelected == controlState )
        {
            m_vSelectedImages.replace(index, image);
        }

        refreshSegmentItemByIndex(index, controlState);
        refreshSegmentItemByIndex(index, CAControlStateNormal);
    }
    refreshSegmentItemByIndex(m_iSelectedIndex, CAControlStateSelected);
}

CAImage* CASegmentedControl::getImageForSegmentAtIndex(int index)
{
    CAImage* image = NULL;
    if(index >= 0 && index < m_vSegments.size())
    {
        CAVector<CAImage*>::iterator itr = m_vNormalImages.begin()+index;
        image = (*itr);
    }
    return image;
}

void CASegmentedControl::setWidthForSegmentAtIndex(float width, int index)
{
    if(index >= 0 && index < m_vSegments.size())
    {
        m_vSegmentWidth.at(index) = width;
        
        refreshAllSegmentItemBounds();
        for (int i=0; i<(int)m_vSegments.size(); ++i)
        {
            refreshSegmentItemByIndex(i, CAControlStateNormal);
        }
        refreshAllSegmentItemPosition();
        setSegmentItemBackgroundImage(m_pSegmentItemBackgroundImage);
        
        cleanAllSeparate();
        createSeparate();
    }
    refreshSegmentItemByIndex(m_iSelectedIndex, CAControlStateSelected);
}

float CASegmentedControl::getWidthForSegmentAtIndex(int index)
{
    float width = 0;
    if(index >= 0 && index < (int)m_vSegments.size())
    {
        width = m_vSegments.at(index)->getBounds().size.width;
    }
    return width;
}

void CASegmentedControl::setContentOffsetForSegmentAtIndex(DSize offset, int index)
{
    if(index >= 0 && index < m_vSegments.size())
    {
        m_vContentOffset.at(index) = offset;
        
        CAObject* object = getObjectByIndex(index, CAControlStateNormal);
        
        if(CALabel* label = dynamic_cast<CALabel*>(object))
        {
            label->setFrameOrigin(m_vContentOffset.at(index));
        }
        else if(CAImage* image = dynamic_cast<CAImage*>(object))
        {
            m_vSegments.at(index)->removeAllSubviews();
            CAImageView* imageView = CAImageView::createWithImage(image);
            imageView->setFrameOrigin(m_vContentOffset.at(index));
            m_vSegments.at(index)->addSubview(imageView);
        }
    }
}

DSize CASegmentedControl::getContentOffsetForSegmentAtIndex(int index)
{
    DSize offset = DSizeZero;
    if(index >= 0 && index < m_vSegments.size())
    {
        offset = m_vContentOffset.at(index);
    }
    return offset;
}

void CASegmentedControl::setEnabledForSegmentAtIndex(bool isEnable, int index)
{
    if(index >= 0 && index < m_vSegments.size())
    {
        m_vIsEnabled.at(index) = isEnable;
        refreshSegmentItemByIndex(index, CAControlStateNormal);
    }
    refreshSegmentItemByIndex(m_iSelectedIndex, CAControlStateSelected);
}

bool CASegmentedControl::isEnabledForSegmentAtIndex(int index)
{
    bool isEnable = true;
    if(index >= 0 && index < m_vSegments.size())
    {
        isEnable = m_vIsEnabled.at(index);
    }
    return isEnable;
}

void CASegmentedControl::removeSegmentAtIndex(int index)
{
    if((index<0 || index >= m_vSegments.size()) && m_vSegments.size() == 1)
        return;
    {
        m_vTitles.at(index)->removeFromSuperview();
        m_vTitles.erase(index);
    }
    
    {
        m_vNormalImages.erase(index);
    }
    
    {
        std::vector<bool>::iterator itr = m_vIsEnabled.begin() + index;
        m_vIsEnabled.erase(itr);
    }
    
    {
        std::vector<DSize>::iterator itr = m_vContentOffset.begin() + index;
        m_vContentOffset.erase(itr);
    }
    
    {
        std::vector<DSize>::iterator itr = m_vImageSize.begin() + index;
        m_vImageSize.erase(itr);
    }
    
    {
        std::vector<float>::iterator itr = m_vSegmentWidth.begin() + index;
        m_vSegmentWidth.erase(itr);
    }
    
    {
        m_vSegmentItemBackground.at(index)->removeFromSuperview();
        m_vSegmentItemBackground.erase(index);
    }
    
    {
        m_vSegments.at(index)->removeFromSuperview();
        m_vSegments.erase(index);
		m_nItemsCount--;
    }
    
    if(index <= m_iSelectedIndex)
    {
        m_iSelectedIndex--;
    }

    refreshAllSegmentItemBounds();
    refreshAllSegmentItemPosition();
    setSegmentItemBackgroundImage(m_pSegmentItemBackgroundImage);
    
    cleanAllSeparate();
    createSeparate();
}


void CASegmentedControl::setContentSize(const CrossApp::DSize &var)
{
    DSize size = var;
//    size.height = MAX(size.height, 58);
//    size.width = MAX(size.width, size.height * 2);
    CAControl::setContentSize(size);
    if (m_pBackgroundView)
    {
        m_pBackgroundView->setFrame(this->getBounds());
    }
    
    refreshAllSegmentItemBounds();
    refreshAllLable();
    float length = 0;
    CAVector<CAView*>::iterator itr = m_vSegments.begin();
    for(; itr != m_vSegments.end(); ++itr)
    {
        DRect rect = DRect(length, 0, (*itr)->getBounds().size.width, (*itr)->getBounds().size.height);
        length += (*itr)->getBounds().size.width;
        (*itr)->setFrame(rect);
    }
    refreshAllSegmentItemBackgroundPosition();
    cleanAllSeparate();
    createSeparate();
}

CAView* CASegmentedControl::getBackgroundView()
{
    return m_pBackgroundView;
}

void CASegmentedControl::setBackgroundImage(CAImage* image)
{
    this->removeSubview(m_pBackgroundView);
    CAScale9ImageView* backGroundImageView = CAScale9ImageView::createWithFrame(this->getBounds());
    backGroundImageView->setImage(image);
    backGroundImageView->setVisible(true);
    m_pBackgroundView = backGroundImageView;
    this->insertSubview(m_pBackgroundView, -2);
}

void CASegmentedControl::createSeparate()
{
    DRect frame = DRect(0, 0, m_fSeparateWidth, this->getBounds().size.height);
    float length = 0;
    size_t count = m_vSegments.size();
	CAVector<CAView *>::iterator itr = m_vSegments.begin();
    for(; itr != m_vSegments.end() && count != 1; ++ itr, --count)
    {
        length += (*itr)->getBounds().size.width;
        frame.origin.x = length;
        CAView* separateImageView = CAView::createWithFrame(frame);
        separateImageView->setColor(m_cTintColor);
        this->insertSubview(separateImageView, 100);
        m_vSeparateView.pushBack(separateImageView);
    }
}

void CASegmentedControl::cleanAllSeparate()
{
	CAVector<CAView *>::iterator itr = m_vSeparateView.begin();
    for(; itr != m_vSeparateView.end(); ++itr)
    {
        (*itr)->removeFromSuperview();
    }
    m_vSeparateView.clear();
}

void CASegmentedControl::refreshAllSegmentItemBounds()
{
    CAVector<CAView*>::iterator itr_segment= m_vSegments.begin();
    for(int index=0; itr_segment!=m_vSegments.end(); ++itr_segment, ++index)
    {
        float setWidth = 0;
        int   setWidthCount = 0;
        std::vector<float>::iterator itr_width = m_vSegmentWidth.begin();
        for(; itr_width!=m_vSegmentWidth.end(); ++itr_width)
        {
            if(*itr_width != -1)
            {
                setWidth+= *itr_width;
                setWidthCount++;
            }
        }
        
        float segmentWidth = m_vSegmentWidth.at(index);
        if(segmentWidth == -1)
        {
            segmentWidth = (this->getBounds().size.width - setWidth)/(m_vSegmentWidth.size()-setWidthCount);
        }
        (*itr_segment)->setBounds(DRect((*itr_segment)->getBounds().origin.x,
                                             (*itr_segment)->getBounds().origin.y,
                                             segmentWidth,
                                             (*itr_segment)->getBounds().size.height));
    }
}

void CASegmentedControl::refreshAllSegmentItemPosition()
{
    float length = 0;
    CAVector<CAView*>::iterator itr = m_vSegments.begin();
    for(; itr != m_vSegments.end(); ++itr)
    {
        DRect rect = DRect(length, 0, (*itr)->getBounds().size.width, (*itr)->getBounds().size.height);
        length += (*itr)->getBounds().size.width;
        (*itr)->setFrame(rect);
    }
    this->setFrame(DRect(this->getFrameOrigin().x, this->getFrameOrigin().y, length, this->getBounds().size.height));
}

void CASegmentedControl::refreshAllSegmentItemBackgroundPosition()
{
    float length = 0;
    CAVector<CAView*>::iterator itr = m_vSegments.begin();
    CAVector<CAView*>::iterator itr_bg = m_vSegmentItemBackground.begin();
    for(; itr != m_vSegments.end(); ++itr, ++itr_bg)
    {
        DRect rect = DRect(length, 0, (*itr)->getFrame().size.width, (*itr)->getBounds().size.height);
        length += (*itr)->getFrame().size.width;
        (*itr_bg)->setFrame(rect);
    }
}

void CASegmentedControl::refreshSegmentItemByPoint(DPoint point, CAControlState controlState)
{
    int index =getSegmentItemIndexByPoint(point);
    refreshSegmentItemByIndex(index, controlState);
}

void CASegmentedControl::refreshSegmentItemByIndex(int index, CAControlState controlState)
{
    index = MAX(index, 0);
    index = MIN(index, m_vSegments.size());

    CAObject* object = getObjectByIndex(index, controlState);
    DSize segmentSize = m_vSegments.at(index)->getBounds().size;
    DSize contentOffset = m_vContentOffset.at(index);
    DSize imageSize = m_vImageSize.at(index);
    
    if(CALabel* label = dynamic_cast<CALabel*>(object))
    {
        label->setCenter(DRect(segmentSize.width*0.5f + contentOffset.width,
                                segmentSize.height*0.5f + contentOffset.height,
                                segmentSize.width,
                                segmentSize.height));
    }
    else if(CAImage* image = dynamic_cast<CAImage*>(object))
    {
        DSize imageSelfSize =  DSize(image->getContentSize().width, image->getContentSize().height);
    
        DSize segmentSizeDip = DSize( segmentSize );
        float width = imageSelfSize.width;
        float height = imageSelfSize.height;
        if(imageSelfSize.width > segmentSizeDip.width)
        {
            width = segmentSizeDip.width;
        }
        if(imageSelfSize.height > segmentSizeDip.height)
        {
            height = segmentSizeDip.height;
        }
        m_vSegments.at(index)->removeAllSubviews();
        CAImageView* imageView = CAImageView::createWithImage(image);
        if( imageSize.width || imageSize.height)
        {
            width = imageSize.width;
            height = imageSize.height;
        }
        imageView->setCenter(DRect(segmentSize.width*0.5f + contentOffset.width,
                                        segmentSize.height*0.5f + contentOffset.height,
                                        width,
                                        height));
        imageView->setTextTag("image");
        m_vSegments.at(index)->addSubview(imageView);
        
        if (controlState == CAControlStateSelected || controlState ==  CAControlStateHighlighted)
        {
            imageView->setColor(m_cImageSelectedColor);
        }
        else
        {
            imageView->setColor(m_cImageColor);
        }
        
    }
    else
    {
        m_vSegments.at(index)->removeAllSubviews();
    }
}

int CASegmentedControl::getSegmentItemIndexByPoint(DPoint point)
{
    if(point.x < 0)
        return -1;
    
    int index = 0;
    float length = 0;
    CAVector<CAView*>::iterator itr = m_vSegments.begin();
    for(; itr != m_vSegments.end(); ++itr)
    {
        length += (*itr)->getBounds().size.width;
        if(point.x < length)
        {
            break;
        }
        index++;
    }
    
    return index;
}

CAObject* CASegmentedControl::getObjectByIndex(int index, CAControlState controlState)
{
    CAObject* object = NULL;
    if(index < m_vTitles.size() && index < m_vNormalImages.size())
    {
        if(CAControlStateNormal == controlState)
        {
            if(m_vTitles.at(index) != NULL)
            {
                object = m_vTitles.at(index);
				((CALabel*)object)->setColor(m_cTextColor);
            }
            else
            {
                object = m_vNormalImages.at(index);
            }
        }
        else if(CAControlStateHighlighted == controlState)
        {
            if(m_vTitles.at(index) != NULL)
            {
                object = m_vTitles.at(index);
				((CALabel*)object)->setColor(m_cTextSelectedColor);
            }
            else
            {
                object = m_vNormalImages.at(index);
            }
        }
        else if(CAControlStateSelected == controlState)
        {
            if(m_vTitles.at(index) != NULL)
            {
                object = m_vTitles.at(index);
				((CALabel*)object)->setColor(m_cTextSelectedColor);
            }
            else
            {
                object = m_vSelectedImages.at(index);
            }
        }
    }
    return object;
}

void CASegmentedControl::addTarget(CAObject* target, SEL_CASegmentedControl selector)
{
    m_pTarget = target;
    m_pCallFunc = selector;
}

void CASegmentedControl::callFunc(CAObject* object, int index)
{
    if(m_pTarget && m_pCallFunc)
    {
        ((CAObject *)m_pTarget->*m_pCallFunc)(this, index);
    }
}

CAView* CASegmentedControl::createDefaultSegment(int index)
{
    CAView* segment = CAView::createWithColor(CAColor_clear);
    if(segment)
    {
        m_vTitles.insert(index, NULL);

        m_vNormalImages.insert(index, NULL);
        
        m_vSelectedImages.insert(index, NULL);
        
        std::vector<bool>::iterator itr = m_vIsEnabled.begin()+index;
        m_vIsEnabled.insert(itr, true);
        
        std::vector<DSize>::iterator itr_offset = m_vContentOffset.begin()+index;
        m_vContentOffset.insert(itr_offset, DSize(0, 0));
        
        std::vector<DSize>::iterator itr_imageSize = m_vImageSize.begin()+index;
        m_vImageSize.insert(itr_imageSize, DSize(0, 0));
        
        std::vector<float>::iterator itr_width = m_vSegmentWidth.begin()+index;
        m_vSegmentWidth.insert(itr_width, -1);
        
        CAImageView* imageView = CAImageView::createWithImage(NULL);
        m_vSegmentItemBackground.insert(index, imageView);
        this->insertSubview(imageView, -1);
    }
    return segment;
}

void CASegmentedControl::setSegmentItemBackgroundImage(CAImage* image)
{
    m_pSegmentItemBackgroundImage = image;
    
    if (!m_vSegmentItemBackground.empty())
    {
        CAVector<CAView*>::iterator itr = m_vSegmentItemBackground.begin();
        for (; itr != m_vSegmentItemBackground.end(); itr++)
        {
            this->removeSubview(*itr);
        }
        m_vSegmentItemBackground.clear();
        for (int i = 0; i < m_nItemsCount; i++)
        {
            CAImageView* imageView = (CAImageView*)getTailorImageAtIndex(i, image);
            m_vSegmentItemBackground.pushBack(imageView);
            imageView->setVisible(false);
            this->insertSubview(imageView, -1);
        }
    }
    
    refreshAllSegmentItemBackgroundPosition();
}

CAView* CASegmentedControl::getTailorImageAtIndex(int index, CAImage* image)
{
    CAImageView* imageView = NULL;

    CAScale9ImageView* clipNode = CAScale9ImageView::createWithFrame(m_pBackgroundView->getBounds());
    clipNode->setImage(image);

    float x = 0;
    
    for(int i=0; i< index; ++i)
    {
        x += m_vSegments.at(i)->getBounds().size.width;
    }
    
    CAView* segment = m_vSegments.at(index);
    if(segment)
    {
        DSize size = clipNode->getFrame().size;
        CARenderImage* render = CARenderImage::create(size.width, size.height);
        render->printscreenWithView(clipNode);
        
        DRect rect;
        rect.origin = DPoint(x, 0);
        rect.size = segment->getBounds().size;

        imageView = CAImageView::createWithImage(render->getImageView()->getImage());
        imageView->setFrame(rect);
        imageView->setImageRect(rect);
    }
    return imageView;
}

void CASegmentedControl::setSegmentItemBackgroundVisibleWithIndex(bool isVisible, int index)
{
    CAVector<CAView*>::iterator itr = m_vSegmentItemBackground.begin()+index;
    if(*itr!=NULL && index>-1 && index< m_vSegments.size())
        (*itr)->setVisible(isVisible);
}

void CASegmentedControl::setImageSizeAtIndex(DSize size, int index)
{
    if(index >= 0 && index < m_vSegments.size())
    {
        m_vImageSize.at(index) = size;
        refreshSegmentItemByIndex(index, CAControlStateNormal);
        refreshSegmentItemByIndex(index, CAControlStateSelected);
    }
}

void CASegmentedControl::setTitleFontName(std::string titleName)
{
    m_sTitleFontName = titleName;
    CAVector<CALabel*>::iterator itr = m_vTitles.begin();
    for(; itr != m_vTitles.end(); itr++)
    {
        (*itr)->setFontName(titleName);
    }
}

void CASegmentedControl::setTitleFontSize(float titleSize)
{
    m_fTitleFontSize = titleSize;
    CAVector<CALabel*>::iterator itr = m_vTitles.begin();
    for(; itr != m_vTitles.end(); itr++)
    {
        if(*itr!=NULL)
            (*itr)->setFontSize(titleSize);
    }
}

#pragma mark --

bool CASegmentedControl::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    int index = getSegmentItemIndexByPoint(point);
    if(index == m_iSelectedIndex)
    {
        return false;
    }
    m_iTouchIndex = index;
    if(isEnabledForSegmentAtIndex(m_iTouchIndex))
    {
        refreshSegmentItemByPoint(point, CAControlStateHighlighted);
        setSegmentItemBackgroundVisibleWithIndex(true, m_iTouchIndex);
        return true;
    }
    return false;
}

void CASegmentedControl::ccTouchMoved(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    if (getBounds().containsPoint(point))
    {
        int index =getSegmentItemIndexByPoint(point);
        if(m_iTouchIndex != index && m_iTouchIndex != -1)
        {
            refreshSegmentItemByIndex(m_iTouchIndex, CAControlStateNormal);
            setSegmentItemBackgroundVisibleWithIndex(false, m_iTouchIndex);
        }
        if(index != m_iSelectedIndex)
        {
            m_iTouchIndex = index;
            refreshSegmentItemByIndex(index, CAControlStateHighlighted);
            setSegmentItemBackgroundVisibleWithIndex(true, index);
        }
    }
    else
    {
        if(m_iTouchIndex == -1)
            return;
        refreshSegmentItemByIndex(m_iTouchIndex, CAControlStateNormal);
        setSegmentItemBackgroundVisibleWithIndex(false, m_iTouchIndex);
        m_iTouchIndex = -1;
    }
}

void CASegmentedControl::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    int index =getSegmentItemIndexByPoint(point);
    if(index>-1 && index<m_vSegments.size())
    {
        if( -1 != m_iSelectedIndex )
        {
            refreshSegmentItemByIndex(m_iSelectedIndex, CAControlStateNormal);
            setSegmentItemBackgroundVisibleWithIndex(false, m_iSelectedIndex);
        }
        refreshSegmentItemByIndex(index, CAControlStateSelected);
        setSegmentItemBackgroundVisibleWithIndex(true, index);
        m_iSelectedIndex = index;
        callFunc(this, index);
    }
}

void CASegmentedControl::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    refreshSegmentItemByIndex(m_iTouchIndex, CAControlStateNormal);
    setSegmentItemBackgroundVisibleWithIndex(false, m_iTouchIndex);
}

NS_CC_END
