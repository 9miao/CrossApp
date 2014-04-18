//
//  LRootLayer.h
//  project
//
//  Created by 栗元峰 on 14-3-18.
//
//

#ifndef __project__LRootLayer__
#define __project__LRootLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

#define CALL_BACK(_CLASS_, _INT_) _CLASS_::onClickTest_##_INT_

class LRootLayer: public CCLayerColor
{
    
public:
    
    bool init();
    
    virtual void setOpacity(GLubyte opacity);
    
    void setOpacityOfChildren(GLubyte opacity, CCNode* node);
    
    virtual void show() = 0;
    
};

#endif /* defined(__project__LRootLayer__) */
