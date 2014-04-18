//
//  LTabBarMenu.h
//  project
//
//  Created by 栗元峰 on 14-3-18.
//
//

#ifndef __project__LTabBarMenu__
#define __project__LTabBarMenu__

#include <iostream>
#include "LMenu.h"

class LTabBarMenuDelegate
{
public:
    
    virtual void callBackTabBarMenu(const char* fileName) = 0;
    
};

class LTabBarMenu
: public CCSprite
{

    std::vector<LMenu*> m_pMenuVec;
    
    int m_nSelected;
    
    CCScale9Sprite* m_pSelected;
    
    bool m_touchProtected;
    
public:
    
    LTabBarMenu();
    
    ~LTabBarMenu();
    
    CREATE_FUNC(LTabBarMenu);
    
    void onEnterTransitionDidFinish();
    
    bool init();
    
    void addMenuWithTabBar(const char* fileName);
    
    CC_SYNTHESIZE(LTabBarMenuDelegate*, m_pDelegate, Delegate);
    
protected:
    
    bool touchUpInSide(LMenu* menu, CCPoint point);
    
    void touchProtected();
    
    void untouchProtected();
};

#endif /* defined(__project__LTabBarMenu__) */
