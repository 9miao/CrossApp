//
//  LMenu.h
//
//  Created by LI YUANFENG on 13-3-14.
//
//

#ifndef __LMenu__
#define __LMenu__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace extension;
class LMenu;

typedef enum
{
    TouchBegin = 0,
    TouchMoved = 1,
    TouchMovedOutSide = 2,
    TouchUpInSide = 3,
    TouchUpSide = 4,
}LMenuType;

typedef enum
{
    LMenuStateNormal = 0,
    LMenuStateHighlighted = 1,
    LMenuStateDisabled = 2,
    LMenuStateSelected = 3
}LMenuTouchState;

typedef bool (CCObject::*SEL_LMenu)(LMenu*, CCPoint);
#define LMenu_selector(_SELECTOR) (SEL_LMenu)(&_SELECTOR)

class LMenu: public CCNodeRGBA
{
    
public:
    
    LMenu(void);
    
    virtual ~LMenu(void);
    
    /**
     *  按钮无背景 设置按钮层区域
     */
    
    static LMenu* create(const CCSize &size);
    
    /**
     *  按钮背景 任何基于CCRGBAProtocol
     */
    
    static LMenu* createWithRGBAProtocol(CCRGBAProtocol* backGround);
    
    /**
     *  按钮背景 CCSprite
     */
    
    static LMenu* createWithSprite(CCSprite* backGround);
    
    /**
     *  按钮背景 CCScale9Sprite
     */
    
    static LMenu* createWithScale9Sprite(CCScale9Sprite* backGround);
    
    /**
     *  按钮背景 CCLayerColor
     */
    
    static LMenu* createWithLayerColor(CCLayerColor* backGround);
    
public:
    
    /**
     *  按钮背景 CCNode
     *
     *  @param 类型 任何基于CCNode
     *  @param 例如 CCSprite、CCScale9Sprite、CCLayerColor等
     */
    
    bool initWithBackGround(CCRGBAProtocol* backGround);
    
    /**
     *  按钮背景 CCSprite
     */
    
    bool initWithSpriteBackGround(CCSprite* backGround);
    
    /**
     *  按钮背景 CCScale9Sprite
     */
    
    bool initWithScale9SpriteBackGround(CCScale9Sprite* backGround);
    
    /**
     *  按钮背景 CCLayerColor
     */
    
    bool initWithLayerColorBackGround(CCLayerColor* backGround);
    
    /**
     *  按钮在各种状态背景 CCNode
     *
     *  @param 类型 任何基于CCNode
     *  @param 例如 CCSprite、CCScale9Sprite、CCLayerColor等
     */
    
    bool setBackGround(LMenuTouchState touchState, CCRGBAProtocol *var);

    /**
     *  按钮在各种状态背景 CCSprite
     */
    
    bool setSpriteBackGround(LMenuTouchState touchState, CCSprite *var);
    
    /**
     *  按钮在各种状态背景 CCScale9Sprite
     */
    
    bool setScale9SpriteBackGround(LMenuTouchState touchState, CCScale9Sprite *var);
    
    /**
     *  按钮在各种状态背景 CCLayerColor
     */
    
    bool setLayerColorBackGround(LMenuTouchState touchState, CCLayerColor *var);
    
    /**
     *  获取按钮背景
     */
    
    CCNode* getBackGround(LMenuTouchState touchState);
    
    /**
     *  按钮上层 Sprite
     *
     *  @param 类型 任何基于CCNodeRGBA
     *  @param 例如 CCSprite、CCScale9Sprite
     *  @param 默认坐标 居中
     */
    
    void setSprite(LMenuTouchState touchState, CCNodeRGBA* var);
    
    /**
     *  按钮上层 Sprite
     *
     *  @param 类型 任何基于CCNodeRGBA
     *  @param 例如 CCSprite、CCScale9Sprite
     *  @param 设置坐标，各种状态坐标需分别设置
     */
    
    void setSprite(LMenuTouchState touchState, CCNodeRGBA* var, CCPoint point);
    
