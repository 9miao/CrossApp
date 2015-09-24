//
//  CAScrollView.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-23.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __CrossApp__CAScrollView__
#define __CrossApp__CAScrollView__

#include <iostream>
#include "CAView.h"
#include <deque>
#include "CAPullToRefreshView.h"
#include "basics/CAApplication.h"

NS_CC_BEGIN

class CAScrollView;
class CC_DLL CAScrollViewDelegate
{
public:

    virtual ~CAScrollViewDelegate(){};

    virtual void scrollViewDidMoved(CAScrollView* view){};
    
    virtual void scrollViewStopMoved(CAScrollView* view){};

    virtual void scrollViewWillBeginDragging(CAScrollView* view){};
    
    virtual void scrollViewDragging(CAScrollView* view){};
    
    virtual void scrollViewDidEndDragging(CAScrollView* view){};
    
    virtual void scrollViewDidZoom(CAScrollView* view){};
    
    virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view){};
    
    virtual void scrollViewFooterBeginRefreshing(CAScrollView* view){};
    
    virtual void scrollViewTouchUpWithoutMoved(CAScrollView* view, const CCPoint& point){};
    
public:
    
    virtual void scrollViewDidScroll(CAScrollView* view){};
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

    virtual void addSubview(CAView* subview);
    
    virtual void insertSubview(CAView* subview, int z);
    
    void removeAllSubviews();
    
    void removeSubview(CAView* subview);

    void removeSubviewByTag(int tag);
    
    CAView* getSubviewByTag(int aTag);
    
    const CAVector<CAView*>& getSubviews();
    
    void setContentOffset(const CCPoint& offset, bool animated);
    
    CCPoint getContentOffset();
    
    void setBackGroundImage(CAImage* image);

    void setBackGroundColor(const CAColor4B &color);
    
    void setZoomScale(float zoom);

    virtual bool isReachBoundaryLeft();
    
    virtual bool isReachBoundaryRight();
    
    virtual bool isReachBoundaryUp();
    
    virtual bool isReachBoundaryDown();
    
    CC_PROPERTY_IS(bool, m_bTouchEnabledAtSubviews, TouchEnabledAtSubviews);
    
    CC_PROPERTY_IS(bool, m_bShowsScrollIndicators, ShowsScrollIndicators);// default is true
    
    CC_PROPERTY_IS(bool, m_bShowsHorizontalScrollIndicator, ShowsHorizontalScrollIndicator);// default is true
    
    CC_PROPERTY_IS(bool, m_bShowsVerticalScrollIndicator, ShowsVerticalScrollIndicator);// default is true
    
    CC_SYNTHESIZE(CAScrollViewDelegate*, m_pScrollViewDelegate, ScrollViewDelegate);
    
    CC_PROPERTY_IS(bool, m_bBounces, Bounces);
    
    CC_SYNTHESIZE_IS(bool, m_bBounceHorizontal, BounceHorizontal);
    
    CC_SYNTHESIZE_IS(bool, m_bBounceVertical, BounceVertical);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bTracking, Tracking);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bDecelerating, Decelerating);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bZooming, Zooming);
    
    CC_SYNTHESIZE(float, m_fMinimumZoomScale, MinimumZoomScale);// default is 1.0
    
    CC_SYNTHESIZE(float, m_fMaximumZoomScale, MaximumZoomScale);// default is 1.0
    
    CC_SYNTHESIZE_READONLY(float, m_fZoomScale, ZoomScale);
    
    CC_PROPERTY_PASS_BY_REF(CCSize, m_obViewSize, ViewSize);
    
    CC_PROPERTY(CAPullToRefreshView*, m_pHeaderRefreshView, HeaderRefreshView);
    
    CC_PROPERTY(CAPullToRefreshView*, m_pFooterRefreshView, FooterRefreshView);
    
    void endHeaderRefresh();
    
    void endFooterRefresh();
    
    void stopDeaccelerateScroll();
    
    void startDeaccelerateScroll();
    
    virtual void switchPCMode(bool var);
    
protected:
 
    inline virtual float minSpeed(float dt)
    {
        return (60 * dt);
    }
    
    inline virtual float maxSpeed(float dt)
    {
        return (_px(90) * 60 * dt);
    }
    
    inline virtual float maxSpeedCache(float dt)
    {
        return (maxSpeed(dt) * 1.5f);
    }
    
    inline virtual float decelerationRatio(float dt)
    {
        return 6 * dt;
    }
    
    virtual float maxBouncesLenght()
    {
        return _px(120.0f);
    }
    
    virtual void contentOffsetFinish(float dt) {};
    
    void getScrollWindowNotOutPoint(CCPoint& point);
    
    virtual float getScrollWindowNotOutHorizontal(float x);
    
    virtual float getScrollWindowNotOutVertical(float y);
    
    bool isScrollWindowNotOutSide();
    
    bool isScrollWindowNotMaxOutSide(const CCPoint& point);
    
    virtual void update(float dt);
    
    void deaccelerateScrolling(float dt);
    
    void closeToPoint(float dt, float now, float total);
    
    void updateIndicator();
    
    void showIndicator();
    
    void hideIndicator();

    void layoutPullToRefreshView();
    
    void changedFromPullToRefreshView();
    
    void detectionFromPullToRefreshView();

    bool isHeaderRefreshing();
    
    bool isFooterRefreshing();
    
    void setContainerFrame(const CCPoint& point, const CCSize& size = CCSizeZero);
    
    virtual void setContentSize(const CCSize& var);
    
    void updatePointOffset(float dt = 0);
    
public:
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void mouseScrollWheel(CATouch* pTouch, float off_x, float off_y, CAEvent* pEvent);
    
private:
    
    using CAView::initWithColor;
    
    using CAView::setColor;
    
protected:
    
    CAView* m_pContainer;
    
    CAIndicator* m_pIndicatorHorizontal;
    
    CAIndicator* m_pIndicatorVertical;
    
    CAVector<CAView*> m_vChildInThis;
    
    CAVector<CATouch*> m_vTouches;
    
    float m_fTouchLength;

    CCPoint m_tInertia;
    
    std::deque<CCPoint> m_tPointOffset;
    
    CCPoint m_tCloseToPoint;
    
    CCPoint m_tInitialPoint;
    
    bool m_bPCMode;
};


class CC_DLL CAIndicator : public CAView
{
public:
    
    typedef enum
    {
        CAIndicatorTypeHorizontal,
        CAIndicatorTypeVertical
    }
    CAIndicatorType;
    
    
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
#endif /* defined(__CrossApp__CAScrollView__) */
