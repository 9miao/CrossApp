//
//  LTableBarController.h
//  project
//
//  Created by 栗元峰 on 14-3-18.
//
//

#ifndef __project__LTableBarController__
#define __project__LTableBarController__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "LMenu.h"
#include "LRootLayer.h"
#include "LTabBarMenu.h"
#include "MySocket.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

#define WINSIZE CCDirector::sharedDirector()->getWinSize()
#define TABBAR_CONTROLLER LTableBarController::shareTableBarController()

class LTableBarController
: public CCLayerColor
, public LTabBarMenuDelegate
{
    vector<LRootLayer*> m_pLayerVec;
    
    CCNode* m_pLayerParent;
    
    LMenu* m_pGoBack;
    
public:

    LTableBarController();
    
    virtual ~LTableBarController();
    
    void onEnterTransitionDidFinish();
    
    static LTableBarController* shareTableBarController();
    
    static void shareNull();
    
    void replaceLayer(LRootLayer* layer);
    
    void pushLayer(LRootLayer* layer);
    
    void popLayer();
    
    LRootLayer* getLayer(unsigned int index);
    
    LRootLayer* getCurrLayer();
    
protected:
    
    bool init();
    
    void clearLayerVector();
    
    void actionBegin();
    
    void actionEnd();
    
    void pushEnd();
    
    void popEnd();
    
    bool touchUpInSide(LMenu* menu, CCPoint point);
    
    void callBackTabBarMenu(const char* fileName);
};


#endif /* defined(__project__LTableBarController__) */