    /**
     *  获取按钮上层 Sprite
     */
    
    CCNodeRGBA* getSprite(LMenuTouchState touchState);
    
public:
    
    /**
     *  按钮标记值，int型
     */
    
    CC_SYNTHESIZE(int, m_sign, Sign);
    
    /**
     *  按钮标记值，字符串型
     */
    
    CC_SYNTHESIZE(std::string, m_textTag, TextTag);
    
    /**
     *  按钮点击特效开关与开关状态获取，bool
     */
    
    CC_SYNTHESIZE(bool, m_specially, TouchSpecially);
    
    /**
     *  按钮点击音效开关与开关状态获取，bool
     */
    
    CC_SYNTHESIZE(bool, m_closeTapSound, CloseTapSound);
    
    /**
     *  按钮选中状态开关与开关状态获取，bool
     */
    
    CC_SYNTHESIZE(bool, m_stateSelected, StateSelected);
    
    /**
     *  按钮触摸事件开关与开关状态获取，bool
     */
    
    CC_PROPERTY(bool, m_bIsTouchEnabled, TouchEnabled);
    
    /**
     *  按钮触摸状态开关与开关状态获取，bool
     */
    
    CC_PROPERTY(bool, m_touchClick, TouchClick);
    
    /**
     *  按钮状态开关与开关状态获取，enum
     */
    
    CC_PROPERTY(LMenuTouchState, m_touchState, TouchState);//按钮状态

    /**
     *  按钮大小设置与获取，CCSize
     */
    
    CC_PROPERTY(CCSize, m_preferredSize, PreferredSize);//设置缩放后大小
    
    /**
     *  获取按钮状态：普通
     */
    
    bool isNormal();
    
    /**
     *  获取按钮状态：高亮
     */
    
    bool isHighlighted();
    
    /**
     *  获取按钮状态：选中
     */
    
    bool isSelected();
    
    /**
     *  获取按钮状态：不可点击
     */
    
    bool isDisabled();

    /**
     *  判断按钮字符串标记是否与其他字符串相同
     */
    
    bool isTextTagEqual(const char* text);
    
    /**
     *  按钮点击映射的方法设置，枚举区分各种触摸状态
     */
    
    void addTarget(void* target, SEL_LMenu selector, LMenuType type);
    
    /**
     *  按钮触摸事件阻断与触摸优先级修改
     */
    
    void setTouchDispatcher(bool bSwallowsTouches, int nPriority = 0);
    
    /**
     *  按钮透明度设置
     */
    
    virtual void setOpacity(GLubyte opacity);
    
    /**
     *  按钮色值设置
     */
    
    virtual void setColor(const ccColor3B &color3);
    
    /**
     *  按钮色值获取
     */
    
    ccColor3B& getColor();
    
protected:
    
    virtual void onExit();
    
    virtual void onEnter();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void setNormalEffect();
    
    virtual void setHighlightedEffect();
    
    virtual void setDisabledEffect();
    
    virtual void setSelectedEffect();
    
    virtual void setNormalColor();
    
    virtual void setHighlightedColor();

    virtual void setDisabledColor();

    virtual void setSelectedColor();
    
protected:
    
    bool m_nbSwallowsTouches;
    
    int m_nTouchPriority;
    
    ccColor3B m_color;
    
    CCNode* m_bgNormal;
    
    CCNode* m_bgHighlighted;
    
    CCNode* m_bgDisabled;
    
    CCNode* m_bgSelected;
    
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
    
    void registerWithTouchDispatcher(void);
    
    void unRegisterWithTouchDispatcher(void);
    
    bool m_IsRehisterTouchDispatcher;
    
    CCRect getRect();
    
private:
    
    void* m_target;
    
    SEL_LMenu m_touchBegin;
    
    SEL_LMenu m_touchMoved;
    
    SEL_LMenu m_touchMovedOutSide;
    
    SEL_LMenu m_touchUpInSide;
    
    SEL_LMenu m_touchUpSide;
    
};

#endif /* defined(__LMenu__) */
