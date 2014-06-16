//
//  CAScrollView.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-23.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __cocos2dx__CAScrollView__
#define __cocos2dx__CAScrollView__

#include <iostream>
#include "sprite_nodes/CAView.h"
#include <deque>

NS_CC_BEGIN

class CAScrollView;
class CC_DLL CAScrollViewDelegate
{
public:

    virtual void scrollViewDidScroll(CAScrollView* view){};
    
    virtual void scrollViewWillBeginDragging(CAScrollView* view){};
    
    virtual void scrollViewDidEndDragging(CAScrollView* view){};
    
    virtual void scrollViewDidZoom(CAScrollView* view){};
};

class CAIndicator;
class CC_DLL CAScrollView : public CAView
{
    
public:

    CAScrollView();
    
    virtual ~CAScrollView();
 
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    virtual bool initWithFrame(const cocos2d::CCRect &rect);

    void addSubview(CAView* subview);
    
    virtual void insertSubview(CAView* subview, int z);
    
    void removeAllSubviews();
    
    void removeSubview(CAView* subview);

    void removeSubviewByTag(int tag);
    
    CAView* getSubviewByTag(int aTag);
    
    CCArray* getSubviews();
    
    void setContentOffset(CCPoint offset, bool animated);
    
    CCPoint getContentOffset();
    
protected:
 
    const CCPoint& getScrollWindowNotOutPoint(CCPoint& point);
    
    float getScrollWindowNotOutHorizontal(float x);
    
    float getScrollWindowNotOutVertical(float y);
    
    bool isScrollWindowNotOutSide();
    
    bool isScrollWindowNotMaxOutSide();
    
    virtual void update(float fDelta);
    
    void deaccelerateScrolling(float delay);
    
    inline virtual float maxSpeed(float delay);
    
    inline virtual float maxSpeedCache(float delay);
    
    inline virtual float decelerationRatio(float delay);
    
    inline virtual CCPoint maxBouncesLenght();
    
    inline float maxBouncesSpeed(float delay);
    
    void closeToPoint(float delay);
    
    void showIndicator();
    
    void hideIndicator();

    virtual void setContentSize(const CCSize& var);
    
protected:
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    
    CC_SYNTHESIZE(CAScrollViewDelegate*, m_pScrollViewDelegate, ScrollViewDelegate);
    
    CC_PROPERTY_IS(bool, m_bBounces, Bounces);
    
    CC_SYNTHESIZE_IS(bool, m_bBounceHorizontal, BounceHorizontal);
    
    CC_SYNTHESIZE_IS(bool, m_bBounceVertical, BounceVertical);
    
	CC_SYNTHESIZE_IS_READONLY(bool, m_bTracking, Tracking);
    
	CC_SYNTHESIZE_IS_READONLY(bool, m_bDecelerating, Decelerating);
    
    CC_PROPERTY_IS(bool, m_bShowsHorizontalScrollIndicator, ShowsHorizontalScrollIndicator);
    
    CC_PROPERTY_IS(bool, m_bShowsVerticalScrollIndicator, ShowsVerticalScrollIndicator);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bZooming, Zooming);
    
    CC_SYNTHESIZE(float, m_fMinimumZoomScale, MinimumZoomScale);
    
    CC_SYNTHESIZE(float, m_fMaximumZoomScale, MaximumZoomScale);
    
    CC_SYNTHESIZE_READONLY(float, m_fZoomScale, ZoomScale);
    
    CC_PROPERTY_IS(bool, m_bscrollEnabled, ScrollEnabled);
    
    CC_PROPERTY(CCSize, m_obViewSize, ViewSize);
    
protected:
    
    CAView* m_pContainer;
    
    CAIndicator* m_pIndicatorHorizontal;
    
    CAIndicator* m_pIndicatorVertical;
    
    CCArray* m_pChildInThis;
    
    CCArray* m_pTouches;
    
    float m_fTouchLength;

    CCPoint m_tInertia;
    
    std::deque<CCPoint> m_tPointOffset;
    
    CCPoint m_tCloseToPoint;
    
};


class CC_DLL CAIndicator : public CAView
{
public:
    
    typedef enum
    {
        CAIndicatorTypeHorizontal,
        CAIndicatorTypeVertical
    }CAIndicatorType;
    
    
public:
    
    CAIndicator();
    
    virtual ~CAIndicator();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    static CAIndicator* createWithFrame(const CCRect& rect, CAIndicatorType type);
    
    virtual bool initWithFrame(const CCRect& rect, CAIndicatorType type);
    
    void setIndicator(const CCSize& parentSize, const CCRect& childrenFrame);
    
    void setHide(bool var);
    
private:

    CAView* m_pIndicator;
    
    CAIndicatorType m_eType;
};

NS_CC_END
#endif /* defined(__cocos2dx__CAScrollView__) */
