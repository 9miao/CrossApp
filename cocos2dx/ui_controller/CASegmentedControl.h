//
//  CASegmentedControl.h
//  cocos2dx
//
//  Created by juguanhui on 14-6-3.
//  Copyright (c) 2014年 cocos2d-x. All rights reserved.
//

#ifndef cocos2dx_CASegmentedControl_h
#define cocos2dx_CASegmentedControl_h

#include "CAControl.h"
#include <vector>

NS_CC_BEGIN

class CAButton;
class CAImage;
class CAImageView;

class CC_DLL CASegmentedControl : public CAControl
{
public:
    
    CASegmentedControl(unsigned int itemsCount);
    
    virtual ~CASegmentedControl();
    
    virtual void onExitTransitionDidStart();
    
    virtual void onEnterTransitionDidFinish();
    
    static CASegmentedControl* createWithFrame(const CCRect& rect, unsigned int itemsCount);
    
    static CASegmentedControl* createWithCenter(const CCRect& rect, unsigned int itemsCount);
    
    void addTarget(CCObject* target, SEL_CAControl selector);
    
    void removeTarget(CCObject* target, SEL_CAControl selector);
    
public:
    
    bool initWithFrame(const CCRect& rect);

    bool initWithCenter(const CCRect& rect);
    
    virtual bool insertSegmentWithTitle(const char* title, int index, CAControlState controlState);
    virtual bool insertSegmentWithBackgroundImage(CAImage *image, int index, CAControlState controlState);
    virtual bool insertSegmentWithImage(CAImage *image, int index, CAControlState controlState);
    virtual void removeSegmentAtIndex(int index);
    virtual void removeAllSegments();
    
    virtual bool setTitleAtIndex(const char* title, int index, CAControlState controlState);
    virtual bool setTitleColorAtIndex(CAColor4B color, int index, CAControlState controlState);
    virtual bool setBackgroundImageAtIndex(CAImage *image, int index, CAControlState controlState);
    virtual bool setImageAtIndex(CAImage *image, int index, CAControlState controlState);
    
    virtual CAView *getDefaultNormalBackgroundView();
    virtual CAView *getDefaultHighlightedBackgroundView();
    virtual CAView *getDefaultSelectedBackgroundView();
    
    virtual CAButton *createDefaultSegment();
    
    CC_SYNTHESIZE_READONLY(int, m_selectedIndex, selectedIndex);       // default 0
    CC_SYNTHESIZE_READONLY(CCSize, m_itemSize, ItemSize);
    void setHighlightedAtIndex(int index);
    void setSelectedAtIndex(int index);
    void setHighlightedNormal();
    void setSelectedHighlighted();
    int getItemCount() const;
    void layoutSubviews();
    
protected:
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    bool indexIsValid(int index);
    virtual void addTarget(CCObject* target, SEL_CAControl selector, CAControlEvents event);
    virtual void removeTarget(CCObject* target, SEL_CAControl selector, CAControlEvents event);
    void setContentSize(const CCSize & var);
    
    
protected:
    
    unsigned int m_nItemsCount;
    std::vector<CAButton *> m_segments;
    CAImageView *m_backGroundImage;
};

NS_CC_END


#endif
