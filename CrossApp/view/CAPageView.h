//
//  CAPageView.h
//  HelloCpp
//
//  Created by 栗元峰 on 14-7-29.
//
//

#ifndef __HelloCpp__CAPageView__
#define __HelloCpp__CAPageView__

#include <iostream>
#include "CAScrollView.h"
#include "basics/CASTLContainer.h"

USING_NS_CC;

class CC_DLL CAPageView;
class CC_DLL CAPageViewDelegate
{
    
public:
    
    virtual ~CAPageViewDelegate(){};
    
    virtual void pageViewDidBeginTurning(CAPageView* pageView){};
    
    virtual void pageViewDidEndTurning(CAPageView* pageView){};
    
    virtual void pageViewDidSelectedPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point){};
    
    CC_DEPRECATED_ATTRIBUTE virtual void pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point){};
};

class CC_DLL CAPageView: public CAScrollView
{
public:
    
    typedef enum
    {
        Horizontal = 0,
        Vertical
    }
    Orientation;
    
    CAPageView(const Orientation& type);
    
    virtual ~CAPageView();
    
    static CAPageView* createWithFrame(const DRect& rect, const Orientation& type);
    
    static CAPageView* createWithCenter(const DRect& rect, const Orientation& type);
    
    static CAPageView* createWithLayout(const DLayout& layout, const Orientation& type);
    
    bool init();
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(Orientation, m_eOrientation, Orientation);
    
    CC_SYNTHESIZE(CAPageViewDelegate* , m_pPageViewDelegate, PageViewDelegate);
    
    CC_PROPERTY_READONLY(int, m_nCurrPage, CurrPage);
    
    CC_PROPERTY(int, m_fSpacing, Spacing);
    
    void setCurrPage(int var, bool animated, bool listener = false);
    
    unsigned int getPageCount();
    
    void setViews(const CADeque<CAView*>& vec);
    
    void setViews(const CAVector<CAView*>& vec);
    
    CAView* getSubViewAtIndex(int index);
    
    virtual void setShowsScrollIndicators(bool var);
    
protected:

    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void contentOffsetFinish(float dt);
    
    virtual void setContentSize(const DSize& contentSize);
    
    void runAnimation(bool animated);
    
private:
    
    using CAScrollView::setBounces;
    
    using CAScrollView::setBounceHorizontal;
    
    using CAScrollView::isBounceHorizontal;
    
    using CAScrollView::setBounceVertical;
    
    using CAScrollView::isBounceVertical;
    
    using CAScrollView::setShowsHorizontalScrollIndicator;
    
    using CAScrollView::isShowsHorizontalScrollIndicator;
    
    using CAScrollView::setShowsVerticalScrollIndicator;
    
    using CAScrollView::isShowsVerticalScrollIndicator;
    
    using CAScrollView::setViewSize;
    
    using CAScrollView::setMinimumZoomScale;
    
    using CAScrollView::getMinimumZoomScale;
    
    using CAScrollView::setMaximumZoomScale;
    
    using CAScrollView::getMaximumZoomScale;
    
    using CAScrollView::getZoomScale;
    
    using CAScrollView::isZooming;
    
    using CAScrollView::addSubview;
    
    using CAScrollView::insertSubview;
    
    using CAScrollView::removeAllSubviews;
    
    using CAScrollView::removeSubview;
    
    using CAScrollView::removeSubviewByTag;
    
    using CAScrollView::getSubviewByTag;
    
private:
    
    typedef enum
    {
        Last,
        Next,
        None
    }State;
    
    State m_ePageViewState;
    
    CADeque<CAView*> m_pViews;
    
    bool m_bListener;
};

#define CAPageViewDirectionHorizontal CAPageView::Horizontal
#define CAPageViewDirectionVertical CAPageView::Vertical

#endif /* defined(__HelloCpp__CAPageView__) */
