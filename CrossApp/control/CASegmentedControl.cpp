//
//  CASegmentedControl.cpp
//  CrossAppx
//
//  Created by juguanhui on 14-6-3.
//  Copyright (c) 2014年 CrossApp-x. All rights reserved.
//

#include "CASegmentedControl.h"
#include "CAButton.h"
#include "view/CAScale9ImageView.h"
#include "dispatcher/CATouch.h"
#include "basics/CAApplication.h"

using namespace std;
NS_CC_BEGIN

CASegmentedControl::CASegmentedControl(unsigned int itemsCount)
    : CAControl()
    , m_selectedIndex(-1)
    , m_nItemsCount(itemsCount)
    , m_pBackgroundView(NULL)
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

CASegmentedControl* CASegmentedControl::createWithFrame(const CCRect& rect, unsigned int itemsCount)
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

CASegmentedControl* CASegmentedControl::createWithCenter(const CCRect& rect, unsigned int itemsCount)
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

bool CASegmentedControl::initWithFrame(const CCRect& rect)
{
    if (!CAControl::init())
    {
        return false;
    }
    this->setColor(CAColor_clear);
    this->setFrame(rect);
    
    this->removeAllSegments();
    const float elemWidth = rect.size.width / m_nItemsCount;
    m_itemSize = CCSizeMake(elemWidth, rect.size.height);
    CCRect elemFrame = CCRectMake(0, 0, m_itemSize.width, m_itemSize.height);
    for (int i = 0; i < m_nItemsCount; ++i)
    {
        CAButton *btn = this->createDefaultSegment();
        if (btn)
        {
            btn->setFrame(elemFrame);
            m_segments.push_back(btn);
            this->insertSubview(btn, 1);
        }
        elemFrame.origin.x += elemWidth;
    }
    return true;
}

bool CASegmentedControl::initWithCenter(const CCRect& rect)
{
    if (!CAControl::init())
    {
        return false;
    }
    
    this->setCenter(rect);
    
    this->removeAllSegments();
    const float elemWidth = rect.size.width / m_nItemsCount;
    m_itemSize = CCSize(elemWidth, rect.size.height);
    CCRect elemFrame = CCRect(0, 0, m_itemSize.width, m_itemSize.height);
    for (int i = 0; i < m_nItemsCount; ++i)
    {
        CAButton *btn = this->createDefaultSegment();
        if (btn)
        {
            btn->setFrame(elemFrame);
            m_segments.push_back(btn);
            this->insertSubview(btn, 1);
        }
        elemFrame.origin.x += elemWidth;
    }
    return true;
}

bool CASegmentedControl::insertSegmentWithTitle(const char* title, int index, const CAControlState& controlState)
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

bool CASegmentedControl::insertSegmentWithBackgroundImage(CAImage *image, int index, const CAControlState& controlState)
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

bool CASegmentedControl::insertSegmentWithImage(CAImage *image, int index, const CAControlState& controlState)
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
    }
    m_segments.clear();
}

bool CASegmentedControl::setTitleAtIndex(const char* title, int index, const CAControlState& controlState)
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

bool CASegmentedControl::setTitleColorAtIndex(const CAColor4B& color, int index, const CAControlState& controlState)
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
    btn->setTitleColorForState(controlState, color);
    
    return true;
}

bool CASegmentedControl::setBackgroundImageAtIndex(CAImage *image, int index, const CAControlState& controlState)
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

bool CASegmentedControl::setBackgroundViewAtIndex(CAView *view, int index, const CAControlState& controlState)
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
    btn->setBackGroundViewForState(controlState, view);
    
    return true;
}

bool CASegmentedControl::setImageAtIndex(CAImage *image, int index, const CAControlState& controlState)
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

bool CASegmentedControl::setImageColorAtIndex(const CAColor4B& color, int index, const CAControlState& controlState)
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
    btn->setImageColorForState(controlState, color);
    
    return true;
}

CAView* CASegmentedControl::getDefaultNormalBackgroundView()
{
    return CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_square_normal.png"));
}

