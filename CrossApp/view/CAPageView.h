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
#include "CrossApp.h"

USING_NS_CC;
class CC_DLL CAPageView;
class CC_DLL CAPageViewDelegate
{
    
public:
    
    virtual ~CAPageViewDelegate(){};
    
    virtual void pageViewDidBeginTurning(CAPageView* pageView){};
    
    virtual void pageViewDidEndTurning(CAPageView* pageView){};
};

class CC_DLL CAPageView: public CAScrollView
{
public:
    
    typedef enum
    {
        CAPageViewDirectionHorizontal,
        CAPageViewDirectionVertical
    }CAPageViewDirection;
    
    CAPageView(CAPageViewDirection type);
    
    virtual ~CAPageView();
    
    static CAPageView* createWithFrame(const CCRect& rect, CAPageViewDirection type);
    
    static CAPageView* createWithCenter(const CCRect& rect, CAPageViewDirection type);
    
    bool init();
    
    CC_SYNTHESIZE_READONLY(CAPageViewDirection, m_ePageViewDirection, PageViewDirection);
    
    CC_SYNTHESIZE(CAPageViewDelegate* , m_pPageViewDelegate, PageViewDelegate);
    
    CC_PROPERTY_READONLY(int, m_nCurrPage, CurrPage);
    
    void setCurrPage(int var, bool animated);
    
    int getPageCount();
    
    void setViews(CAVector<CAView*> vec);
    
protected:

    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void contentOffsetFinish();
    
private:
    
    using CAScrollView::setBounceHorizontal;
    
    using CAScrollView::isBounceHorizontal;
    
    using CAScrollView::setBounceVertical;
    
    using CAScrollView::isBounceVertical;
    
    using CAScrollView::setShowsHorizontalScrollIndicator;
    
    using CAScrollView::isShowsHorizontalScrollIndicator;
    
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
    
    using CAResponder::setTouchMovedListenHorizontal;
    
    using CAResponder::setTouchMovedListenVertical;
    
private:
    
    typedef enum
    {
        CAPageViewLast,
        CAPageViewNext,
        CAPageViewNone
    }CAPageViewState;
    
    CAPageViewState m_ePageViewState;
    
    CAVector<CAView*> m_pViews;
};


#endif /* defined(__HelloCpp__CAPageView__) */
