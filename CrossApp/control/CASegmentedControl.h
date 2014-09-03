//
//  CASegmentedControl.h
//  CrossAppx
//
//  Created by juguanhui on 14-6-3.
//  Copyright (c) 2014年 CrossApp-x. All rights reserved.
//

#ifndef CrossAppx_CASegmentedControl_h
#define CrossAppx_CASegmentedControl_h

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
    
    void addTarget(CAObject* target, SEL_CAControl selector);
    
    void removeTarget(CAObject* target, SEL_CAControl selector);
    
    CC_PROPERTY(CAView*, m_pBackgroundView, BackgroundView);
    
public:
    
    virtual bool initWithFrame(const CCRect& rect);
    virtual bool initWithCenter(const CCRect& rect);
    
    virtual bool insertSegmentWithTitle(const char* title, int index, const CAControlState& controlState);
    virtual bool insertSegmentWithBackgroundImage(CAImage *image, int index, const CAControlState& controlState);
    virtual bool insertSegmentWithImage(CAImage *image, int index, const CAControlState& controlState);
    virtual void removeSegmentAtIndex(int index);
    virtual void removeAllSegments();
    
    virtual bool setTitleAtIndex(const char* title, int index, const CAControlState& controlState);
    virtual bool setTitleColorAtIndex(const CAColor4B& color, int index, const CAControlState& controlState);
    virtual bool setBackgroundImageAtIndex(CAImage *image, int index, const CAControlState& controlState);
    virtual bool setBackgroundViewAtIndex(CAView *view, int index, const CAControlState& controlState);
    virtual bool setImageAtIndex(CAImage *image, int index, const CAControlState& controlState);
    virtual bool setImageColorAtIndex(const CAColor4B& color, int index, const CAControlState& controlState);
    
    virtual CAView *getDefaultNormalBackgroundView();
    virtual CAView *getDefaultHighlightedBackgroundView();
    virtual CAView *getDefaultSelectedBackgroundView();
    
    virtual CAButton *createDefaultSegment();
    
    CC_PROPERTY_PASS_BY_REF(std::string, m_sTitleFontName, TitleFontName);
    CC_SYNTHESIZE_READONLY(int, m_selectedIndex, selectedIndex);       // default 0
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCSize, m_itemSize, ItemSize);
    void setHighlightedAtIndex(int index);
    void setSelectedAtIndex(int index);
    void setHighlightedNormal();
    void setSelectedHighlighted();
    int getItemCount() const;
    void layoutSubviews();
    
protected:
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
    bool indexIsValid(int index);
    void setContentSize(const CCSize & var);
    using CAControl::addTarget;
    using CAControl::removeTarget;
    
protected:
    
    unsigned int m_nItemsCount;
    std::vector<CAButton *> m_segments;
    CAImageView *m_backGroundImage;
};

NS_CC_END


#endif
