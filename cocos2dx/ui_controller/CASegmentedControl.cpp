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
                                               std::vector<CAImage *> items,
                                               int selectedIndex)
{
    CASegmentedControl* segmentedControl = new CASegmentedControl();
    if (segmentedControl && segmentedControl->init(rect, items, selectedIndex))
    {
        segmentedControl->autorelease();
        return segmentedControl;
    }
    
    CC_SAFE_DELETE(segmentedControl);
    return NULL;
}

bool CASegmentedControl::initWithFrame(const CCRect& rect,
                                       std::vector<CAImage *> items,
                                       int selectedIndex)
{
    if (!CAControl::init())
    {
        return false;
    }
    
    this->setFrame(rect);
    m_selectedIndex = selectedIndex;
    return true;
}

void CASegmentedControl::insertSegmentWithTitle(const char* title, int index)
{
    
}

void CASegmentedControl::insertSegmentWithImage(CAImage *image, int index)
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

NS_CC_END
