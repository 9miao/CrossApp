//
//  LTMPShow.h
//  project
//
//  Created by 栗元峰 on 14-4-1.
//
//

#ifndef __project__LTMPShow__
#define __project__LTMPShow__

#include <iostream>

#include "cocos2d.h"
#include "cocos-ext.h"
#include "LTableBarController.h"

USING_NS_CC;
USING_NS_CC_EXT;

class LTMPShow: public LRootLayer
{

    
public:
    
    LTMPShow();
    
    ~LTMPShow();
    
    void onEnterTransitionDidFinish();
    
    void onExitTransitionDidStart();
    
    CREATE_FUNC(LTMPShow);
    
    bool init();
    
    virtual void show();
    
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};


#endif /* defined(__project__LTMPShow__) */
