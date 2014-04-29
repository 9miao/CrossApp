//
//  CATableView.h
//  cocos2dx
//
//  Created by liyuanfeng on 14-4-28.
//  Copyright (c) 2014 www.9miao.com All rights reserved.
//

#ifndef __cocos2dx__CATableView__
#define __cocos2dx__CATableView__

#include <iostream>
#include "CAScrollView.h"
#include <deque>

NS_CC_BEGIN

typedef enum
{
    CATableViewOrientationHorizontal,
    CATableViewOrientationVertical
} CATableViewOrientation;

class CATableViewCell;
class CATableView;

class CATableViewDataSource
{
public:
    
    //初始化cell回调
    virtual CATableViewCell* tableCellAtIndex(CATableView *table, unsigned int idx) = 0;
    
    //获取cell总个数回调
    virtual unsigned int numberfRowsInTableView(CATableView *table) = 0;
    
    void removeDelegate(CATableView* table){};
    
};


class CC_DLL CATableView: public CAScrollView
{
    
public:
    
    CATableView();
    
    virtual ~CATableView();
    
    virtual bool initWithFrame(const cocos2d::CCRect &rect, CATableViewOrientation tableViewOrientation = CATableViewOrientationVertical);
    
    inline virtual float maxSpeed();
    
    inline virtual float maxSpeedCache();
    
    inline virtual float decelerationRatio();
    
protected:
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    
    CATableViewOrientation m_eTableViewOrientation;
    
    std::deque<CATableViewCell*> m_pTableViewCells;
    
};

class CC_DLL CATableViewCell: public CCNode
{
    
public:
    
    CATableViewCell();
    
    virtual ~CATableViewCell();
    
    CREATE_FUNC(CATableViewCell);
    
    
    
protected:
    
    CCNodeRGBA* m_pBackGroundView;
    
};

NS_CC_END;

#endif /* defined(__cocos2dx__CATableView__) */
