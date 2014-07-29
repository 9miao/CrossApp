//
//  CAScrollView.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-23.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __CrossAppx__CAScrollView__
#define __CrossAppx__CAScrollView__

#include <iostream>
#include "CAView.h"
#include <deque>

NS_CC_BEGIN

class CAScrollView;
class CAScrollViewDelegate
{
public:

    virtual ~CAScrollViewDelegate(){};
    
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
    
    static CAScrollView* createWithFrame(const CCRect& rect);
    
    static CAScrollView* createWithCenter(const CCRect& rect);
    
    virtual bool init();

    void addSubview(CAView* subview);
    
    virtual void insertSubview(CAView* subview, int z);
    
    void removeAllSubviews();
    
    void removeSubview(CAView* subview);

    void removeSubviewByTag(int tag);
    
    CAView* getSubviewByTag(int aTag);
    
    CCArray* getSubviews();
    
    void setContentOffset(CCPoint offset, bool animated);
    
    CCPoint getContentOffset();
    
    void setBackGroundImage(CAImage* image);

    void setBackGroundColor(const CAColor4B &color);
    
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
 
    const CCPoint& getScrollWindowNotOutPoint(CCPoint& point);
    
    virtual float getScrollWindowNotOutHorizontal(float x);
    
    virtual float getScrollWindowNotOutVertical(float y);
    
    bool isScrollWindowNotOutSide();
    
    bool isScrollWindowNotMaxOutSide(const CCPoint& point);
    
    virtual void update(float dt);
    
    void deaccelerateScrolling(float dt);
    
    inline virtual float maxSpeed(float dt);
    
    inline virtual float maxSpeedCache(float dt);
    
    inline virtual float decelerationRatio(float dt);
    
    inline virtual CCPoint maxBouncesLenght();
    
    inline float maxBouncesSpeed(float dt);
    
    virtual void contentOffsetFinish(){}
    
    void closeToPoint(float dt);
    
    void showIndicator();
    
    void hideIndicator();

    virtual void setContentSize(const CCSize& var);
    
protected:
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
private:
    
    using CAView::initWithColor;
    
    using CAView::setImage;
    
    using CAView::setImageRect;
    
    using CAView::setColor;
    
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
    
    CAIndicator(CAIndicatorType type);
    
    virtual ~CAIndicator();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    static CAIndicator* create(CAIndicatorType type);
    
    virtual bool init();
    
    void setIndicator(const CCSize& parentSize, const CCRect& childrenFrame);
    
    void setHide(bool var);

private:

    CAView* m_pIndicator;
    
    CAIndicatorType m_eType;
};

NS_CC_END
#endif /* defined(__CrossAppx__CAScrollView__) */
