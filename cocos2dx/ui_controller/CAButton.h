//
//  CAButton.h
//  cocos2dx
//
//  Created by Li Yuanfeng on 14-3-23.
//  Copyright (c) 2014 www.9miao.com All rights reserved.
//


#ifndef __CAButton__
#define __CAButton__

#include <iostream>
#include "CAControl.h"

NS_CC_BEGIN

class CAButton;

typedef bool (CCObject::*SEL_CAButton)(CAButton*, CCPoint);
#define CAButton_selector(_SELECTOR) (SEL_CAButton)(&_SELECTOR)

class CC_DLL CAButton : public CAControl
{
    
public:
    
    CAButton(void);
    
    virtual ~CAButton(void);

    virtual void onExitTransitionDidStart();
    
    virtual void onEnterTransitionDidFinish();
    
    static CAButton* createWithFrame(const CCRect& rect);

    static CAButton* createWithCenter(const CCRect& rect);
    
public:
    
    bool initWithFrame(const CCRect& rect);
    
    bool initWithCenter(const CCRect& rect);
    
    void setBackGroundDefault();
    
    void setBackGround(CAControlState controlState, CAView *var);

    void setSprite(CAControlState controlState, CAView* var);

    void setSprite(CAControlState controlState, CAView* var, CCPoint point);

    CAView* getSprite(CAControlState controlState);
    
    virtual void setControlState(CAControlState var);
    
public:
    
    CC_SYNTHESIZE(std::string, m_textTag, TextTag);
    
    CC_SYNTHESIZE(bool, m_closeTapSound, CloseTapSound);

    CC_SYNTHESIZE(bool, m_stateSelected, StateSelected);

	CC_SYNTHESIZE_IS_READONLY(bool, m_touchClick, TouchClick);

    bool isTextTagEqual(const char* text);
    
    void addTarget(void* target, SEL_CAButton selector, CAControlType type);

	void InterruptTouchState();

    virtual void setOpacity(GLubyte opacity);

    virtual void setColor(const ccColor3B &color3);
    
    ccColor3B& getColor();
    
    virtual void setTouchEnabled(bool enabled);
    
protected:
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:

    ccColor3B m_color;
    
    CAView* m_pSpriteNormal;
    
    CAView* m_pSpriteHighlighted;
    
    CAView* m_pSpriteDisabled;
    
    CAView* m_pSpriteSelected;
    
    CCPoint m_tSpriteNPoint;
    
    CCPoint m_tSpriteHPoint;

    CCPoint m_tSpriteDPoint;
    
    CCPoint m_tSpriteSPoint;

protected:
    
    void updateWithPreferredSize();
    
    void updateWithPoint();
    
    void setTouchMoved(CCPoint point);
    
    void setTouchMovedOutSide(CCPoint point);
    
    void setTouchUpSide(CCPoint point);
    
    void setTouchUpInSide(CCPoint point);
    
    bool setTouchBegin(CCPoint point);

    bool m_IsRehisterTouchDispatcher;

    virtual void setContentSize(const CCSize & var);
    
private:
    
    void* m_target;
    
    SEL_CAButton m_touchBegin;
    
    SEL_CAButton m_touchMoved;
    
    SEL_CAButton m_touchMovedOutSide;
    
    SEL_CAButton m_touchUpInSide;
    
    SEL_CAButton m_touchUpSide;
    
};

NS_CC_END

#endif /* defined(__CAButton__) */
