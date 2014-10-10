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
    
    virtual void pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const CCPoint& point){};
};

class CC_DLL CAPageView: public CAScrollView
{
public:
    
    typedef enum
    {
        CAPageViewDirectionHorizontal,
        CAPageViewDirectionVertical
    }CAPageViewDirection;
    
    CAPageView(const CAPageViewDirection& type);
    
    virtual ~CAPageView();
    
    static CAPageView* createWithFrame(const CCRect& rect, const CAPageViewDirection& type);
    
    static CAPageView* createWithCenter(const CCRect& rect, const CAPageViewDirection& type);
    
    bool init();
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CAPageViewDirection, m_ePageViewDirection, PageViewDirection);
    
    CC_SYNTHESIZE(CAPageViewDelegate* , m_pPageViewDelegate, PageViewDelegate);
    
    CC_PROPERTY_READONLY(int, m_nCurrPage, CurrPage);
    
    void setCurrPage(int var, bool animated, bool listener = false);
    
    int getPageCount();
    
    void setViews(const CADeque<CAView*>& vec);
    
    void setViews(const CAVector<CAView*>& vec);
    
    CAView* getSubViewAtIndex(int index);
    
protected:

    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void contentOffsetFinish(float dt);
    
    inline virtual float minSpeed(float dt)
    {
        return (3000 * dt);
    }
    
    inline virtual float maxSpeed(float dt)
    {
        return (CCPoint(m_obContentSize).getLength() * 3000 * dt);
    }
    
private:
    
    using CAScrollView::setBounces;
    
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
    
    CADeque<CAView*> m_pViews;
    
    bool m_bListener;
};


#endif /* defined(__HelloCpp__CAPageView__) */
