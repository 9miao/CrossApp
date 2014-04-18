//
//  CCScrollViewX.h
//  scrollView
//
//  Created by mac on 13-10-23.
//
//

#ifndef __scrollView__CCScrollViewX__
#define __scrollView__CCScrollViewX__

#include <iostream>
#include "cocos-ext.h"
#include "cocos2d.h"
USING_NS_CC_EXT;
USING_NS_CC;

class CCScrollViewX: public CCScrollView, public CCScrollViewDelegate {
    
    virtual void scrollViewDidScroll(CCScrollView* view);
    
    virtual void scrollViewDidZoom(CCScrollView* view);
    
    static CCScrollViewX* create(CCSize size, CCNode* container = NULL);
    
    int _number;
    
    int _isNext;
    
    bool isActions;
    
    CCPoint m_touchBeginPoint;
    
    bool _isMoved;
    
    std::vector<std::string> m_array;
    
    std::vector<std::string> m_Urlarray;
    
    CCSprite* m_nImage[3];

    void initWithBg();
    
    void scrollViewActions();
    
    void actionsFinish();
    
    void startPlay();
    
    CCSprite* m_nInstructsContainer;
    
    std::vector<CCSprite*> m_nInstructs;
    
    CCSprite* m_nInstruct;
    
    void initWithInstructsContainer();
    
    void touchUpInSide();
    
    void registerWithTouchDispatcher(void);
    
public:
    
    CCScrollViewX();
    
    virtual ~CCScrollViewX();
    
    static CCScrollViewX* create();

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void addChild(CCNode * child, int zOrder, int tag);
    virtual void addChild(CCNode * child, int zOrder);
    virtual void addChild(CCNode * child);
};

#endif /* defined(__scrollView__CCScrollViewX__) */
