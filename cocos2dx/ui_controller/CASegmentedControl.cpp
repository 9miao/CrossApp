//
//  CASegmentedControl.cpp
//  cocos2dx
//
//  Created by juguanhui on 14-6-3.
//  Copyright (c) 2014年 cocos2d-x. All rights reserved.
//

#include "CASegmentedControl.h"
#include "CAButton.h"
#include "sprite_nodes/CAScale9ImageView.h"
#include "touch_dispatcher/CCTouch.h"

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

CASegmentedControl* CASegmentedControl::createWithFrame(const CCRect& rect)
{
    CASegmentedControl* segmentedControl = new CASegmentedControl();
    if (segmentedControl && segmentedControl->initWithFrame(rect))
    {
        segmentedControl->autorelease();
        return segmentedControl;
    }
    
    CC_SAFE_DELETE(segmentedControl);
    return NULL;
}

bool CASegmentedControl::initWithFrame(const CCRect& rect)
{
    if (!CAControl::init())
    {
        return false;
    }
    
    this->setFrame(rect);
    m_selectedIndex = 0;
    
    this->removeAllSegments();
    int totalCount = 2;
    const float elemWidth = rect.size.width / totalCount;
    m_itemSize = CCSizeMake(elemWidth, rect.size.height);
    CCRect elemFrame = CCRectMake(0, 0, m_itemSize.width, m_itemSize.height);
    for (int i = 0; i < totalCount; ++i)
    {
        CAButton *btn = this->createDefaultSegment();
        if (btn)
        {
            btn->setFrame(elemFrame);
            char tmp[8] = {0};
            snprintf(tmp, 8, "%d", i);
            btn->setTitleForState(CAControlStateNormal, tmp);
            btn->setTitleForState(CAControlStateSelected, tmp);
            btn->setTitleForState(CAControlStateHighlighted, tmp);
            m_segments.push_back(btn);
            this->addSubview(btn);
            btn->setControlState((m_selectedIndex == i) ? CAControlStateSelected : CAControlStateNormal);
        }
        elemFrame.origin.x += elemWidth;
    }
    return true;
}

bool CASegmentedControl::insertSegmentWithTitle(const char* title, int index, CAControlState controlState)
{
    const int curItemCount = m_segments.size();
    if (index < 0)
    {
        index = 0;
    }
    else if (index >= curItemCount)
    {
        index = curItemCount;
    }
    
    CAButton *newBtn = this->createDefaultSegment();
    if (NULL == newBtn)
    {
        return false;
    }
    
    newBtn->setTitleForState(controlState, title);
    m_segments.insert(m_segments.begin() + index, newBtn);
    this->addSubview(newBtn);
    this->layoutSubviews();
    return true;
}

bool CASegmentedControl::insertSegmentWithBackgroundImage(CAImage *image, int index, CAControlState controlState)
{
    const int curItemCount = m_segments.size();
    if (index < 0)
    {
        index = 0;
    }
    else if (index >= curItemCount)
    {
        index = curItemCount;
    }
    
    CAButton *newBtn = this->createDefaultSegment();
    if (NULL == newBtn)
    {
        return false;
    }
    
    newBtn->setBackGroundViewForState(controlState, CAScale9ImageView::createWithImage(image));
    m_segments.insert(m_segments.begin() + index, newBtn);
    this->addSubview(newBtn);
    this->layoutSubviews();
    return true;
}

bool CASegmentedControl::insertSegmentWithImage(CAImage *image, int index, CAControlState controlState)
{
    const int curItemCount = m_segments.size();
    if (index < 0)
    {
        index = 0;
    }
    else if (index >= curItemCount)
    {
        index = curItemCount;
    }
    
    CAButton *newBtn = this->createDefaultSegment();
    if (NULL == newBtn)
    {
        return false;
    }
    
    newBtn->setImageForState(controlState, image);
    m_segments.insert(m_segments.begin() + index, newBtn);
    this->addSubview(newBtn);
    this->layoutSubviews();
    return true;
}

void CASegmentedControl::removeSegmentAtIndex(int index)
{
    if (!this->indexIsValid(index))
    {
        return ;
    }
    
    vector<CAButton *>::iterator vi = m_segments.begin() + index;
    (*vi)->removeFromSuperview();
    CC_SAFE_RELEASE(*vi);
    m_segments.erase(vi);
}

void CASegmentedControl::removeAllSegments()
{
    for (std::vector<CAButton *>::iterator vi = m_segments.begin(); vi != m_segments.end(); ++vi)
    {
        (*vi)->removeFromSuperview();
        CC_SAFE_RELEASE(*vi);
    }
    m_segments.clear();
}

