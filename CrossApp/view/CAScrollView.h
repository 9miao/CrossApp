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
    
    enum class MultitouchGesture
    {
        Zoom,
        Rotate,
        ZoomAndRotate,
        None
    };
    
public:
    // event listeners. If these functions are set, the corresponding function of CAScrollViewDelegate will fail.
    
    CC_LISTENING_FUNCTION(void(), DidMoved);
    
    CC_LISTENING_FUNCTION(void(), StopMoved);
    
    CC_LISTENING_FUNCTION(void(), WillBeginDragging);
    
    CC_LISTENING_FUNCTION(void(), Dragging);
    
    CC_LISTENING_FUNCTION(void(), DidEndDragging);
    
    CC_LISTENING_FUNCTION(void(), DidZoom);
    
    CC_LISTENING_FUNCTION(void(DPoint), TouchUpWithoutMoved);
    
    CC_LISTENING_FUNCTION(void(), HeaderBeginRefreshing);
    
    CC_LISTENING_FUNCTION(void(), FooterBeginRefreshing);
    
public:

    CAScrollView();
    
    virtual ~CAScrollView();
 
    virtual void onEnterTransitionDidFinish() override;
    
    virtual void onExitTransitionDidStart() override;
    
    static CAScrollView* createWithFrame(const DRect& rect);
    
    static CAScrollView* createWithCenter(const DRect& rect);
    
    static CAScrollView* createWithLayout(const DLayout& layout);
    
    virtual bool init() override;

    virtual void addSubview(CAView* subview) override;
    
    virtual void insertSubview(CAView* subview, int z) override;
    
    void removeAllSubviews() override;
    
    void removeSubview(CAView* subview) override;

    void removeSubviewByTag(int tag) override;
    
    void removeSubviewByTextTag(const std::string& textTag) override;
    
    CAView* getSubviewByTag(int aTag) override;
    
    CAView* getSubviewByTextTag(const std::string& textTag) override;
    
    const CAVector<CAView*>& getSubviews() override;
    
    void setContentOffset(const DPoint& offset, bool animated);
    
    DPoint getContentOffset();
    
    void setBackgroundImage(CAImage* image, bool isScale9 = false);

    void setZoomScale(float zoom);

    virtual bool isReachBoundaryLeft() override;
    
    virtual bool isReachBoundaryRight() override;
    
    virtual bool isReachBoundaryUp() override;
    
    virtual bool isReachBoundaryDown() override;
    
    CC_PROPERTY_IS(bool, m_bTouchEnabledAtSubviews, TouchEnabledAtSubviews);
    
    CC_PROPERTY_IS(bool, m_bShowsScrollIndicators, ShowsScrollIndicators);// default is true
    
    CC_PROPERTY_IS(bool, m_bShowsHorizontalScrollIndicator, ShowsHorizontalScrollIndicator);// default is true
    
    CC_PROPERTY_IS(bool, m_bShowsVerticalScrollIndicator, ShowsVerticalScrollIndicator);// default is true
    
    CC_SYNTHESIZE_DEPRECATED_ATTRIBUTE(CAScrollViewDelegate*, m_pScrollViewDelegate, ScrollViewDelegate);
    
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
    
    virtual float maxBouncesLength()
    {
        return 120.0f;
    }
    
    virtual void contentOffsetFinish() {};
    
    void getScrollWindowNotOutPoint(DPoint& point);
    
    virtual float getScrollWindowNotOutHorizontal(float x);
    
    virtual float getScrollWindowNotOutVertical(float y);
    
    bool isScrollWindowNotOutSide();
    
    bool isScrollWindowNotMaxOutSide(const DPoint& point);
    
    virtual void update(float dt) override;

    void deaccelerateScrolling(float dt);
    
    void initIndicator();
    
    void showIndicator();
    
    void hideIndicator();

    void updateIndicator();
    
    void layoutPullToRefreshView();
    
    void changedFromPullToRefreshView();
    
    void detectionFromPullToRefreshView();

    void headerWillBeginRefreshing();
    
    void footerWillBeginRefreshing();
    
    bool isHeaderRefreshing();
    
    bool isFooterRefreshing();
    
    void setContainerPoint(const DPoint& point, const DSize& size = DSize(-1, -1));
    
    virtual void setContentSize(const DSize& var) override;
    
    void updatePointOffset(float dt = 0);
    
    void updateIndicatorLayout();
    
public:
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent) override;
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent) override;
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent) override;
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent) override;
    
    virtual void mouseScrollWheel(CATouch* pTouch, float off_x, float off_y, CAEvent* pEvent) override;
    
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
    
    CAView* m_pBackgroundView;
    
    bool m_bPCMode;
};


class CC_DLL CAIndicator : public CAView
{
public:
    
    enum class Orientation : int
    {
        Horizontal = 0,
        Vertical
    };
    
public:
    
        CAIndicator(CAIndicator::Orientation type, CAScrollView* var);
    
    virtual ~CAIndicator();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    static CAIndicator* create(CAIndicator::Orientation type, CAScrollView* var);
    
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
    
    CAIndicator::Orientation m_eType;
    
    bool m_bPCMode;
    
    bool m_bTouch;
    
    bool m_bHide;
};


NS_CC_END
#endif /* defined(__CrossApp__CAScrollView__) */
