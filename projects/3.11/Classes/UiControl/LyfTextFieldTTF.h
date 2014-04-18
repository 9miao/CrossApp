//
//  LTextField.h
//  mir-mobile
//
//  Created by 栗元峰 on 13-12-12.
//
//

#ifndef __mir_mobile__LTextField__
#define __mir_mobile__LTextField__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

NS_CC_BEGIN

class LyfTextFieldTTF: public CCTextFieldTTF{
    
public:
    
	LyfTextFieldTTF();
    
	virtual ~LyfTextFieldTTF();
    
	static LyfTextFieldTTF * textFieldWithPlaceHolder(const char *placeholder, const CCSize& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize);
    
	static LyfTextFieldTTF * textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);
    
	virtual void onEnter();
    
	virtual void onExit();
    
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
	/**
	 @brief    Open keyboard and receive input text.
	 */
	virtual bool attachWithIME();
    
	/**
	 @brief    End text input and close keyboard.
	 */
	virtual bool detachWithIME();
    
	virtual void setString(const char *text);
    
	virtual void deleteBackward();
    
    virtual void setPlaceHolder(const char * text);
    
    virtual void setTouchDispatcher(bool bSwallowsTouches);
    
    void registerWithTouchDispatcher();
    
    void unRegisterWithTouchDispatcher();
    
    CC_SYNTHESIZE(int, m_nMaxLength, MaxLength);
    
    CC_SYNTHESIZE(CCSize, m_nSize, Size);
    
    CC_SYNTHESIZE(int, m_nType, Type);
    
private:
    
	void initCursor(int height);
    
	bool isInTextField(CCTouch *pTouch);
    
    void setContentSize(const cocos2d::CCSize &contentSize);
    
private:
	// 点击开始位置
	CCPoint m_beginPos;
    
	// 光标精灵
	CCSprite *m_pCursorSprite;
    
	// 光标动画
	CCAction *m_pCursorAction;
    
    bool m_isInputState;
    
    bool m_nbSwallowsTouches;
    
    bool m_IsRehisterTouchDispatcher;
    
    CCPoint m_touchBeginUIPoint;
};

NS_CC_END

#endif /* defined(__mir_mobile__LTextField__) */
