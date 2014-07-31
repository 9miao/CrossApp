//
//  CAButton.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-3-23.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//


#ifndef __CAButton__
#define __CAButton__

#include <iostream>
#include "CAControl.h"
NS_CC_BEGIN

typedef enum
{
    CAButtonTypeCustom = 0,
    CAButtonTypeSquareRect,
    CAButtonTypeRoundedRect,
} CAButtonType;

class CAImageView;
class CALabel;

class CC_DLL CAButton : public CAControl
{
    
public:
    
    CAButton(CAButtonType buttonType);
    
    virtual ~CAButton(void);

    virtual void onExitTransitionDidStart();
    
    virtual void onEnterTransitionDidFinish();
    
    static CAButton* create(CAButtonType buttonType);
    
    static CAButton* createWithFrame(const CCRect& rect, CAButtonType buttonType);

    static CAButton* createWithCenter(const CCRect& rect, CAButtonType buttonType);
    
public:
    
    virtual bool init();
    
    virtual bool initWithFrame(const CCRect& rect);
    
    virtual bool initWithCenter(const CCRect& rect);
    
    void setBackGroundViewForState(CAControlState controlState, CAView *var);
    
    CAView* getBackGroundViewForState(CAControlState controlState);
    
    void setImageForState(CAControlState controlState, CAImage* var);
    
    void setTitleForState(CAControlState controlState, std::string var);
    
    void setImageColorForState(CAControlState controlState, CAColor4B var);
    
    void setTitleColorForState(CAControlState controlState, CAColor4B var);
    
    void setTitleFontName(std::string var);
    
    void setControlState(CAControlState var);
    
    using CAControl::addTarget;
    
    using CAControl::removeTarget;
    
    using CAControl::removeAllTargets;
    
public:
    
    CC_SYNTHESIZE(std::string, m_textTag, TextTag);
    
    CC_SYNTHESIZE_IS(bool, m_closeTapSound, CloseTapSound);

    CC_SYNTHESIZE_IS(bool, m_bAllowsSelected, AllowsSelected);

    CC_SYNTHESIZE_IS_READONLY(bool, m_bSelected, Selected);
    
	CC_SYNTHESIZE_IS_READONLY(bool, m_bTouchClick, TouchClick);
    
    bool isTextTagEqual(const char* text);
    
	void interruptTouchState();

public:
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
protected:

    CAButtonType m_eButtonType;
    
    CAColor4B m_color;
    
    CAImage* m_pImage[CAControlStateAll];

    std::string m_sTitle[CAControlStateAll];

    CAColor4B m_sImageColor[CAControlStateAll];
    
    CAColor4B m_sTitleColor[CAControlStateAll];
    
    std::string m_sTitleFontName;
    
    CAImageView* m_pImageView;
    
    CALabel* m_pLabel;
    
    CAView* m_pBackGroundView[CAControlStateAll];
    
protected:
    
    void updateWithPreferredSize();
    
    void setTouchMoved(CCPoint point);
    
    void setTouchMovedOutSide(CCPoint point);
    
    void setTouchUpSide(CCPoint point);
    
    void setTouchUpInSide(CCPoint point);
    
    bool setTouchBegin(CCPoint point);

    void setContentSize(const CCSize & var);

    void setBackGroundViewSquareRect();
    
    void setBackGroundViewRoundedRect();
};

NS_CC_END

#endif /* defined(__CAButton__) */