CAView* CASegmentedControl::getDefaultHighlightedBackgroundView()
{
    return CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_square_highlighted.png"));
}

CAView* CASegmentedControl::getDefaultSelectedBackgroundView()
{
    return CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_square_highlighted.png"));
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
        newBtn->setTouchEnabled(false);
    }
    return newBtn;
}

void CASegmentedControl::setHighlightedAtIndex(int index)
{
    if (!this->indexIsValid(index))
    {
        return ;
    }
    
    this->setHighlightedNormal();
    if (index != m_selectedIndex)
    {
        m_segments.at(index)->setControlStateHighlighted();
    }
}

void CASegmentedControl::setHighlightedNormal()
{
    for (int i=0; i<m_segments.size(); i++)
    {
        if (m_segments.at(i)->getControlState() != CAControlStateSelected)
        {
            m_segments.at(i)->setControlStateNormal();
        }
    }
}

void CASegmentedControl::setSelectedHighlighted()
{
    int index = m_selectedIndex;

    for (int i=0; i<m_segments.size(); i++)
    {
        if (m_segments.at(i)->getControlState() == CAControlStateHighlighted)
        {
            index = i;
        }
        else if (m_segments.at(i)->getControlState() == CAControlStateSelected)
        {
            m_segments.at(i)->setControlStateNormal();
        }
    }
    
    m_segments.at(index)->setControlStateSelected();
    
    if (m_selectedIndex != index)
    {
        m_selectedIndex = index;
        
        if (m_pTarget[CAControlEventTouchValueChanged] && m_selTouch[CAControlEventTouchValueChanged])
        {
            ((CAObject *)m_pTarget[CAControlEventTouchValueChanged]->*m_selTouch[CAControlEventTouchValueChanged])(this, CCPointZero);
        }
    }
}

void CASegmentedControl::setSelectedAtIndex(int index)
{
    this->setHighlightedAtIndex(index);
    this->setSelectedHighlighted();
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
    m_itemSize = CCSize(elemWidth, controlSize.height);
    CCRect elemFrame = CCRect(0, 0, m_itemSize.width, m_itemSize.height);
    
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

bool CASegmentedControl::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    this->setHighlightedAtIndex((int)(point.x / m_itemSize.width));
    
    return true;
}

void CASegmentedControl::ccTouchMoved(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    if (getBounds().containsPoint(point))
    {
        this->setHighlightedAtIndex((int)(point.x / m_itemSize.width));
    }
    else
    {
        this->setHighlightedNormal();
    }
}

void CASegmentedControl::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    this->setSelectedHighlighted();
}

void CASegmentedControl::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    this->setHighlightedNormal();
}

bool CASegmentedControl::indexIsValid(int index)
{
    return index >= 0 && index < m_segments.size();
}

void CASegmentedControl::setContentSize(const CrossApp::CCSize &var)
{
    CCSize size = var;
    size.height = MAX(size.height, _px(60));
    size.width = MAX(size.width, size.height * 2);
    CAControl::setContentSize(size);
    this->layoutSubviews();
    if (m_pBackgroundView)
    {
        m_pBackgroundView->setFrame(this->getBounds());
    }
}

void CASegmentedControl::addTarget(CAObject* target, SEL_CAControl selector)
{
    this->addTarget(target, selector, CAControlEventTouchValueChanged);
}

void CASegmentedControl::removeTarget(CAObject* target, SEL_CAControl selector)
{
    this->removeTarget(target, selector, CAControlEventTouchValueChanged);
}

void CASegmentedControl::setBackgroundView(CrossApp::CAView *view)
{
    CC_SAFE_RETAIN(view);
    CC_SAFE_RELEASE_NULL(m_pBackgroundView);
    m_pBackgroundView = view;
    m_pBackgroundView->setFrame(this->getBounds());
    this->insertSubview(m_pBackgroundView, -1);
}

CAView* CASegmentedControl::getBackgroundView()
{
    return m_pBackgroundView;
}

NS_CC_END