bool CASegmentedControl::setTitleAtIndex(const char* title, int index, CAControlState controlState)
{
    if (!this->indexIsValid(index))
    {
        return false;
    }
    
    CAButton *btn = m_segments.at(index);
    if (NULL == btn)
    {
        return false;
    }
    btn->setTitleForState(controlState, title);
    return true;
}

bool CASegmentedControl::setBackgroundImageAtIndex(CAImage *image, int index, CAControlState controlState)
{
    if (!this->indexIsValid(index))
    {
        return false;
    }
    
    CAButton *btn = m_segments.at(index);
    if (NULL == btn)
    {
        return false;
    }
    btn->setBackGroundViewForState(controlState, CAScale9ImageView::createWithImage(image));
    return true;
}

bool CASegmentedControl::setImageAtIndex(CAImage *image, int index, CAControlState controlState)
{
    if (!this->indexIsValid(index))
    {
        return false;
    }
    
    CAButton *btn = m_segments.at(index);
    if (NULL == btn)
    {
        return false;
    }
    btn->setImageForState(controlState, image);
    return true;
}

CAView* CASegmentedControl::getDefaultNormalBackgroundView()
{
    return CAScale9ImageView::createWithImage(CAImage::create("btn_square_normal.png"));
}

CAView* CASegmentedControl::getDefaultHighlightedBackgroundView()
{
    return CAScale9ImageView::createWithImage(CAImage::create("btn_square_highlighted.png"));
}

CAView* CASegmentedControl::getDefaultSelectedBackgroundView()
{
    return CAScale9ImageView::createWithImage(CAImage::create("btn_square_selected.png"));
}

CAButton* CASegmentedControl::createDefaultSegment()
{
    CCRect elemFrame = CCRectMake(0, 0, m_itemSize.width, m_itemSize.height);
    
    CAButton *newBtn = CAButton::createWithFrame(elemFrame, CAButtonTypeCustom);
    if (newBtn)
    {
        CAView* normalBG = this->getDefaultNormalBackgroundView();
        CAView* selectedBG = this->getDefaultSelectedBackgroundView();
        CAView* highlightedBG = this->getDefaultHighlightedBackgroundView();
        newBtn->setBackGroundViewForState(CAControlStateNormal, normalBG);
        newBtn->setBackGroundViewForState(CAControlStateSelected, selectedBG);
        newBtn->setBackGroundViewForState(CAControlStateHighlighted, highlightedBG);
        newBtn->setAllowsSelected(true);
    }
    return newBtn;
}

void CASegmentedControl::setSelectedAtIndex(int index)
{
    if (!this->indexIsValid(index))
    {
        return ;
    }
    
    CAButton *oldSelectedButton = m_segments.at(m_selectedIndex);
    if (oldSelectedButton)
    {
        oldSelectedButton->setControlStateNormal();
    }
    
    CAButton *newSelectedButton = m_segments.at(index);
    if (newSelectedButton)
    {
        newSelectedButton->setControlStateSelected();
    }
    
    m_selectedIndex = index;
}

int CASegmentedControl::getItemCount() const
{
    return m_segments.size();
}

void CASegmentedControl::layoutSubviews()
{
    const CCSize controlSize = this->getBounds().size;
    const int totalCount = this->getItemCount();
    const float elemWidth = controlSize.width / totalCount;
    m_itemSize = CCSizeMake(elemWidth, controlSize.height);
    CCRect elemFrame = CCRectMake(0, 0, m_itemSize.width, m_itemSize.height);
    
    for (int i = 0; i < totalCount; ++i)
    {
        CAButton *btn = m_segments.at(i);
        if (btn)
        {
            btn->setFrame(elemFrame);
            btn->setControlState((m_selectedIndex == i) ? CAControlStateSelected : CAControlStateNormal);
        }
        elemFrame.origin.x += elemWidth;
    }
}

bool CASegmentedControl::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    if (!this->isVisible())
    {
        return false;
    }
    
    if (!this->getBounds().containsPoint(point))
    {
        return false;
    }
    
    return true;
}

void CASegmentedControl::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    this->setSelectedAtIndex((int)(point.x / m_itemSize.width));
}

void CASegmentedControl::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    this->ccTouchEnded(pTouch, pEvent);
}

bool CASegmentedControl::indexIsValid(int index)
{
    return index >= 0 && index < m_segments.size();
}

NS_CC_END
