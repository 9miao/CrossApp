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

typedef enum {
    CAActivityIndicatorViewStyleWhiteLarge,
    CAActivityIndicatorViewStyleWhite,
    CAActivityIndicatorViewStyleGray,
} CAActivityIndicatorViewStyle;

class CAActivityIndicatorView : public CAView {
    
public:
    CAActivityIndicatorView();
    virtual ~CAActivityIndicatorView();
    
    static CAActivityIndicatorView* create();
    static CAActivityIndicatorView* createWithFrame(const CCRect& rect);
    static CAActivityIndicatorView* createWithCenter(const CCRect& rect);
    
    virtual bool init();
    virtual bool initWithFrame(const CCRect& rect);
    virtual bool initWithCenter(const CCRect& rect);
    
    virtual void onEnter();
    virtual void onExit();
    virtual void visit();
    virtual void draw();
    
    // sizes the view according to the style
    virtual void setStyle(CAActivityIndicatorViewStyle style);
    
    virtual void startAnimating();
    virtual void stopAnimating();
    virtual bool isAnimating();

    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CAColor4B, m_color, Color);
    
private:
    float m_duration;
    bool m_animating;
    bool m_hidesWhenStopped; // default is true. calls -setHidden when animating gets set to false
    CAActivityIndicatorViewStyle m_style; // default is CAActivityIndicatorViewStyleWhite
//    CAActivityIndicatorViewStyle m_actualActivityIndicatorViewStyle;
    
private:
    CCPoint m_vertex[12][2];
    
private:
    void animation(float dt);
    int m_animationIndex;
};

NS_CC_END

#endif /* defined(__CrossApp__UIActivityIndicatorView__) */
