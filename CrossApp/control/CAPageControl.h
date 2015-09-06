//
//  CAPageControl.h
//  CrossApp
//
//  Created by dai xinping on 14-8-7.
//  Copyright (c) 2014年 CrossApp All rights reserved.
//

#ifndef __CrossApp__CAPageControl__
#define __CrossApp__CAPageControl__

#include <iostream>
#include "basics/CASTLContainer.h"
#include "CAControl.h"

NS_CC_BEGIN

class CAImage;
class CAImageView;

typedef enum
{
    CAPageControlStyleDot,
    CAPageControlStyleRound,
    CAPageControlStyleRectangle
    
}CAPageControlStyle;

class CC_DLL CAPageControl : public CAControl
{
    
public:
    CAPageControl();
    virtual ~CAPageControl();
    
    static CAPageControl* create();
    static CAPageControl* createWithFrame(const CCRect& rect);
    static CAPageControl* createWithCenter(const CCRect& rect);
    
    virtual bool init();
    virtual bool initWithFrame(const CCRect& rect);
    virtual bool initWithCenter(const CCRect& rect);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void visit();
    
    void addTarget(CAObject* target, SEL_CAControl selector);

    void removeTarget(CAObject* target, SEL_CAControl selector);
    
    CC_SYNTHESIZE(int, m_numberOfPages, NumberOfPages); // default is 0
    CC_SYNTHESIZE(int, m_currentPage, CurrentPage);     // default is 0. value pinned to 0..numberOfPages-1
    CC_SYNTHESIZE_IS(bool, m_bSinglePage, SinglePage);  // hide the the indicator if there is only one page. default is NO
    
    // if set, clicking to a new page won't update the currently displayed page until -updateCurrentPageDisplay is called. default is false
    CC_SYNTHESIZE_IS(bool, m_bDefersCurrentPageDisplay, DefersCurrentPageDisplay);
    
    // update page display to match the currentPage. ignored if m_bDefersCurrentPageDisplay is false. setting the page value directly will update immediately
    void updateCurrentPageDisplay();
    
    // returns minimum size required to display dots for given page count. can be used to size control if page count could change
    CCSize sizeForNumberOfPages(int pageCount);
    
    CC_SYNTHESIZE(CAColor4B, m_pageIndicatorTintColor, PageIndicatorTintColor);
    CC_SYNTHESIZE(CAColor4B, m_currentPageIndicatorTintColor, CurrentPageIndicatorTintColor);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pPageImage, PageIndicatorImage);
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pSelectPageImage, CurrIndicatorImage);
    
    CC_PROPERTY_PASS_BY_REF(CAPageControlStyle, m_style, Style); // default is CAPageControlStyleDot
    
    virtual void setTouchEnabled(bool enable);
        
protected:
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);

private:
    CAVector<CAImageView*> m_pIndicators;
};

NS_CC_END

#endif /* defined(__CrossApp__CAPageControl__) */
