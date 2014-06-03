//
//  CASegmentedControl.cpp
//  cocos2dx
//
//  Created by juguanhui on 14-6-3.
//  Copyright (c) 2014年 cocos2d-x. All rights reserved.
//

#include "CASegmentedControl.h"
#include "CAButton.h"

using namespace std;
NS_CC_BEGIN

CASegmentedControl::CASegmentedControl()
    : CAControl()
    , m_selectedIndex(0)
{
    
}

CASegmentedControl::~CASegmentedControl()
{
    this->removeAllSegments();
}

void CASegmentedControl::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();
}

void CASegmentedControl::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();
}

CASegmentedControl* CASegmentedControl::create(const CCRect& rect,
                                               std::vector<CAImage *> normalImages,
                                               std::vector<CAImage *> selectedImages,
                                               int selectedIndex)
{
    CASegmentedControl* segmentedControl = new CASegmentedControl();
    if (segmentedControl && segmentedControl->init(rect, normalImages, selectedImages, selectedIndex))
    {
        segmentedControl->autorelease();
        return segmentedControl;
    }
    
    CC_SAFE_DELETE(segmentedControl);
    return NULL;
}

bool CASegmentedControl::init(const CCRect& rect,
                              std::vector<CAImage *> normalImages,
                              std::vector<CAImage *> selectedImages,
                              int selectedIndex)
{
    if (!CAControl::init())
    {
        return false;
    }
    
    if (normalImages.size() != selectedImages.size())
    {
        return false;
    }
    
    this->setFrame(rect);
    m_selectedIndex = selectedIndex;
    
    this->removeAllSegments();
    int totalCount = normalImages.size();
    const float elemWidth = rect.size.width / totalCount;
    CCRect elemFrame = CCRectMake(0, 0, elemWidth, rect.size.height);
    for (int i = 0; i < totalCount; ++i)
    {
        CAButton *btn = CAButton::createWithFrame(elemFrame, CAButtonTypeCustom);
        if (btn)
        {
            char tmp[8] = {0};
            snprintf(tmp, 8, "%d", i);
            btn->setImageForState(CAControlStateNormal, normalImages.at(i));
            btn->setImageForState(CAControlStateSelected, selectedImages.at(i));
            btn->setTitleForState(CAControlStateNormal, tmp);
            btn->setTitleForState(CAControlStateSelected, tmp);
            m_segments.push_back(btn);
            this->addSubview(btn);
            if (i == m_selectedIndex)
            {
                btn->setControlStateSelected();
            }
            else
            {
                btn->setControlStateNormal();
            }
        }
        elemFrame.origin.x += elemWidth;
    }
    return true;
}

void CASegmentedControl::insertSegmentWithTitle(const char* title, int index, CAControlState controlState)
{
    
}

void CASegmentedControl::insertSegmentWithImage(CAImage *image, int index, CAControlState controlState)
{
    
}

void CASegmentedControl::removeSegmentAtIndex(int index)
{
    if (index >= 0 && index < m_segments.size())
    {
        vector<CAButton *>::iterator vi = m_segments.begin() + index;
        CC_SAFE_RELEASE(*vi);
        m_segments.erase(vi);
    }
}

void CASegmentedControl::removeAllSegments()
{
    for (std::vector<CAButton *>::iterator vi = m_segments.begin(); vi != m_segments.end(); ++vi)
    {
        CC_SAFE_RELEASE(*vi);
    }
    m_segments.clear();
}

void CASegmentedControl::setTitleAtIndex(const char* title, int index, CAControlState controlState)
{
    
}

void CASegmentedControl::setImageAtIndex(CAImage *image, int index, CAControlState controlState)
{
    
}

NS_CC_END
