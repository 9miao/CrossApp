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
    
    virtual void scrollViewTouchUpWithoutMoved(CAScrollView* view, const DPoint& point){};
};

class CAIndicator;
class CC_DLL CAScrollView : public CAView
{
public:
    
    typedef enum
    {
        Zoom,
        Rotate,
        ZoomAndRotate,
        None
    }
    MultitouchGesture;
    
public:

    CAScrollView();
    
    virtual ~CAScrollView();
 
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    static CAScrollView* createWithFrame(const DRect& rect);
    
    static CAScrollView* createWithCenter(const DRect& rect);
    
    static CAScrollView* createWithLayout(const DRectLayout& layout);
    
    virtual bool init();

    virtual void addSubview(CAView* subview);
    
    virtual void insertSubview(CAView* subview, int z);
    
    void removeAllSubviews();
    
    void removeSubview(CAView* subview);

    void removeSubviewByTag(int tag);
    
    CAView* getSubviewByTag(int aTag);
    
    const CAVector<CAView*>& getSubviews();
    
    void setContentOffset(const DPoint& offset, bool animated);
    
    DPoint getContentOffset();
    
    void setBackgroundImage(CAImage* image);

    void setBackgroundColor(const CAColor4B &color);
    
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
    
    CC_PROPERTY_PASS_BY_REF(DSize, m_obViewSize, ViewSize);
    
    CC_PROPERTY(CAPullToRefreshView*, m_pHeaderRefreshView, HeaderRefreshView);
    
    CC_PROPERTY(CAPullToRefreshView*, m_pFooterRefreshView, FooterRefreshView);
    
    CC_SYNTHESIZE_PASS_BY_REF(MultitouchGesture, m_eMultitouchGesture, MultitouchGesture);// default is CAScrollView::MultitouchGesture::None
    
    void endHeaderRefresh();
    
    void endFooterRefresh();
    
    void stopDeaccelerateScroll();
    
    void startDeaccelerateScroll();
    
    void startPullToHeaderRefreshView();
    
    virtual void switchPCMode(bool var);
    
protected:
 
    inline virtual float minSpeed(float dt)
    {
        return (60 * dt);
    }
    
    inline virtual float maxSpeed(float dt)
    {
        return (90 * 60 * dt);
    }
    
    inline virtual float decelerationRatio(float dt)
    {
        return 6 * dt;
    }
    
    virtual float maxBouncesLenght()
    {
        return 120.0f;
    }
    
    virtual void contentOffsetFinish(float dt) {};
    
    void getScrollWindowNotOutPoint(DPoint& point);
    
    virtual float getScrollWindowNotOutHorizontal(float x);
    
    virtual float getScrollWindowNotOutVertical(float y);
    
    bool isScrollWindowNotOutSide();
    
    bool isScrollWindowNotMaxOutSide(const DPoint& point);
    
    virtual void update(float dt);
    
    void deaccelerateScrolling(float dt);
    
    void closeToPoint(float dt, float now, float total);
    
    void initIndicator();
    
    void showIndicator();
    
    void hideIndicator();

    void layoutPullToRefreshView();
    
    void changedFromPullToRefreshView();
    
    void detectionFromPullToRefreshView();

    bool isHeaderRefreshing();
    
    bool isFooterRefreshing();
    
    void setContainerFrame(const DPoint& point, const DSize& size = DSizeZero);
    
    virtual void setContentSize(const DSize& var);
    
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
    
    float m_fPreviewScale;
    
    float m_fBeganTouchLength;

    float m_fBeganZoomScale;
    
    float m_fBeganAngle;
    
    float m_fBeganGestureAngle;
    
    DPoint m_tInertia;
    
    std::deque<DPoint> m_tPointOffset;
    
    DPoint m_tCloseToPoint;
    
    DPoint m_tInitialPoint;
    
    bool m_bPCMode;
};


class CC_DLL CAIndicator : public CAView
{
public:
    
    typedef enum
    {
        CAIndicatorTypeHorizontal = 0,
        CAIndicatorTypeVertical
    }
    CAIndicatorType;
    
    
public:
    
    CAIndicator(const CAIndicatorType& type, CAScrollView* var);
    
    virtual ~CAIndicator();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    static CAIndicator* create(const CAIndicatorType& type, CAScrollView* var);
    
    virtual bool init();
    
    void setIndicator(const DSize& parentSize, const DRect& childrenFrame);
    
    void setHide(bool var);

    void switchPCMode(bool var);
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void mouseMoved(CATouch* pTouch, CAEvent* pEvent);
    
    virtual void mouseMovedOutSide(CATouch* pTouch, CAEvent* pEvent);
    
    CC_SYNTHESIZE_READONLY(CAScrollView*, m_pMyScrollView, MyScrollView)
    
private:

    CAView* m_pIndicator;
    
    CAIndicatorType m_eType;
    
    bool m_bPCMode;
    
    bool m_bTouch;
};


NS_CC_END
#endif /* defined(__CrossApp__CAScrollView__) */
