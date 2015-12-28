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
    
    static CAButton* createWithFrame(const DRect& rect, const CAButtonType& buttonType);
    
    static CAButton* createWithCenter(const DRect& rect, const CAButtonType& buttonType);
    
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
    
    void setImageOffset(const DSize& offset);
    
    void setImageSize(const DSize& size);
    
    void setTitleOffset(const DSize& offset);
    
    void setTitleLabelSize(const DSize& size);
    
    void setTitleFontSize(float fontSize);
    
    virtual void setControlState(const CAControlState& var);
    
    using CAControl::addTarget;
    
    using CAControl::removeTarget;
    
    using CAControl::removeAllTargets;
    
public:
    
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

    CAImageView* m_pImageView;
    
    CALabel* m_pLabel;
    
    CAView* m_pBackGroundView[CAControlStateAll];
    
    std::string m_sTitleFontName;
    
    float m_fTitleFontSize;
        
    DSize m_pTitleLabelSize;
    
    bool m_bDefineTitleLabelSize;
    
    DSize m_pImageSize;
    
    bool m_bDefineImageSize;
    
    DSize m_pTitleOffset;
    
    bool m_bDefineTitleOffset;
    
    DSize m_pImageOffset;
    
    bool m_bDefineImageOffset;
    
protected:
    
    void updateWithPreferredSize();
    
    void setTouchMoved(const DPoint& point);
    
    void setTouchMovedOutSide(const DPoint& point);
    
    void setTouchUpOutSide(const DPoint& point);
    
    void setTouchUpInSide(const DPoint& point);
    
    bool setTouchBegin(const DPoint& point);

    void setTouchLongPress(float dt);
    
    void setContentSize(const DSize & var);

    void setBackGroundViewSquareRect();
    
    void setBackGroundViewRoundedRect();
};

NS_CC_END

#endif /* defined(__CAButton__) */
