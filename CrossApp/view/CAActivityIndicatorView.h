//
//  CAActivityIndicatorView.h
//  CrossApp
//
//  Created by dai xinping on 14-8-18.
//  Copyright (c) 2014年 cocos2d-x. All rights reserved.
//

#ifndef __CrossApp__CAActivityIndicatorView__
#define __CrossApp__CAActivityIndicatorView__

#include <iostream>
#include "CAView.h"

NS_CC_BEGIN

typedef enum
{
    CAActivityIndicatorViewStyleWhiteLarge,
    CAActivityIndicatorViewStyleWhite,
    CAActivityIndicatorViewStyleGray,
    CAActivityIndicatorViewStyleImage,
}
CAActivityIndicatorViewStyle;

class CAScale9ImageView;
class CC_DLL CAActivityIndicatorView : public CAView
{
    
public:
    CAActivityIndicatorView();
    virtual ~CAActivityIndicatorView();
    
    static CAActivityIndicatorView* create();
    static CAActivityIndicatorView* createWithFrame(const CCRect& rect);
    static CAActivityIndicatorView* createWithCenter(const CCRect& rect);
    
    virtual bool init();

    virtual void onEnter();
    virtual void onExit();
    virtual void visit();
    virtual void draw();
    
    // sizes the view according to the style
    virtual void setStyle(CAActivityIndicatorViewStyle style);
    
    // will set CAActivityIndicatorViewStyleImage when call this func
    CC_PROPERTY(CAView*, m_pImageView, ActivityIndicatorView);
    
    // 
    CC_PROPERTY(CAView*, m_pBackView, ActivityBackView);
    
    void startAnimating();
    void stopAnimating();
    bool isAnimating();

    void setTargetOnCancel(CAObject* target, SEL_CallFunc callBack);
    
    CC_SYNTHESIZE(float, m_fLoadingMinTime, LoadingMinTime);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CAColor4B, m_color, Color);
    
protected:
    void setContentSize(const CCSize & var);

private:
    float m_fLoadingTime;
    float m_duration;
    bool m_bStopAnimation;
    bool m_hidesWhenStopped; // default is true. calls -setHidden when animating gets set to false
    CAActivityIndicatorViewStyle m_style; // default is CAActivityIndicatorViewStyleWhite
    
    CAObject* m_pTarget;
    SEL_CallFunc m_pCallFunc;
    
private:
    CCPoint m_vertex[12][2];
    
private:
    void animation(float dt);
    int m_animationIndex;
};

NS_CC_END

#endif /* defined(__CrossApp__UIActivityIndicatorView__) */
