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
    
    CAButton(const CAButtonType& buttonType);
    
    virtual ~CAButton(void);
    
    virtual void onExitTransitionDidStart();
    
    virtual void onEnterTransitionDidFinish();
    
    static CAButton* create(const CAButtonType& buttonType);
    
    static CAButton* createWithFrame(const CCRect& rect, const CAButtonType& buttonType);
    
    static CAButton* createWithCenter(const CCRect& rect, const CAButtonType& buttonType);
    
public:
    
    virtual bool init();
    
    void setBackGroundViewForState(const CAControlState& controlState, CAView *var);
    
    CAView* getBackGroundViewForState(const CAControlState& controlState);
    
    void setImageForState(const CAControlState& controlState, CAImage* var);
    
    CAImage* getImageForState(const CAControlState& controlState);
    
    void setTitleForState(const CAControlState& controlState, const std::string& var);
    
    const std::string& getTitleForState(const CAControlState& controlState);
    
    void setImageColorForState(const CAControlState& controlState, const CAColor4B& var);
    
    void setTitleColorForState(const CAControlState& controlState, const CAColor4B& var);
    
    void setTitleFontName(const std::string& var);
    
    virtual void setControlState(const CAControlState& var);
    
    using CAControl::addTarget;
    
    using CAControl::removeTarget;
    
    using CAControl::removeAllTargets;
    
public:
    
    CC_SYNTHESIZE_IS(bool, m_closeTapSound, CloseTapSound);

    CC_SYNTHESIZE_IS(bool, m_bAllowsSelected, AllowsSelected);

    CC_SYNTHESIZE_IS_READONLY(bool, m_bSelected, Selected);
    
	CC_SYNTHESIZE_IS_READONLY(bool, m_bTouchClick, TouchClick);

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
    
    void setTouchMoved(const CCPoint& point);
    
    void setTouchMovedOutSide(const CCPoint& point);
    
    void setTouchUpSide(const CCPoint& point);
    
    void setTouchUpInSide(const CCPoint& point);
    
    bool setTouchBegin(const CCPoint& point);

    void setContentSize(const CCSize & var);

    void setBackGroundViewSquareRect();
    
    void setBackGroundViewRoundedRect();
};

NS_CC_END

#endif /* defined(__CAButton__) */
