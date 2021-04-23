//
//  CAActivityIndicatorView.h
//  CrossApp
//
//  Created by dai xinping on 14-8-18.
//  Copyright (c) 2014年 CrossApp. All rights reserved.
//

#ifndef __CrossApp__CAActivityIndicatorView__
#define __CrossApp__CAActivityIndicatorView__

#include <iostream>
#include "CAView.h"

NS_CC_BEGIN

class CAScale9ImageView;
class CAImageView;
class CC_DLL CAActivityIndicatorView : public CAView
{
    
public:
    
    enum class Style : int
    {
        WhiteLarge = 0,
        GrayLarge,
        White,
        Gray,
        Image
    };
    
public:
    
    CAActivityIndicatorView();
    virtual ~CAActivityIndicatorView();
    
    static CAActivityIndicatorView* create();
    static CAActivityIndicatorView* createWithFrame(const DRect& rect);
    static CAActivityIndicatorView* createWithCenter(const DRect& rect);
    static CAActivityIndicatorView* createWithLayout(const DLayout& layout);
    
    virtual bool init();

    virtual void onEnter();
    virtual void onExit();
    
    void setTimesOneCycle(float times);
    void setCycleTime(float time);
    // sizes the view according to the style
    void setStyle(CAActivityIndicatorView::Style style);
    
    void setActivityIndicatorOffset(const DSize& offset);
    
    // will set CAActivityIndicatorView::Style::Image when call this func
    CC_PROPERTY(CAView*, m_pImageView, ActivityIndicatorView);
    
    CC_PROPERTY(CAView*, m_pBackView, ActivityBackView);
    
    void startAnimating();
    void stopAnimating();
    bool isAnimating();

    void onCancel(const std::function<void()>& callBack);
    
    CC_SYNTHESIZE(float, m_fLoadingMinTime, LoadingMinTime);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CAColor4B, m_color, Color);
    
protected:
    void setContentSize(const DSize & var);

private:
    float m_fLoadingTime;
    float m_duration;
    bool m_bStopAnimation;
    bool m_hidesWhenStopped; // default is true. calls -setHidden when animating gets set to false
    CAActivityIndicatorView::Style m_style; // default is CAActivityIndicatorView::Style::White
    
    CAObject* m_pTarget;
    std::function<void()> m_pCallFunc;
    DSize m_obActivityIndicatorOffset;
    
private:
    std::vector<float> m_vRotate;
    
private:
    void animation(float dt);
    int m_animationIndex;
    int m_nTimesOneCycle;
    float m_fCycleTime;
};

NS_CC_END

#endif /* defined(__CrossApp__UIActivityIndicatorView__) */
