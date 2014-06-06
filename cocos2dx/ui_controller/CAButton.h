//
//  CAButton.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-3-23.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
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
    CAButtonTypeRounded3DRect
} CAButtonType;

class CAImageView;
class CCLabelTTF;

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

    void setImageForState(CAControlState controlState, CAImage* var);
    
    void setTitleForState(CAControlState controlState, std::string var);
    
    void setImageColorForState(CAControlState controlState, ccColor3B var);
    
    void setTitleColorForState(CAControlState controlState, ccColor3B var);
    
    void setTitleFontName(std::string var);
    
    void setControlState(CAControlState var);
    
public:
    
    CC_SYNTHESIZE(std::string, m_textTag, TextTag);
    
    CC_SYNTHESIZE_IS(bool, m_closeTapSound, CloseTapSound);

    CC_SYNTHESIZE_IS(bool, m_bAllowsSelected, AllowsSelected);

    CC_SYNTHESIZE_IS_READONLY(bool, m_bSelected, Selected);
    
	CC_SYNTHESIZE_IS_READONLY(bool, m_bTouchClick, TouchClick);
    
    bool isTextTagEqual(const char* text);
    
	void interruptTouchState();

    virtual void setOpacity(GLubyte opacity);

    virtual void setColor(const ccColor3B &color3);
    
    ccColor3B& getColor();
    
protected:
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:

    CAButtonType m_eButtonType;
    
    ccColor3B m_color;
    
    CAImage* m_pImage[CAControlStateAll];

    std::string m_sTitle[CAControlStateAll];

    ccColor3B m_sImageColor[CAControlStateAll];
    
    ccColor3B m_sTitleColor[CAControlStateAll];
    
    std::string m_sTitleFontName;
    
    CAImageView* m_pImageView;
    
    CCLabelTTF* m_pLabel;
    
protected:
    
    void updateWithPreferredSize();
    
    void setTouchMoved(CCPoint point);
    
    void setTouchMovedOutSide(CCPoint point);
    
    void setTouchUpSide(CCPoint point);
    
    void setTouchUpInSide(CCPoint point);
    
    bool setTouchBegin(CCPoint point);

    bool m_IsRehisterTouchDispatcher;

    void setContentSize(const CCSize & var);
    
    bool isMutableTouches(void) const;
    
    void setBackGroundViewSquareRect();
    
    void setBackGroundViewRoundedRect();
    
    void setBackGroundViewRounded3DRect();
    
/********************************************************************/
    
//Ready to give up some functions use...
    
public:
    
    CC_DEPRECATED_ATTRIBUTE void setView(CAControlState controlState, CAView* var);
    
    CC_DEPRECATED_ATTRIBUTE void setView(CAControlState controlState, CAView* var, CCPoint point);
    
    CC_DEPRECATED_ATTRIBUTE CAView* getView(CAControlState controlState);
    
protected:
    
    void updateWithPoint();
    
    CCPoint m_tSpriteNPoint;
    
    CCPoint m_tSpriteHPoint;
    
    CCPoint m_tSpriteDPoint;
    
    CCPoint m_tSpriteSPoint;
    
    CAView* m_pSpriteNormal;
    
    CAView* m_pSpriteHighlighted;
    
    CAView* m_pSpriteDisabled;
    
    CAView* m_pSpriteSelected;
    
/********************************************************************/
};

NS_CC_END

#endif /* defined(__CAButton__) */
