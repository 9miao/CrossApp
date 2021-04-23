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

class CC_DLL CAPageControl : public CAControl
{
    
public:
    
    enum class Style : int
    {
        Dot = 0,
        Round,
        Rectangle
    };
    
public:
    
    CAPageControl();
    virtual ~CAPageControl();
    
    static CAPageControl* create();
    static CAPageControl* createWithFrame(const DRect& rect);
    static CAPageControl* createWithCenter(const DRect& rect);
    static CAPageControl* createWithLayout(const DLayout& layout);
                                      
    virtual bool init();
    virtual bool initWithFrame(const DRect& rect);
    virtual bool initWithCenter(const DRect& rect);
    
    void setTarget(const std::function<void(int)>& function);
    
    CC_SYNTHESIZE(int, m_numberOfPages, NumberOfPages); // default is 0
    CC_SYNTHESIZE(int, m_currentPage, CurrentPage);     // default is 0. value pinned to 0..numberOfPages-1
    CC_SYNTHESIZE_IS(bool, m_bSinglePage, SinglePage);  // hide the the indicator if there is only one page. default is NO
    
    // if set, clicking to a new page won't update the currently displayed page until -updateCurrentPageDisplay is called. default is false
    CC_SYNTHESIZE_IS(bool, m_bDefersCurrentPageDisplay, DefersCurrentPageDisplay);
    
    // update page display to match the currentPage. ignored if m_bDefersCurrentPageDisplay is false. setting the page value directly will update immediately
    void updateCurrentPageDisplay();
    
    CC_SYNTHESIZE(CAColor4B, m_pageIndicatorTintColor, PageIndicatorTintColor);
    CC_SYNTHESIZE(CAColor4B, m_currentPageIndicatorTintColor, CurrentPageIndicatorTintColor);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pPageImage, PageIndicatorImage);
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pSelectPageImage, CurrIndicatorImage);
    
    CC_PROPERTY(CAPageControl::Style, m_style, Style); // default is CAPageControl::Style::Dot
    
    virtual void setTouchEnabled(bool enable);
        
protected:
        
    virtual void onExitTransitionDidStart();
    virtual void onEnterTransitionDidFinish();
        
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);

private:
    CAVector<CAImageView*> m_pIndicators;
    
    std::function<void(int)> m_function{nullptr};
};

NS_CC_END

#endif /* defined(__CrossApp__CAPageControl__) */
