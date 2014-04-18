//
//  LNoClickLayer.h
//  DDHero
//
//  Created by LI YUANFENG on 13-4-19.
//
//

#ifndef __DDHero__LNoClickLayer__
#define __DDHero__LNoClickLayer__

#include <iostream>
#include "cocos2d.h"
#define NOCLICK LNoClickLayer::shareLNoClickLayer()
#define NOALLCLICK LNoClickLayer::shareLNoAllClickLayer()
#define REMOVENOCLICK LRemoveNoClickLayer::RemoveNoClickLayer()

using namespace cocos2d;
class LNoClickLayer: public CCLayerColor {
    
    void onExit();
    
    void onEnter();
    
    void registerWithTouchDispatcher(void);
    
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    
    float _offy;
    
    static LNoClickLayer* shareLNoClickLayer();
    
    static LNoClickLayer* create();
    
    static LNoClickLayer* shareLNoAllClickLayer();
    
    static bool getIsExist();
    
    LNoClickLayer();
    
    virtual ~LNoClickLayer();
    
};

class LRemoveNoClickLayer {
    
public:
    
    static void RemoveNoClickLayer();
    
};

#endif /* defined(__DDHero__LNoClickLayer__) */
