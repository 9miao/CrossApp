//
//  CATableView.cpp
//  cocos2dx
//
//  Created by 栗元峰 on 14-4-28.
//  Copyright (c) 2014年 厦门雅基软件有限公司. All rights reserved.
//

#include "CATableView.h"

NS_CC_BEGIN

#pragma CATableView

CATableView::CATableView()
:m_eTableViewOrientation(CATableViewOrientationVertical)
{

}

CATableView::~CATableView()
{

}

bool CATableView::initWithFrame(const cocos2d::CCRect &rect, CATableViewOrientation tableViewOrientation)
{
    m_eTableViewOrientation = tableViewOrientation;
    return CAScrollView::initWithFrame(rect);
}

bool CATableView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return CAScrollView::ccTouchBegan(pTouch, pEvent);
}

void CATableView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CAScrollView::ccTouchMoved(pTouch, pEvent);
}

void CATableView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CAScrollView::ccTouchEnded(pTouch, pEvent);
}

void CATableView::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    CAScrollView::ccTouchCancelled(pTouch, pEvent);
}

float CATableView::maxSpeed()
{
    return (CCPoint(m_obContainerSize).getLength() / 32);
}

float CATableView::maxSpeedCache()
{
    return (maxSpeed() * 1.5f);
}

float CATableView::decelerationRatio()
{
    return 0.1f;
}

#pragma CATableViewCell

CATableViewCell::CATableViewCell()
{

}

CATableViewCell::~CATableViewCell()
{

}



NS_CC_END