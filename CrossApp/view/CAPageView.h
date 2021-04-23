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
};

class CC_DLL CAPageView: public CAScrollView
{
public:
    
    enum class Orientation : int
    {
        Horizontal = 0,
        Vertical
    };
    
public:
    // event listeners. If these functions are set, the corresponding function of CAPageViewDelegate will fail.
    CC_LISTENING_FUNCTION(void(), BeginTurning);
        
    CC_LISTENING_FUNCTION(void(), EndTurning);
        
    CC_LISTENING_FUNCTION(void(unsigned int, DPoint), DidSelectedPageAtIndex);

public:

    CAPageView(CAPageView::Orientation type);
    
    virtual ~CAPageView();
    
    static CAPageView* createWithFrame(const DRect& rect, const Orientation& type);
    
    static CAPageView* createWithCenter(const DRect& rect, const Orientation& type);
    
    static CAPageView* createWithLayout(const DLayout& layout, const Orientation& type);
    
    virtual bool init() override;
    
    CC_SYNTHESIZE_READONLY(Orientation, m_eOrientation, Orientation);
    
    CC_SYNTHESIZE_DEPRECATED_ATTRIBUTE(CAPageViewDelegate* , m_pPageViewDelegate, PageViewDelegate);
    
    CC_PROPERTY_READONLY(int, m_nCurrPage, CurrPage);
    
    CC_PROPERTY(int, m_fSpacing, Spacing);
    
    void setCurrPage(int var, bool animated, bool listener = false);
    
    unsigned int getPageCount();
    
    void setViews(const CADeque<CAView*>& vec);
    
    void setViews(const CAVector<CAView*>& vec);
    
    CAView* getSubViewAtIndex(int index);
    
    virtual void setShowsScrollIndicators(bool var) override;
    
protected:

    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent) override;
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent) override;
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent) override;
    
    virtual void contentOffsetFinish() override;
    
    virtual void setContentSize(const DSize& contentSize) override;
    
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
    
    enum class State
    {
        Last,
        Next,
        None
    };
    
    CAPageView::State m_ePageViewState;
    
    CADeque<CAView*> m_pViews;
    
    bool m_bListener;
};

#define CAPageViewDirectionHorizontal CAPageView::Horizontal
#define CAPageViewDirectionVertical CAPageView::Vertical

#endif /* defined(__HelloCpp__CAPageView__) */
