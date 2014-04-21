//
//  CAButton.h
//
//  Created by LI YUANFENG on 13-3-14.
//
//

#ifndef __CAButton__
#define __CAButton__

#include <iostream>
#include "base_nodes/CCNode.h"

NS_CC_BEGIN

class CAButton;

typedef enum
{
    TouchBegin          = 0,
    TouchMoved          = 1,
    TouchMovedOutSide   = 2,
    TouchUpInSide       = 3,
    TouchUpSide         = 4,
}CAButtonType;

typedef enum
{
    CAButtonStateNormal         = 0,
    CAButtonStateHighlighted    = 1,
    CAButtonStateDisabled       = 2,
    CAButtonStateSelected       = 3
}CAButtonTouchState;

typedef bool (CCObject::*SEL_CAButton)(CAButton*, CCPoint);
#define CAButton_selector(_SELECTOR) (SEL_CAButton)(&_SELECTOR)

class CAButton: public CCNodeRGBA
{
    
public:
    
    CAButton(void);
    
    virtual ~CAButton(void);

    virtual void onExitTransitionDidStart();
    
    virtual void onEnterTransitionDidFinish();
    
    static CAButton* createWithFrame(const CCRect& rect);

public:
    
    bool initWithFrame(const CCRect& rect);
    
    void setBackGroundDefault();
    
    void setBackGround(CAButtonTouchState touchState, CCNodeRGBA *var);

    CCNodeRGBA* getBackGround(CAButtonTouchState touchState);

    void setSprite(CAButtonTouchState touchState, CCNodeRGBA* var);

    void setSprite(CAButtonTouchState touchState, CCNodeRGBA* var, CCPoint point);

    CCNodeRGBA* getSprite(CAButtonTouchState touchState);
    
public:
    
    CC_SYNTHESIZE(std::string, m_textTag, TextTag);
    
    CC_SYNTHESIZE(bool, m_specially, TouchSpecially);

    CC_SYNTHESIZE(bool, m_closeTapSound, CloseTapSound);

    CC_SYNTHESIZE(bool, m_stateSelected, StateSelected);

    CC_PROPERTY(bool, m_touchClick, TouchClick);

    CC_PROPERTY(CAButtonTouchState, m_touchState, TouchState);

    bool isNormal();

    bool isHighlighted();

    bool isSelected();

    bool isDisabled();
    
    bool isTextTagEqual(const char* text);
    
    void addTarget(void* target, SEL_CAButton selector, CAButtonType type);

    virtual void setOpacity(GLubyte opacity);

    virtual void setColor(const ccColor3B &color3);
    
    ccColor3B& getColor();
    
    virtual void setContentSize(const CCSize & var);
    
    virtual void setTouchEnabled(bool enabled);
    
protected:
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void setNormalEffect();
    
    virtual void setHighlightedEffect();
    
    virtual void setDisabledEffect();
    
    virtual void setSelectedEffect();

protected:

    ccColor3B m_color;
    
    CCNodeRGBA* m_bgNormal;
    
    CCNodeRGBA* m_bgHighlighted;
    
    CCNodeRGBA* m_bgDisabled;
    
    CCNodeRGBA* m_bgSelected;
    
    CCNodeRGBA* m_spriteNormal;
    
    CCNodeRGBA* m_spriteHighlighted;
    
    CCNodeRGBA* m_spriteDisabled;
    
    CCNodeRGBA* m_spriteSelected;
    
    CCPoint m_spriteNPoint;
    
    CCPoint m_spriteHPoint;

    CCPoint m_spriteDPoint;
    
    CCPoint m_spriteSPoint;

protected:
    
    void updateWithPreferredSize();
    
    void updateWithPoint();
    
    void setTouchMoved(CCPoint point);
    
    void setTouchMovedOutSide(CCPoint point);
    
    void setTouchUpSide(CCPoint point);
    
    void setTouchUpInSide(CCPoint point);
    
    bool setTouchBegin(CCPoint point);

    bool m_IsRehisterTouchDispatcher;

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
